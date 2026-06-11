// utcommon/test_camera.cpp
#include <gtest/gtest.h>
#include <cmath>

#include "camera.hpp"
#include "vector.hpp"  // <- include directo para render::vector
#include "ray.hpp"     // <- include directo para render::ray

using render::camera;
using render::camera_pose;
using render::camera_optics;
using render::vector;
using render::ray;

namespace {  // helpers y utilidades

constexpr double kEps = 1e-12;

// get_origin / get_direction: soporta tanto r.origin() como r.origin
template <class R>
auto get_origin(const R& r) -> vector {
    if constexpr (requires { r.origin(); }) {
        return r.origin();
    } else {
        return r.origin;
    }
}
template <class R>
auto get_direction(const R& r) -> vector {
    if constexpr (requires { r.direction(); }) {
        return r.direction();
    } else {
        return r.direction;
    }
}

void ExpectVectorNear(const vector& a, const vector& b, double eps = kEps) {
    EXPECT_LE((a - b).magnitude(), eps);
}

template <class R1, class R2>
void ExpectRayNear(const R1& r1, const R2& r2, double eps = kEps) {
    ExpectVectorNear(get_origin(r1),    get_origin(r2),    eps);
    ExpectVectorNear(get_direction(r1), get_direction(r2), eps);
}

// Construye una cámara 2x2 con FOV=90°, aspect=1 para comprobaciones cerradas
camera MakeTinyCamera_2x2() {
    const camera_pose pose{
        /*origin=*/vector{0,0,0},
        /*look_at=*/vector{0,0,-1},
        /*up=*/vector{0,1,0}
    };
    const camera_optics opt(/*fov=*/90.0, /*w=*/2, /*h=*/2);
    return {pose, opt}; // braced-init (modernize-return-braced-init-list)
}

} // namespace

// ----------------------------- camera_optics ---------------------------------

TEST(CameraOptics, DefaultConstructorValues) {
    const camera_optics o;
    EXPECT_DOUBLE_EQ(o.fov_deg, 90.0);
    EXPECT_NEAR(o.aspect, 16.0/9.0, 1e-15);
    EXPECT_EQ(o.width, 1920);
    EXPECT_EQ(o.height, 1080);
}

TEST(CameraOptics, FovAspectConstructorSetsOnlyFovAndAspect) {
    const camera_optics o(60.0, 2.0);
    EXPECT_DOUBLE_EQ(o.fov_deg, 60.0);
    EXPECT_DOUBLE_EQ(o.aspect, 2.0);
    EXPECT_EQ(o.width, 1920);
    EXPECT_EQ(o.height, 1080);
}

TEST(CameraOptics, FovWidthHeightConstructorComputesAspect) {
    const camera_optics o(75.0, 4, 2);
    EXPECT_DOUBLE_EQ(o.fov_deg, 75.0);
    EXPECT_DOUBLE_EQ(o.aspect, 2.0);
    EXPECT_EQ(o.width, 4);
    EXPECT_EQ(o.height, 2);
}

TEST(CameraOptics, FovWidthZeroHeightUsesDefaultAspect) {
    const camera_optics o(45.0, 3, 0);
    EXPECT_DOUBLE_EQ(o.fov_deg, 45.0);
    EXPECT_NEAR(o.aspect, 16.0/9.0, 1e-15);
    EXPECT_EQ(o.width, 3);
    EXPECT_EQ(o.height, 0);
}

// -------------------------------- camera -------------------------------------

TEST(Camera, BasisAndGettersAreConsistent) {
    const camera cam = MakeTinyCamera_2x2();

    // forward ~ (0,0,-1); right ~ (1,0,0); up ~ (0,1,0)
    ExpectVectorNear(cam.forward(), vector{0,0,-1});
    ExpectVectorNear(cam.right(),   vector{1,0,0});
    ExpectVectorNear(cam.up(),      vector{0,1,0});

    EXPECT_EQ(cam.image_width(),  2);
    EXPECT_EQ(cam.image_height(), 2);

    EXPECT_NEAR(cam.forward().magnitude(), 1.0, 1e-15);
    EXPECT_NEAR(cam.right().magnitude(),   1.0, 1e-15);
    EXPECT_NEAR(cam.up().magnitude(),      1.0, 1e-15);

    const auto r = cam.primary_ray(0.0, 0.0);
    ExpectVectorNear(get_origin(r), cam.origin());
}

TEST(Camera, PrimaryRayNormalizedMatchesExpectedCorners) {
    const camera cam = MakeTinyCamera_2x2();

    // u=v=0 => dir = (-0.5, 0.5, -1)
    {
        const ray r = cam.primary_ray(0.0, 0.0);
        ExpectVectorNear(get_direction(r), vector{-0.5, 0.5, -1.0});
    }
    // u=v=1 => dir = (1.5, -1.5, -1)
    {
        const ray r = cam.primary_ray(1.0, 1.0);
        ExpectVectorNear(get_direction(r), vector{1.5, -1.5, -1.0});
    }
}

TEST(Camera, PrimaryRayFromPixelMatchesNormalizedCoordinates) {
    const camera cam = MakeTinyCamera_2x2();

    // (f=0,c=0, dx=dy=0) -> u=0, v=0 => igual que primary_ray(0,0)
    {
        const ray r_pix = cam.primary_ray_from_pixel(/*f=*/0, /*c=*/0, /*dx=*/0.0, /*dy=*/0.0);
        const ray r_uv  = cam.primary_ray(/*u=*/0.0, /*v=*/0.0);
        ExpectRayNear(r_pix, r_uv);
        ExpectVectorNear(get_direction(r_pix), vector{-0.5, 0.5, -1.0});
    }

    // (f=1,c=1, dx=dy=0) -> u=0.5, v=0.5 => dir = (0.5,-0.5,-1)
    {
        const ray r_pix = cam.primary_ray_from_pixel(/*f=*/1, /*c=*/1, /*dx=*/0.0, /*dy=*/0.0);
        ExpectVectorNear(get_direction(r_pix), vector{0.5, -0.5, -1.0});
    }

    // Jitter +0.5,+0.5 en (0,0) => dir pura forward (0,0,-1)
    {
        const ray r_pix = cam.primary_ray_from_pixel(/*f=*/0, /*c=*/0, /*dx=*/+0.5, /*dy=*/+0.5);
        ExpectVectorNear(get_direction(r_pix), vector{0.0, 0.0, -1.0});
    }
}

TEST(Camera, OriginGetterIsCameraPosition) {
    const camera_pose pose{ vector{3,2,1}, vector{3,2,0}, vector{0,1,0} };
    const camera_optics opt(60.0, 4, 2);
    const camera cam(pose, opt);
    ExpectVectorNear(cam.origin(), vector{3,2,1});
}

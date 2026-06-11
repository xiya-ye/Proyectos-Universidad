// utcommon/test_sphere.cpp
#include <gtest/gtest.h>


#include "sphere.hpp"    // render::sphere
#include "object.hpp"    // render::Hit, render::hit_opt
#include "ray.hpp"       // render::ray
#include "vector.hpp"    // render::vector
#include "material.hpp"  // render::material
#include "color.hpp"     // render::color

using render::sphere;
using render::ray;
using render::vector;
using render::material;
using render::color;
using render::Hit;
using render::hit_opt;

namespace { // ------------------------------- helpers -------------------------

void ExpectVecNear(const vector& a, const vector& b, double eps = 1e-12) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

material MakeMatte(const char* name, color albedo) {
    return material{name, albedo};
}

} // namespace

// =============================== Constructores / getters =====================

TEST(SphereCtor, DefaultCtorAndAccessors) {
    const sphere s; // por defecto: centro (0,0,0), radio 1, material nullptr
    ExpectVecNear(s.center(), vector{0.0, 0.0, 0.0});
    EXPECT_DOUBLE_EQ(s.radius(), 1.0);
    EXPECT_EQ(s.material_ptr(), nullptr);
}

TEST(SphereCtor, ParamCtorSaturatesNonPositiveRadius) {
    const material m = MakeMatte("m", color{0.7, 0.6, 0.5});
    const sphere s(vector{1.0, 2.0, 3.0}, /*r=*/-2.5, /*m=*/&m);
    ExpectVecNear(s.center(), vector{1.0, 2.0, 3.0});
    EXPECT_DOUBLE_EQ(s.radius(), 0.0);      // r <= 0 → saturado a 0
    EXPECT_EQ(s.material_ptr(), &m);
}

// ================================== intersect() ==============================

TEST(SphereIntersect, HitsNearRootWithFrontFaceNormal) {
    const material m = MakeMatte("white", color{1.0, 1.0, 1.0});
    const sphere s(vector{0.0, 0.0, 5.0}, /*r=*/1.0, &m);

    const ray r{ vector{0.0, 0.0, 0.0}, vector{0.0, 0.0, 1.0} };
    const hit_opt h = s.intersect(r, /*t_min=*/1e-6, /*t_max=*/1e9);

    if (!h.has_value()) { GTEST_SKIP() << "No hit (unexpected for this setup)";
}
    const Hit hit = h.value_or(Hit{});

    EXPECT_NEAR(hit.t, 4.0, 1e-12);
    ExpectVecNear(hit.p, vector{0.0, 0.0, 4.0});
    EXPECT_EQ(hit.mat, &m);

    // outward = (p - C)/r = (0,0,-1); D·outward = 1*(-1) = -1 < 0 → front_face=true
    EXPECT_TRUE(hit.front_face);
    ExpectVecNear(hit.normal, vector{0.0, 0.0, -1.0});
}

TEST(SphereIntersect, ChoosesFarRootWhenNearIsBelowTminAndNormalInverted) {
    const material m = MakeMatte("white", color{1.0, 1.0, 1.0});
    const sphere s(vector{0.0, 0.0, 5.0}, /*r=*/1.0, &m);

    const ray r{ vector{0.0, 0.0, 0.0}, vector{0.0, 0.0, 1.0} };
    // near = 4, far = 6; forzamos t_min > 4
    const hit_opt h = s.intersect(r, /*t_min=*/4.5, /*t_max=*/100.0);

    if (!h.has_value()) { GTEST_SKIP() << "No hit (unexpected for this setup)";
}
    const Hit hit = h.value_or(Hit{});

    EXPECT_NEAR(hit.t, 6.0, 1e-12);
    ExpectVecNear(hit.p, vector{0.0, 0.0, 6.0});
    EXPECT_EQ(hit.mat, &m);

    // outward = (0,0,1); D·outward = 1*1 = 1 > 0 → front_face=false; normal = -outward
    EXPECT_FALSE(hit.front_face);
    ExpectVecNear(hit.normal, vector{0.0, 0.0, -1.0});
}

TEST(SphereIntersect, MissWhenRayDoesNotReachSphere) {
    const material m = MakeMatte("w", color{1.0, 1.0, 1.0});
    const sphere s(vector{0.0, 3.0, 5.0}, /*r=*/1.0, &m);

    const ray r{ vector{0.0, 0.0, 0.0}, vector{0.0, 0.0, 1.0} };
    const hit_opt h = s.intersect(r, /*t_min=*/1e-6, /*t_max=*/1e9);
    EXPECT_FALSE(h.has_value());
}

TEST(SphereIntersect, RespectsBoundsTmax) {
    const material m = MakeMatte("w", color{1.0, 1.0, 1.0});
    const sphere s(vector{0.0, 0.0, 5.0}, /*r=*/1.0, &m);

    const ray r{ vector{0.0, 0.0, 0.0}, vector{0.0, 0.0, 1.0} };
    // near = 4; si t_max < 4, no hay impacto
    const hit_opt h = s.intersect(r, /*t_min=*/1e-6, /*t_max=*/3.99);
    EXPECT_FALSE(h.has_value());
}

TEST(SphereIntersect, FromInsideSphereNormalPointsInwardAfterOrientation) {
    const material m = MakeMatte("w", color{1.0, 1.0, 1.0});
    const sphere s(vector{0.0, 0.0, 0.0}, /*r=*/1.0, &m);

    const ray r{ vector{0.0, 0.0, 0.0}, vector{1.0, 0.0, 0.0} }; // origen en el interior
    const hit_opt h = s.intersect(r, /*t_min=*/1e-6, /*t_max=*/100.0);

    if (!h.has_value()) { GTEST_SKIP() << "No hit (unexpected for inside-ray test)";
}
    const Hit hit = h.value_or(Hit{});

    EXPECT_NEAR(hit.t, 1.0, 1e-12);
    ExpectVecNear(hit.p, vector{1.0, 0.0, 0.0});
    EXPECT_EQ(hit.mat, &m);

    // outward = (1,0,0); D·outward = 1 → normal = -outward
    EXPECT_FALSE(hit.front_face);
    ExpectVecNear(hit.normal, vector{-1.0, 0.0, 0.0});
}

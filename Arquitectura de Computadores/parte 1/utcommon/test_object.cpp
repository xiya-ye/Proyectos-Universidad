// utcommon/test_object.cpp
#include <gtest/gtest.h>

#include <optional>
#include <type_traits>
#include <cmath>

#include "object.hpp"  // render::Hit, render::hit_opt, render::object
#include "vector.hpp"  // render::vector
#include "ray.hpp"     // render::ray

using render::Hit;
using render::hit_opt;
using render::object;
using render::ray;
using render::vector;

namespace { // helpers (evita misc-use-anonymous-namespace)

constexpr double kEps = 1e-12;

void ExpectVecNear(const vector& a, const vector& b, double eps = kEps) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

// Obtiene el valor de un optional ya comprobado sin gatillar bugprone-unchecked-optional-access
template <class T>
T CheckedValueOrFail(const std::optional<T>& opt, const char* what) {
    if (!opt.has_value()) {
        ADD_FAILURE() << "Expected " << what << " but got empty optional";
        return T{};
    }
    return *opt; // la dereferencia está protegida por el if anterior
}

// Objeto de prueba: plano z=0, normal hacia +Z (outward = (0,0,1)).
class fake_plane final : public object {
public:
    [[nodiscard]] hit_opt intersect(const ray& r, double t_min, double t_max) const noexcept override {
        const double denom = r.direction.z;
        if (std::fabs(denom) < 1e-12) { return std::nullopt; } // paralelo al plano
        const double t = -r.origin.z / denom;                   // z=0 => t = -Oz/Dz
        if (t < t_min or t > t_max) { return std::nullopt; }
        Hit h{};
        h.t = t;
        h.p = r.at(t);
        h.mat = nullptr;              // no necesitamos material aquí
        const vector outward{0,0,1};  // unitario
        h.set_face_normal(r, outward);
        return h;
    }
};

} // namespace

// -----------------------------------------------------------------------------
// Rasgos de tipo y alias
// -----------------------------------------------------------------------------
TEST(ObjectTraits, ObjectIsAbstractAndHasVirtualDtorAndIsNotCopyableMovable) {
    static_assert(std::is_abstract_v<object>);
    static_assert(std::has_virtual_destructor_v<object>);
    static_assert(!std::is_copy_constructible_v<object>);
    static_assert(!std::is_move_constructible_v<object>);
    static_assert(!std::is_copy_assignable_v<object>);
    static_assert(!std::is_move_assignable_v<object>);
}

TEST(ObjectTraits, HitOptIsOptionalOfHit) {
    static_assert(std::is_same_v<hit_opt, std::optional<Hit>>);
}

// -----------------------------------------------------------------------------
// Hit::set_face_normal — exterior (dot < 0), interior (dot > 0), borde (dot == 0)
// -----------------------------------------------------------------------------
TEST(HitSetFaceNormal, OutsideKeepsOutwardNormal) {
    Hit h{};
    const ray r{/*o=*/vector{0,1,0}, /*d=*/vector{0,-1,0}}; // hacia abajo
    const vector outward{0,1,0}; // +Y
    h.set_face_normal(r, outward);
    EXPECT_TRUE(h.front_face);
    ExpectVecNear(h.normal, outward);
}

TEST(HitSetFaceNormal, InsideFlipsNormal) {
    Hit h{};
    const ray r{/*o=*/vector{0,-1,0}, /*d=*/vector{0,1,0}}; // hacia arriba
    const vector outward{0,1,0}; // +Y
    h.set_face_normal(r, outward);
    EXPECT_FALSE(h.front_face);
    ExpectVecNear(h.normal, vector{0,-1,0});
}

TEST(HitSetFaceNormal, ZeroDotCountsAsInsideAndFlips) {
    Hit h{};
    const ray r{/*o=*/vector{0,0,0}, /*d=*/vector{1,0,0}}; // perpendicular a n
    const vector outward{0,1,0}; // +Y
    h.set_face_normal(r, outward);
    EXPECT_FALSE(h.front_face);                      // dot == 0 => no es <0 → se invierte
    ExpectVecNear(h.normal, vector{0,-1,0});
}

// -----------------------------------------------------------------------------
// object::intersect — ejercido vía fake_plane (plano z=0)
// -----------------------------------------------------------------------------
TEST(ObjectIntersect, PlaneHitWithinRangeReturnsHit) {
    const fake_plane pl;
    const ray r{/*o=*/vector{0,0,1}, /*d=*/vector{0,0,-1}}; // cae a z=0 en t=1
    const hit_opt h = pl.intersect(r, /*t_min=*/0.0, /*t_max=*/10.0);
    const Hit hit = CheckedValueOrFail(h, "plane hit");
    EXPECT_NEAR(hit.t, 1.0, 1e-12);
    ExpectVecNear(hit.p, vector{0,0,0});
    EXPECT_TRUE(hit.front_face);                 // dot(D, outward) = -1 < 0 → exterior
    ExpectVecNear(hit.normal, vector{0,0,1});    // se mantiene outward
}

TEST(ObjectIntersect, PlaneMissWhenParallel) {
    const fake_plane pl;
    const ray r{/*o=*/vector{0,0,1}, /*d=*/vector{1,0,0}}; // paralelo al plano z=0
    const hit_opt h = pl.intersect(r, 0.0, 10.0);
    EXPECT_FALSE(h.has_value());
}

TEST(ObjectIntersect, PlaneMissWhenOutsideRange) {
    const fake_plane pl;
    const ray r{/*o=*/vector{0,0,1}, /*d=*/vector{0,0,-1}}; // t geométrico = 1
    const hit_opt h1 = pl.intersect(r, /*t_min=*/1.1, /*t_max=*/10.0); // por debajo de t_min
    EXPECT_FALSE(h1.has_value());
    const hit_opt h2 = pl.intersect(r, /*t_min=*/0.0, /*t_max=*/0.9);  // por encima de t_max
    EXPECT_FALSE(h2.has_value());
}

TEST(ObjectIntersect, PlaneHitFromBelowFlipsNormal) {
    const fake_plane pl;
    const ray r{/*o=*/vector{0,0,-1}, /*d=*/vector{0,0,1}}; // sube a z=0 en t=1
    const hit_opt h = pl.intersect(r, 0.0, 10.0);
    const Hit hit = CheckedValueOrFail(h, "plane hit from below");
    EXPECT_FALSE(hit.front_face);                // dot(D, outward) = +1 > 0 → interior
    ExpectVecNear(hit.normal, vector{0,0,-1});   // invertida
}

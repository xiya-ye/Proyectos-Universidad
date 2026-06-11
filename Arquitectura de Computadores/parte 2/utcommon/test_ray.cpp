// utcommon/test_ray.cpp
#include <gtest/gtest.h>

#include <type_traits>
#include <cmath>
#include <utility>

#include "ray.hpp"     // render::ray
#include "vector.hpp"  // render::vector

using render::ray;
using render::vector;

namespace { // helpers (evita misc-use-anonymous-namespace)
constexpr double kEps = 1e-12;

void ExpectVecNear(const vector& a, const vector& b, double eps = kEps) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}
} // namespace

// -----------------------------------------------------------------------------
// Rasgos básicos del tipo (ligero y sin requerimientos especiales)
// -----------------------------------------------------------------------------
TEST(RayTraits, BasicProperties) {
    static_assert(std::is_default_constructible_v<ray>);
    static_assert(std::is_nothrow_default_constructible_v<ray>);
    static_assert(std::is_trivially_destructible_v<ray>);

    // La función at() está marcada noexcept en la cabecera
    static_assert(noexcept(std::declval<const ray&>().at(0.0)));
}

// -----------------------------------------------------------------------------
// Constructores
// -----------------------------------------------------------------------------
TEST(RayCtor, ValueCtorStoresMembers) {
    const vector o{1.0, 2.0, 3.0};
    const vector d{-4.0, 0.5, 7.0};
    const ray r{o, d};

    ExpectVecNear(r.origin, o);
    ExpectVecNear(r.direction, d);
}

TEST(RayCtor, DefaultCtorIsConsistentWithAtZero) {
    const ray r{};                  // valores por defecto del proyecto
    const vector p0 = r.at(0.0);    // no ignoramos [[nodiscard]]
    // En cualquier caso, debe cumplirse P(0) = Or
    ExpectVecNear(p0, r.origin);
}

// -----------------------------------------------------------------------------
// at(t): P(t) = Or + t·D (sin exigir D unitario)
// -----------------------------------------------------------------------------
TEST(RayAt, ReturnsOriginPlusTDirection) {
    const ray r{ vector{1.0, -2.0, 0.5}, vector{2.0, 3.0, -4.0} };

    const vector p1 = r.at(0.0);    // Or
    ExpectVecNear(p1, r.origin);

    const vector p2 = r.at(1.0);    // Or + D
    ExpectVecNear(p2, vector{ 1.0+2.0, -2.0+3.0, 0.5-4.0 });

    const vector p3 = r.at(2.5);    // Or + 2.5·D
    ExpectVecNear(p3, vector{ 1.0+2.5*2.0, -2.0+2.5*3.0, 0.5+2.5*(-4.0) });
}

TEST(RayAt, WorksWithNegativeAndFractionalT) {
    const ray r{ vector{0.0, 0.0, 0.0}, vector{0.0, 10.0, 0.0} };

    const vector p_neg = r.at(-0.25);  // retrocede 0.25 en la dirección
    ExpectVecNear(p_neg, vector{0.0, -2.5, 0.0});

    const vector p_frac = r.at(0.125); // avanza 1.25
    ExpectVecNear(p_frac, vector{0.0, 1.25, 0.0});
}

TEST(RayAt, DirectionNeedNotBeUnitLength) {
    // No exigimos normalización de D: la fórmula es puramente lineal
    const ray r{ vector{5.0, 5.0, 5.0}, vector{100.0, 0.0, -50.0} };

    const vector p = r.at(0.01); // pequeño paso
    ExpectVecNear(p, vector{ 5.0 + 1.0, 5.0 + 0.0, 5.0 - 0.5 });
}

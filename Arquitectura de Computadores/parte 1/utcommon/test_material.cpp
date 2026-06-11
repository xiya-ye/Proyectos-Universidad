// utcommon/test_material.cpp
#include <gtest/gtest.h>

#include <cmath>
#include <optional>

#include "material.hpp"  // render::material
#include "ray.hpp"       // render::ray
#include "vector.hpp"    // render::vector
#include "color.hpp"     // render::color
#include "object.hpp"    // render::Hit
#include "sampler.hpp"   // render::sampler (RNG compartido)

using render::material;
using render::ray;
using render::vector;
using render::color;
using render::Hit;
using render::sampler;

namespace { // helpers (evita misc-use-anonymous-namespace)

constexpr double kEps = 1e-12;

// Comparación de vectores
void ExpectVecNear(const vector& a, const vector& b, double eps = kEps) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

// Comparación de colores
void ExpectColorNear(const color& a, const color& b, double eps = 1e-12) {
    EXPECT_NEAR(a.r, b.r, eps);
    EXPECT_NEAR(a.g, b.g, eps);
    EXPECT_NEAR(a.b, b.b, eps);
}

// Obtiene el valor de un optional ya comprobado sin gatillar bugprone-unchecked-optional-access
template <class T>
T CheckedValueOrFail(const std::optional<T>& opt, const char* what) {
    if (!opt.has_value()) {
        ADD_FAILURE() << "Expected " << what << " but got empty optional";
        return T{};
    }
    return *opt;
}

// Reimplementación local de reflect (igual a la de material.cpp) para expectativas
vector Reflect(const vector& d, const vector& n) noexcept {
    return d - 2.0 * d.dot(n) * n;
}

// Construye un Hit con normal orientada coherente (set_face_normal) en p
Hit MakeHit(const vector& p, const vector& outward_n, const ray& r_in) {
    Hit h{};
    h.p = p;
    h.set_face_normal(r_in, outward_n);
    return h;
}

} // namespace

// -----------------------------------------------------------------------------
// MATE: clamp de albedo y propiedades básicas de scatter
// -----------------------------------------------------------------------------
TEST(Material, MatteScatter_ClampsAlbedoAndSetsOrigin) {
    const color input{1.2, -0.1, 0.5};     // fuera de rango en r y g
    const color expected{1.0, 0.0, 0.5};   // clamp a [0,1]
    const material m{"matte", input};

    // Rayo entrante hacia abajo, normal +Y; punto en el origen
    const ray rin{/*o=*/vector{0, 10, 0}, /*d=*/vector{0, -1, 0}};
    const Hit h = MakeHit(/*p=*/vector{0,0,0}, /*outward_n=*/vector{0,1,0}, rin);

    sampler rng{123456789ULL}; // semilla fija (determinista, aunque no dependemos del valor exacto)
    auto sc = m.scatter(rin, h, rng);
    const auto [rout, atten] = CheckedValueOrFail(sc, "matte scatter");

    // Origen
    ExpectVecNear(rout.origin, h.p);

    // Atenuación = albedo clamped
    ExpectColorNear(atten, expected);

    // Dirección = n + jitter → cada componente ∈ [n_i - 1, n_i + 1]
    const vector n = h.normal;
    const vector dr = rout.direction;
    EXPECT_GE(dr.x, n.x - 1.0); EXPECT_LE(dr.x, n.x + 1.0);
    EXPECT_GE(dr.y, n.y - 1.0); EXPECT_LE(dr.y, n.y + 1.0);
    EXPECT_GE(dr.z, n.z - 1.0); EXPECT_LE(dr.z, n.z + 1.0);
}

// -----------------------------------------------------------------------------
// METAL: fuzz=0 (reflexión pura) y fuzz>0 (ruido acotado)
// -----------------------------------------------------------------------------
TEST(Material, MetalScatter_FuzzZero_IsUnitReflect) {
    const color albedo{0.2, 0.3, 0.4};
    const material m{"metal", albedo, /*fuzz=*/0.0};

    // Dirección oblicua hacia abajo, normal +Y en el origen
    const ray rin{/*o=*/vector{0, 1, 0}, /*d=*/vector{1, -1, 0}};
    const Hit h = MakeHit(/*p=*/vector{0,0,0}, /*outward_n=*/vector{0,1,0}, rin);

    sampler rng{42ULL}; // no influye con fuzz=0
    auto sc = m.scatter(rin, h, rng);
    const auto [rout, atten] = CheckedValueOrFail(sc, "metal scatter (fuzz=0)");

    // Atenuación
    ExpectColorNear(atten, albedo);

    // Reflexión perfecta y normalizada
    const vector d1r = Reflect(rin.direction, h.normal);
    const vector d1rhat = d1r.normalize();
    ExpectVecNear(rout.direction, d1rhat, 1e-12);
    ExpectVecNear(rout.origin, h.p, 1e-12);
}

TEST(Material, MetalScatter_FuzzAddsUniformNoisePerComponent) {
    const color albedo{0.7, 0.6, 0.5};
    const double fuzz = 0.25;
    const material m{"metal", albedo, fuzz};

    const ray rin{/*o=*/vector{0, 1, 0}, /*d=*/vector{-1, -1, 0}};
    const Hit h = MakeHit(/*p=*/vector{0,0,0}, /*outward_n=*/vector{0,1,0}, rin);

    sampler rng{2024ULL};
    auto sc = m.scatter(rin, h, rng);
    const auto [rout, atten] = CheckedValueOrFail(sc, "metal scatter (fuzz>0)");

    ExpectColorNear(atten, albedo);

    const vector d1rhat = Reflect(rin.direction, h.normal).normalize();
    const vector phi = rout.direction - d1rhat;

    // Cada componente del ruido debe caer en [-fuzz, +fuzz]
    EXPECT_GE(phi.x, -fuzz - 1e-12); EXPECT_LE(phi.x, fuzz + 1e-12);
    EXPECT_GE(phi.y, -fuzz - 1e-12); EXPECT_LE(phi.y, fuzz + 1e-12);
    EXPECT_GE(phi.z, -fuzz - 1e-12); EXPECT_LE(phi.z, fuzz + 1e-12);
}

// -----------------------------------------------------------------------------
// REFRACTIVO: TIR al salir y refracción al entrar (incidencia normal)
// -----------------------------------------------------------------------------
TEST(Material, RefractiveScatter_TotalInternalReflectionOnExit) {
    const double ior = 1.5;
    const material m{"glass", ior};

    // Para TIR: front_face = false (saliendo) y sinθ ≈ 1.
    // Usamos do_hat ⟂ n: (1,0,0). Con n=+Y y front_face=false ⇒ ρ=ior > 1 ⇒ TIR.
    const ray rin{/*o=*/vector{0, 0, 0}, /*d=*/vector{1, 0, 0}};
    // outward +Y; dot(dir, outward)=0 ⇒ front_face=false
    const Hit h = MakeHit(/*p=*/vector{0,0,0}, /*outward_n=*/vector{0,1,0}, rin);

    sampler rng{7ULL}; // no influye en refractivo
    auto sc = m.scatter(rin, h, rng);
    const auto [rout, atten] = CheckedValueOrFail(sc, "refractive TIR");

    // Atenuación blanca
    ExpectColorNear(atten, color{1.0, 1.0, 1.0});

    // TIR ⇒ reflexión especular
    const vector expected = Reflect(rin.direction.normalize(), h.normal);
    ExpectVecNear(rout.direction, expected, 1e-12);
    ExpectVecNear(rout.origin, h.p, 1e-12);
}

TEST(Material, RefractiveScatter_RefractsOnEnter_NormalIncidence) {
    const double ior = 1.5;
    const material m{"glass", ior};

    // Entrada con incidencia normal: do_hat = (0,-1,0), n=+Y → dr = -n
    const ray rin{/*o=*/vector{0, 1, 0}, /*d=*/vector{0, -1, 0}};
    const Hit h = MakeHit(/*p=*/vector{0,0,0}, /*outward_n=*/vector{0,1,0}, rin); // front_face=true

    sampler rng{99ULL};
    auto sc = m.scatter(rin, h, rng);
    const auto [rout, atten] = CheckedValueOrFail(sc, "refractive enter normal incidence");

    ExpectColorNear(atten, color{1.0, 1.0, 1.0});
    // A incidencia normal, la dirección transmitida es -n
    ExpectVecNear(rout.direction, vector{0, -1, 0}, 1e-12);
    ExpectVecNear(rout.origin, h.p, 1e-12);
}

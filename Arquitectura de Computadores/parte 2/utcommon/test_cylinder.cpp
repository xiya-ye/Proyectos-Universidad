// utcommon/test_cylinder.cpp
#include <gtest/gtest.h>

#include <cmath>
#include <optional>

#include "cylinder.hpp"  // render::cylinder
#include "object.hpp"    // render::Hit
#include "ray.hpp"       // render::ray
#include "vector.hpp"    // render::vector
#include "material.hpp"  // render::material
#include "color.hpp"     // render::color

using render::cylinder;
using render::ray;
using render::vector;
using render::material;
using render::color;

namespace { // helpers y constantes

constexpr double kEps = 1e-12;
constexpr double kInf = 1e9;

material MakeMatteWhite() {
    return material{"m_white", color{1.0, 1.0, 1.0}};
}

void ExpectVecNear(const vector& a, const vector& b, double eps = kEps) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

// Devuelve un Hit comprobado SIN usar .value() ni *opt en el sitio del test.
// Si falta, registra fallo y devuelve un Hit por defecto (no se usará en éxito).
template <class T>
T CheckedValueOrFail(const std::optional<T>& opt, const char* what) {
    if (!opt.has_value()) {
        ADD_FAILURE() << "Expected " << what << " but got empty optional";
        return T{};
    }
    return *opt; // la dereferencia está en una rama garantizada por el if
}

} // namespace

// Cilindro en el origen, r=1, eje (0,2,0) => h=2, half=1.

TEST(Cylinder, HitsCurvedSurface_ReturnsNearestRoot) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{2,0,0}, /*d=*/vector{-1,0,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    const render::Hit hit = CheckedValueOrFail(h, "hit on curved surface");

    EXPECT_NEAR(hit.t, 1.0, 1e-12);
    ExpectVecNear(hit.p, vector{1,0,0});
    EXPECT_EQ(hit.mat, &mat);
}

TEST(Cylinder, CurvedSurfaceIsRejectedIfOutsideHeight) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{2,1.1,0}, /*d=*/vector{-1,0,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    EXPECT_FALSE(h.has_value());
}

TEST(Cylinder, HitsTopCap) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{0,3,0}, /*d=*/vector{0,-1,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    const render::Hit hit = CheckedValueOrFail(h, "hit on top cap");

    EXPECT_NEAR(hit.t, 2.0, 1e-12);
    ExpectVecNear(hit.p, vector{0,1,0});
    EXPECT_EQ(hit.mat, &mat);
}

TEST(Cylinder, RayParallelToCaps_NoCapHit) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{0,1,2}, /*d=*/vector{1,0,0}}; // paralelo a tapas, lejos de lateral
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    EXPECT_FALSE(h.has_value());
}

TEST(Cylinder, DegenerateAxisZero_OnlyCapsRemainCoincident) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,0,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{0,2,0}, /*d=*/vector{0,-1,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    const render::Hit hit = CheckedValueOrFail(h, "hit with degenerate axis");

    EXPECT_NEAR(hit.t, 2.0, 1e-12);
    ExpectVecNear(hit.p, vector{0,0,0});
    EXPECT_EQ(hit.mat, &mat);
}

TEST(Cylinder, MissesCompletely) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{5,0,0}, /*d=*/vector{1,0,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/kInf);
    EXPECT_FALSE(h.has_value());
}

TEST(Cylinder, CurvedSurface_PicksNearestRootWithinRange) {
    const material mat = MakeMatteWhite();
    const cylinder c{/*C=*/vector{0,0,0}, /*r=*/1.0, /*axis=*/vector{0,2,0}, /*m=*/&mat};

    const ray r{/*o=*/vector{2,0,0}, /*d=*/vector{-1,0,0}};
    auto h = c.intersect(r, /*t_min=*/0.0, /*t_max=*/1.5); // fuerza raíz cercana
    const render::Hit hit = CheckedValueOrFail(h, "nearest root within range");

    EXPECT_NEAR(hit.t, 1.0, 1e-12);
    ExpectVecNear(hit.p, vector{1,0,0});
}

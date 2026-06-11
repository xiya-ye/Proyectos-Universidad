// utcommon/test_color.cpp
#include <gtest/gtest.h>

#include <cmath>        // std::pow
#include <cstdint>      // std::uint8_t
#include <type_traits>  // std::is_same_v

#include "color.hpp"    // render::color

using render::color;

namespace { // helpers

constexpr double kEps = 1e-12;

void ExpectColorNear(const color& a, const color& b, double eps = kEps) {
    EXPECT_NEAR(a.r, b.r, eps);
    EXPECT_NEAR(a.g, b.g, eps);
    EXPECT_NEAR(a.b, b.b, eps);
}

} // namespace

// =============================== COMPILE-TIME ================================
// Verifica que las funciones marcadas constexpr realmente se evalúan en CE.

static_assert(std::is_same_v<decltype(2.0 * color{}), color>,
              "operator*(double, color) debe devolver color");

namespace { // scope para constantes constexpr

constexpr color kZero{};
static_assert(kZero.r == 0.0 and kZero.g == 0.0 and kZero.b == 0.0,
              "constructor por defecto debe inicializar a 0");

constexpr color kC{0.25, 0.5, 1.0};
constexpr color kClampOk = kC.clamp();
static_assert(kClampOk.r == 0.25 and kClampOk.g == 0.5 and kClampOk.b == 1.0,
              "clamp no debe alterar valores ya en [0,1]");

constexpr color kC2{2.0, -1.0, 0.5};
constexpr color kClampEdge = kC2.clamp();
static_assert(kClampEdge.r == 1.0 and kClampEdge.g == 0.0 and kClampEdge.b == 0.5,
              "clamp debe saturar a [0,1]");

constexpr color kA{1.0, 2.0, 3.0};
constexpr color kB{4.0, 5.0, 6.0};
constexpr color kSum = kA + kB;
static_assert(kSum.r == 5.0 and kSum.g == 7.0 and kSum.b == 9.0,
              "suma canal a canal incorrecta");

constexpr color kScaleRight = kA * 2.0;
static_assert(kScaleRight.r == 2.0 and kScaleRight.g == 4.0 and kScaleRight.b == 6.0,
              "escala por la derecha incorrecta");

constexpr color kScaleLeft  = 2.0 * kA;
static_assert(kScaleLeft.r == 2.0 and kScaleLeft.g == 4.0 and kScaleLeft.b == 6.0,
              "escala por la izquierda incorrecta");

constexpr color kHadamard = kA * kB;
static_assert(kHadamard.r == 4.0 and kHadamard.g == 10.0 and kHadamard.b == 18.0,
              "producto Hadamard incorrecto");

constexpr auto kRgb255 = color{1.0, 0.5, 0.0}.to_rgb255();
static_assert(std::get<0>(kRgb255) == 255, "to_rgb255 R");
static_assert(std::get<1>(kRgb255) == 127, "to_rgb255 G (127.5 -> 127 truncado)");
static_assert(std::get<2>(kRgb255) == 0,   "to_rgb255 B");

} // namespace

// ================================ RUNTIME ====================================

TEST(Color, ConstructorAndMembers) {
    const color c{0.1, 0.2, 0.3};
    EXPECT_DOUBLE_EQ(c.r, 0.1);
    EXPECT_DOUBLE_EQ(c.g, 0.2);
    EXPECT_DOUBLE_EQ(c.b, 0.3);

    const color d{};
    EXPECT_DOUBLE_EQ(d.r, 0.0);
    EXPECT_DOUBLE_EQ(d.g, 0.0);
    EXPECT_DOUBLE_EQ(d.b, 0.0);
}

TEST(Color, ClampClipsToRange) {
    const color c{-0.2, 0.4, 1.4};
    const color cl = c.clamp();
    ExpectColorNear(cl, color{0.0, 0.4, 1.0});
}

TEST(Color, ApplyGammaUsesClampedAndExponent) {
    // Valores fuera de rango se clipean antes de pow()
    const color c{2.0, -1.0, 0.25};
    const double gamma = 2.0;
    const color cg = c.apply_gamma(gamma);

    // Esperados: pow(1, 1/2)=1 ; pow(0, 1/2)=0 ; pow(0.25, 1/2)=0.5
    ExpectColorNear(cg, color{1.0, 0.0, 0.5});
}

TEST(Color, ApplyGammaWithGammaOneIsIdentityOnClamped) {
    const color c{-0.1, 0.3, 1.2};
    const color cg = c.apply_gamma(1.0);
    // clamp -> (0,0.3,1); pow(x,1)=x
    ExpectColorNear(cg, color{0.0, 0.3, 1.0});
}

TEST(Color, ToRgb255TruncatesAfterScalingAndClamps) {
    // Dentro de rango
    {
        const color c{0.0, 0.5, 0.999};
        const auto [R, G, B] = c.to_rgb255();
        EXPECT_EQ(R, 0);
        EXPECT_EQ(G, 127);   // 127.5 -> 127 (truncado)
        EXPECT_EQ(B, 254);   // 254.745 -> 254
    }
    // Fuera de rango (clamp -> 0 o 255)
    {
        const color c{-0.1, 1.4, 0.2};
        const auto [R, G, B] = c.to_rgb255();
        EXPECT_EQ(R, 0);
        EXPECT_EQ(G, 255);
        EXPECT_EQ(B, static_cast<std::uint8_t>(0.2 * 255.0)); // 51
    }
}

TEST(Color, AdditionAndHadamardAndScalar) {
    const color a{0.1, 0.2, 0.3};
    const color b{0.9, 0.8, 0.7};

    // Suma
    const color s = a + b;
    ExpectColorNear(s, color{1.0, 1.0, 1.0});

    // Hadamard (conmutativo)
    const color h1 = a * b;
    const color h2 = b * a;
    ExpectColorNear(h1, h2);

    // Escala por la derecha e izquierda (deben coincidir)
    const color sr = a * 2.0;
    const color sl = 2.0 * a;
    ExpectColorNear(sr, sl);
}


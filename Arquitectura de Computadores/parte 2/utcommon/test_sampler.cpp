// utcommon/test_sampler.cpp
#include <gtest/gtest.h>

#include <cstdint>
#include <cmath>
#include <utility>

#include "sampler.hpp" // render::sampler
#include "vector.hpp"  // render::vector

using render::sampler;
using render::vector;

namespace { // helpers

constexpr double kEps = 1e-12;

template <class T>
void ExpectInRangeInclusive(const T v, const T lo, const T hi) {
    EXPECT_LE(v, hi);
    EXPECT_GE(v, lo);
}

template <class T>
void ExpectInRangeHalfOpen(const T v, const T lo, const T hi) {
    EXPECT_LT(v, hi);
    EXPECT_GE(v, lo);
}

} // namespace

// -----------------------------------------------------------------------------
// Rasgos y noexcept
// -----------------------------------------------------------------------------
TEST(SamplerTraits, NoexceptAndConstructible) {
    static_assert(noexcept(sampler{0ULL}));
    static_assert(noexcept(std::declval<sampler&>().next01()));
    static_assert(noexcept(std::declval<sampler&>().next_signed01()));
    static_assert(noexcept(std::declval<sampler&>().next_m11()));
    static_assert(noexcept(std::declval<sampler&>().uniform(0.0, 1.0)));
    static_assert(noexcept(std::declval<sampler&>().jitter_half()));
    static_assert(noexcept(std::declval<sampler&>().random_m11_vector()));
}

// -----------------------------------------------------------------------------
// Determinismo por semilla — tests separados para reducir complejidad
// -----------------------------------------------------------------------------
TEST(SamplerDeterminism, Next01SameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const double v1 = a.next01();
        const double v2 = b.next01();
        EXPECT_DOUBLE_EQ(v1, v2);
    }
}

TEST(SamplerDeterminism, NextSigned01SameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const double v1 = a.next_signed01();
        const double v2 = b.next_signed01();
        EXPECT_DOUBLE_EQ(v1, v2);
    }
}

TEST(SamplerDeterminism, NextM11SameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const double v1 = a.next_m11();
        const double v2 = b.next_m11();
        EXPECT_DOUBLE_EQ(v1, v2);
    }
}

TEST(SamplerDeterminism, UniformSameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    constexpr double A = -2.5, B = 7.25;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const double u1 = a.uniform(A, B);
        const double u2 = b.uniform(A, B);
        EXPECT_DOUBLE_EQ(u1, u2);
    }
}

TEST(SamplerDeterminism, JitterHalfSameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const double j1 = a.jitter_half();
        const double j2 = b.jitter_half();
        EXPECT_DOUBLE_EQ(j1, j2);
    }
}

TEST(SamplerDeterminism, RandomM11VectorSameSeed) {
    constexpr std::uint64_t seed = 123456789ULL;
    constexpr int N = 64;
    sampler a(seed), b(seed);
    for (int i = 0; i < N; ++i) {
        const vector v1 = a.random_m11_vector();
        const vector v2 = b.random_m11_vector();
        EXPECT_DOUBLE_EQ(v1.x, v2.x);
        EXPECT_DOUBLE_EQ(v1.y, v2.y);
        EXPECT_DOUBLE_EQ(v1.z, v2.z);
    }
}

// -----------------------------------------------------------------------------
// next01(): rango [0,1)
// -----------------------------------------------------------------------------
TEST(SamplerRanges, Next01IsInHalfOpenUnitInterval) {
    sampler s(42ULL);
    for (int i = 0; i < 1000; ++i) {
        const double v = s.next01();
        ExpectInRangeHalfOpen(v, 0.0, 1.0);
    }
}

// -----------------------------------------------------------------------------
// next_signed01() y next_m11(): rango [-1,1) y equivalencia
// -----------------------------------------------------------------------------
TEST(SamplerRanges, NextSignedAndM11AreInMinus1To1HalfOpenAndEqual) {
    sampler s1(777ULL);
    sampler s2(777ULL);
    for (int i = 0; i < 1000; ++i) {
        const double a = s1.next_signed01();
        const double b = s2.next_m11(); // alias de next_signed01
        ExpectInRangeHalfOpen(a, -1.0, 1.0);
        ExpectInRangeHalfOpen(b, -1.0, 1.0);
        EXPECT_DOUBLE_EQ(a, b);
    }
}

// -----------------------------------------------------------------------------
// uniform(a,b): mapeo afín desde next01()
// -----------------------------------------------------------------------------
TEST(SamplerUniform, MapsFromNext01Affine) {
    constexpr std::uint64_t seed = 2025ULL;
    constexpr double lo = -10.0, hi = 5.0;

    sampler ref(seed);
    const double u0 = ref.next01(); // primer valor de next01

    sampler s(seed);
    const double got = s.uniform(lo, hi);
    const double expected = lo + (hi - lo) * u0;
    EXPECT_NEAR(got, expected, kEps);
}

// -----------------------------------------------------------------------------
// jitter_half(): rango [-0.5, 0.5)
// -----------------------------------------------------------------------------
TEST(SamplerRanges, JitterHalfIsInMinusHalfToHalfHalfOpen) {
    sampler s(99ULL);
    for (int i = 0; i < 2000; ++i) {
        const double j = s.jitter_half();
        ExpectInRangeHalfOpen(j, -0.5, 0.5);
    }
}

// -----------------------------------------------------------------------------
// random_m11_vector(): componentes en [-1,1) y coherencia con 3 llamadas a next_m11()
// -----------------------------------------------------------------------------
TEST(SamplerVectors, RandomM11VectorMatchesThreeCalls) {
    constexpr std::uint64_t seed = 555ULL;
    sampler a(seed), b(seed);

    const vector v = a.random_m11_vector();

    const double x = b.next_m11();
    const double y = b.next_m11();
    const double z = b.next_m11();

    ExpectInRangeHalfOpen(v.x, -1.0, 1.0);
    ExpectInRangeHalfOpen(v.y, -1.0, 1.0);
    ExpectInRangeHalfOpen(v.z, -1.0, 1.0);

    EXPECT_DOUBLE_EQ(v.x, x);
    EXPECT_DOUBLE_EQ(v.y, y);
    EXPECT_DOUBLE_EQ(v.z, z);
}

// -----------------------------------------------------------------------------
// Sanity: sin NaN/Inf tras muchas llamadas
// -----------------------------------------------------------------------------
TEST(SamplerSanity, NoNaNOrInfAcrossManyCalls) {
    sampler s(3141592653ULL);
    for (int i = 0; i < 5000; ++i) {
        const double a = s.next01();
        const double b = s.next_signed01();
        const double c = s.next_m11();
        const double d = s.uniform(-100.0, 100.0);
        const double e = s.jitter_half();
        const vector  v = s.random_m11_vector();

        auto finite = [](double x){ return std::isfinite(x); };
        EXPECT_TRUE(finite(a));
        EXPECT_TRUE(finite(b));
        EXPECT_TRUE(finite(c));
        EXPECT_TRUE(finite(d));
        EXPECT_TRUE(finite(e));
        EXPECT_TRUE(finite(v.x) and finite(v.y) and finite(v.z));
    }
}

// utcommon/test_ppm_writer.cpp
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

#include "ppm_writer.hpp" // render::ppm::write_header, write, write_pixels_ascii, to_u8
#include "color.hpp"      // render::color

using render::color;
using render::ppm::image_size;
using render::ppm::writer_opts;

namespace { // helpers

// Construye una imagen 2x2 sencilla para verificar orden de escritura (fila a fila).
// Layout: (x,y)
// (0,0): (0,0,0)        (1,0): (1,0,0)
// (0,1): (0,1,0)        (1,1): (0,0,1)
struct FakeImage2x2 {
    [[nodiscard]] static int width()   noexcept { return 2; }
    [[nodiscard]] static int height()  noexcept { return 2; }

    [[nodiscard]] static color get_unchecked(int x, int y)  noexcept {
        if (x == 0 and y == 0) { return {0.0, 0.0, 0.0};
}
        if (x == 1 and y == 0) { return {1.0, 0.0, 0.0};
}
        if (x == 0 and y == 1) { return {0.0, 1.0, 0.0};
}
        return {0.0, 0.0, 1.0}; // (1,1)
    }
};

namespace {
std::string StripCarriageReturns(std::string s) {
    auto r = std::ranges::remove(s, '\r'); // r es un subrange [new_end, old_end)
    s.erase(r.begin(), r.end());           // borra desde el new_end hasta el final
    return s;
}
} // namespace

// ------------------------------ write_header ---------------------------------
TEST(PpmWriter, WriteHeader_EmitsP3SizeAndMaxval) {
    std::ostringstream os;
    const image_size sz{2, 3};
    render::ppm::write_header(os, sz);
    const std::string got = StripCarriageReturns(os.str());
    const std::string expected = "P3\n2 3\n255\n";
    EXPECT_EQ(got, expected);
}

// --------------------------------- to_u8 -------------------------------------
TEST(PpmWriter, ToU8_ClampsAndTruncatesAndAppliesGamma) {
    // gamma lineal (inv_gamma=1)
    const double inv1 = 1.0;
    EXPECT_EQ(render::ppm::to_u8(-1.0, inv1), 0);     // clamp abajo
    EXPECT_EQ(render::ppm::to_u8(0.0, inv1), 0);
    EXPECT_EQ(render::ppm::to_u8(1.0, inv1), 255);
    EXPECT_EQ(render::ppm::to_u8(2.0, inv1), 255);    // clamp arriba

    // Truncado (no redondeo): 10/255 + ε → 10
    EXPECT_EQ(render::ppm::to_u8(10.0/255.0 + 1e-6, inv1), 10);

    // Efecto gamma: inv_gamma = 1/2.2
    const double inv_gamma = 1.0 / 2.2;
    const double v = 0.25;
    const int expected = static_cast<int>(std::pow(std::clamp(v, 0.0, 1.0), inv_gamma) * 255.0);
    EXPECT_EQ(render::ppm::to_u8(v, inv_gamma), expected);
}

// ---------------------------- write_pixels_ascii -----------------------------
TEST(PpmWriter, WritePixelsAscii_WritesRowMajorTopDownLeftRight) {
    std::ostringstream os;
    const image_size sz{2, 2};
    const writer_opts opts{ .gamma = 1.0 }; // lineal para cuentas exactas

    auto getter = [](int x, int y) -> color {
        if (x == 0 and y == 0) { return {0.0, 0.0, 0.0};
}
        if (x == 1 and y == 0) { return {1.0, 0.0, 0.0};
}
        if (x == 0 and y == 1) { return {0.0, 1.0, 0.0};
}
        return {0.0, 0.0, 1.0};
    };

    render::ppm::write_pixels_ascii(os, sz, getter, opts);
    const std::string got = StripCarriageReturns(os.str());
    const std::string expected =
        "0 0 0\n"
        "255 0 0\n"
        "0 255 0\n"
        "0 0 255\n";
    EXPECT_EQ(got, expected);
}

TEST(PpmWriter, WritePixelsAscii_UsesDefaultInvGammaWhenGammaNonPositive) {
    std::ostringstream os;
    const image_size sz{1, 1};
    const writer_opts opts{ .gamma = 0.0 }; // fuerza 1/2.2 por defecto

    auto getter = [](int, int) -> color { return {0.5, 0.5, 0.5}; };
    render::ppm::write_pixels_ascii(os, sz, getter, opts);

    const int expected = static_cast<int>(std::pow(0.5, 1.0/2.2) * 255.0);
    const std::string exp =
        std::to_string(expected) + " " +
        std::to_string(expected) + " " +
        std::to_string(expected) + "\n";
    EXPECT_EQ(StripCarriageReturns(os.str()), exp);
}

// ------------------------------- write (Image) -------------------------------
TEST(PpmWriter, Write_ImageOverload_EmitsHeaderThenPixelsFromGetUnchecked) {
    std::ostringstream os;
    const FakeImage2x2 img{};
    const writer_opts opts{ .gamma = 1.0 };

    render::ppm::write(os, img, opts);
    const std::string got = StripCarriageReturns(os.str());
    const std::string expected =
        "P3\n2 2\n255\n"
        "0 0 0\n"
        "255 0 0\n"
        "0 255 0\n"
        "0 0 255\n";
    EXPECT_EQ(got, expected);
}

// ----------------------------- write (genérico) ------------------------------
TEST(PpmWriter, Write_GenericOverload_RespectsGetterAndGamma) {
    std::ostringstream os;
    const image_size sz{2, 1};
    const writer_opts opts{ .gamma = 2.0 }; // inv_gamma=0.5

    auto getter = [](int x, int) -> color {
        if (x == 0) { return {0.5, 0.5, 0.5};
}
        return {1.0, 1.0, 1.0};
    };

    render::ppm::write(os, sz, getter, opts);
    const std::string got = StripCarriageReturns(os.str());

    const int mid = static_cast<int>(std::pow(0.5, 0.5) * 255.0); // sqrt(0.5)*255 → truncado
    const std::string expected =
        "P3\n2 1\n255\n" +
        (std::to_string(mid) + " " + std::to_string(mid) + " " + std::to_string(mid) + "\n") +
        "255 255 255\n";

    EXPECT_EQ(got, expected);
}
}
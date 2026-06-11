// utsoa/test_image_soa.cpp
#include <gtest/gtest.h>

#include "image_soa.hpp"   // render::image_soa
#include "color.hpp"       // render::color

#include <cstddef>         // std::size_t
#include <stdexcept>       // std::invalid_argument, std::out_of_range
#include <type_traits>     // std::is_same_v
#include <utility>         // std::declval
#include <vector>          // std::vector

using render::image_soa;
using render::color;

namespace { // ----------------------- helpers ---------------------------------
void ExpectColorNear(const color& a, const color& b, double eps = 1e-12) {
    EXPECT_NEAR(a.r, b.r, eps);
    EXPECT_NEAR(a.g, b.g, eps);
    EXPECT_NEAR(a.b, b.b, eps);
}
} // namespace

// ---------------------------- static checks ---------------------------------
static_assert(noexcept(std::declval<const image_soa&>().width()));
static_assert(noexcept(std::declval<const image_soa&>().height()));
static_assert(noexcept(std::declval<const image_soa&>().size()));
static_assert(noexcept(std::declval<const image_soa&>().get_unchecked(0,0)));
static_assert(noexcept(std::declval<image_soa&>().set_unchecked(0,0, std::declval<const color&>())));

// Tipos devueltos por los accesores de canal
static_assert(std::is_same_v<decltype(std::declval<image_soa&>().r_channel()), std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<image_soa&>().g_channel()), std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<image_soa&>().g_channel()), std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<image_soa&>().b_channel()), std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<const image_soa&>().r_channel()), const std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<const image_soa&>().g_channel()), const std::vector<double>&>);
static_assert(std::is_same_v<decltype(std::declval<const image_soa&>().b_channel()), const std::vector<double>&>);

// --------------------------------- ctor -------------------------------------

TEST(ImageSoa, ConstructorRejectsInvalidSizes) {
    EXPECT_THROW((image_soa(0, 1)),  std::invalid_argument);
    EXPECT_THROW((image_soa(1, 0)),  std::invalid_argument);
    EXPECT_THROW((image_soa(-1, 2)), std::invalid_argument); // cobertura negativos
    EXPECT_THROW((image_soa(2, -1)), std::invalid_argument);
}

TEST(ImageSoa, ConstructorAllocatesChannelsWithCorrectSize) {
    const image_soa img(3, 2);
    EXPECT_EQ(img.width(), 3);
    EXPECT_EQ(img.height(), 2);
    EXPECT_EQ(img.size(), static_cast<std::size_t>(3 * 2));

    const auto& R = img.r_channel();
    const auto& G = img.g_channel();
    const auto& B = img.b_channel();
    EXPECT_EQ(R.size(), img.size());
    EXPECT_EQ(G.size(), img.size());
    EXPECT_EQ(B.size(), img.size());
}

// ------------------------------- set/get ------------------------------------

TEST(ImageSoa, SetGetWithinBoundsAffectOnlyOnePixel) {
    image_soa img(3, 2);
    const color red{1.0, 0.0, 0.0};
    const color green{0.0, 1.0, 0.0};

    // Inicializa todo a verde
    img.clear(green);

    // Cambia un único píxel
    img.set(1, 1, red);

    // Verifica píxel cambiado
    ExpectColorNear(img.get(1, 1), red);

    // Verifica que los demás no se alteran
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            if (x == 1 and y == 1) { continue; }
            ExpectColorNear(img.get(x, y), green);
        }
    }
}

TEST(ImageSoa, SetGetThrowOnOutOfRange) {
    image_soa img(2, 2);
    const color c{0.1, 0.2, 0.3};

    // set out-of-range
    EXPECT_THROW(img.set(-1, 0, c), std::out_of_range);
    EXPECT_THROW(img.set( 2, 0, c), std::out_of_range);
    EXPECT_THROW(img.set( 0,-1, c), std::out_of_range);
    EXPECT_THROW(img.set( 0, 2, c), std::out_of_range);

    // get out-of-range
    EXPECT_THROW((void)img.get(-1, 0), std::out_of_range);
    EXPECT_THROW((void)img.get( 2, 0), std::out_of_range);
    EXPECT_THROW((void)img.get( 0,-1), std::out_of_range);
    EXPECT_THROW((void)img.get( 0, 2), std::out_of_range);
}

// ----------------------- unchecked set/get (PRE: in-range) ------------------

TEST(ImageSoa, SetUncheckedGetUncheckedRoundTrip) {
    image_soa img(3, 2);
    const color c1{0.4, 0.5, 0.6};
    const color c2{0.9, 0.1, 0.2};

    img.set_unchecked(0, 0, c1);
    img.set_unchecked(2, 1, c2);

    ExpectColorNear(img.get_unchecked(0, 0), c1);
    ExpectColorNear(img.get_unchecked(2, 1), c2);
}

// -------------------------------- clear -------------------------------------

TEST(ImageSoa, ClearFillsAllChannels) {
    image_soa img(4, 3);
    const color base{0.25, 0.5, 1.0};
    img.clear(base);

    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            ExpectColorNear(img.get(x, y), base);
        }
    }
}

// ---------------------------- channel accessors -----------------------------

TEST(ImageSoa, NonConstChannelAccessAllowsMutation) {
    image_soa img(3, 2);
    img.clear(color{0.0, 0.0, 0.0});

    auto& R = img.r_channel();
    auto& G = img.g_channel();
    auto& B = img.b_channel();

    ASSERT_EQ(R.size(), img.size());
    ASSERT_EQ(G.size(), img.size());
    ASSERT_EQ(B.size(), img.size());

    // Índice (x=2,y=1) ⇒ idx = 1*3 + 2 = 5
    const std::size_t idx = static_cast<std::size_t>(1) * 3U + 2U;
    R[idx] = 0.1;
    G[idx] = 0.2;
    B[idx] = 0.3;

    ExpectColorNear(img.get_unchecked(2, 1), color{0.1, 0.2, 0.3});
}

TEST(ImageSoa, ConstChannelAccessSharesStorage) {
    image_soa img(2, 1);
    img.set_unchecked(1, 0, color{0.7, 0.8, 0.9});

    const auto& Rc = static_cast<const image_soa&>(img).r_channel();
    const auto& Gc = static_cast<const image_soa&>(img).g_channel();
    const auto& Bc = static_cast<const image_soa&>(img).b_channel();

    ASSERT_EQ(Rc.size(), img.size());
    ASSERT_EQ(Gc.size(), img.size());
    ASSERT_EQ(Bc.size(), img.size());

    // Coherencia con get_unchecked
    ExpectColorNear(img.get_unchecked(1, 0), color{Rc[1], Gc[1], Bc[1]});
}

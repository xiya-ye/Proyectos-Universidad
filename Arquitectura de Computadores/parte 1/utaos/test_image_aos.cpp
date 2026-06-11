// utaos/test_image_aos.cpp
#include <gtest/gtest.h>

#include <type_traits>  // std::is_same_v
#include <vector>       // std::vector
#include <cstddef>      // std::size_t
#include <stdexcept>    // std::invalid_argument, std::out_of_range
#include <utility>      // std::declval

#include "image_aos.hpp"
#include "color.hpp"    // render::color

using render::image_aos;
using render::color;

namespace { // helpers

void ExpectColorNear(const color& a, const color& b, double eps = 1e-12) {
    EXPECT_NEAR(a.r, b.r, eps);
    EXPECT_NEAR(a.g, b.g, eps);
    EXPECT_NEAR(a.b, b.b, eps);
}

} // namespace

TEST(ImageAos, ConstructValidSizeInitializes) {
    image_aos img(3, 2);
    EXPECT_EQ(img.width(), 3);
    EXPECT_EQ(img.height(), 2);
    EXPECT_EQ(img.size(), static_cast<std::size_t>(3 * 2));

    const auto& px = img.pixels();
    ASSERT_EQ(px.size(), img.size());
    ExpectColorNear(px.front(), color{});
    ExpectColorNear(px.back(),  color{});
}

TEST(ImageAos, ConstructInvalidWidthThrows) {
    EXPECT_THROW(image_aos(-1, 2), std::invalid_argument);
    EXPECT_THROW(image_aos( 0, 2), std::invalid_argument);
}

TEST(ImageAos, ConstructInvalidHeightThrows) {
    EXPECT_THROW(image_aos(2, -1), std::invalid_argument);
    EXPECT_THROW(image_aos(2,  0), std::invalid_argument);
}

TEST(ImageAos, SetGetInRangeWorks) {
    image_aos img(4, 3);
    const color c{0.1, 0.2, 0.3};
    img.set(1, 2, c);
    auto got = img.get(1, 2);
    ExpectColorNear(got, c);
}

TEST(ImageAos, SetGetOutOfRangeThrow) {
    image_aos img(2, 2);
    const color c{1, 1, 1};
    // x < 0, x >= w
    EXPECT_THROW(img.set(-1, 0, c), std::out_of_range);
    EXPECT_THROW((void)img.get(-1, 0),    std::out_of_range);
    EXPECT_THROW(img.set( 2, 0, c), std::out_of_range);
    EXPECT_THROW((void)img.get( 2, 0),    std::out_of_range);
    // y < 0, y >= h
    EXPECT_THROW(img.set(0, -1, c), std::out_of_range);
    EXPECT_THROW((void)img.get(0, -1),    std::out_of_range);
    EXPECT_THROW(img.set(0,  2, c), std::out_of_range);
    EXPECT_THROW((void)img.get(0,  2),    std::out_of_range);
}

TEST(ImageAos, SetUncheckedAndGetUncheckedWorkWithinRange) {
    image_aos img(3, 3);
    const color c{0.25, 0.5, 0.75};
    ASSERT_NO_THROW(img.set_unchecked(2, 1, c));
    auto got = img.get_unchecked(2, 1);
    ExpectColorNear(got, c);
}

TEST(ImageAos, RowMajorLayoutMatchesSpec) {
    const int w = 3, h = 2;
    image_aos img(w, h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const int idx = y * w + x;
            img.set_unchecked(x, y, color{double(idx), double(idx + 0.1), double(idx + 0.2)});
        }
    }

    const auto& px = img.pixels();
    ASSERT_EQ(px.size(), static_cast<std::size_t>(w * h));

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const int idx = y * w + x;
            const color expected{double(idx), double(idx + 0.1), double(idx + 0.2)};
            ExpectColorNear(px[static_cast<std::size_t>(idx)], expected);
            ExpectColorNear(img.get_unchecked(x, y), expected);
        }
    }
}

TEST(ImageAos, ClearFillsWithColor) {
    image_aos img(4, 2);
    const color fill{0.9, 0.8, 0.7};
    img.clear(fill);

    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            ExpectColorNear(img.get_unchecked(x, y), fill);
        }
    }
}

TEST(ImageAos, PixelsWriteThroughAndConstOverloadTypes) {
    image_aos img(2, 1);
    const color c{0.4, 0.5, 0.6};
    {
        auto& v = img.pixels();           // no-const
        ASSERT_EQ(v.size(), img.size());
        v[0] = c;                         // escritura directa
    }
    ExpectColorNear(img.get_unchecked(0, 0), c);

    static_assert(std::is_same_v<decltype(std::declval<image_aos&>().pixels()),
                                 std::vector<color>&>,
                  "pixels() no-const debe devolver std::vector<color>&");
    static_assert(std::is_same_v<decltype(std::declval<const image_aos&>().pixels()),
                                 const std::vector<color>&>,
                  "pixels() const debe devolver const std::vector<color>&");
}

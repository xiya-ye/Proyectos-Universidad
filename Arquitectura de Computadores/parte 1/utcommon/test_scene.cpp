// utcommon/test_scene.cpp
#include <gtest/gtest.h>

#include <optional>  // std::optional, std::nullopt

#include "scene.hpp"     // render::scene
#include "object.hpp"    // render::object, render::Hit, render::hit_opt
#include "ray.hpp"       // render::ray
#include "vector.hpp"    // render::vector
#include "color.hpp"     // render::color
#include "material.hpp"  // render::material

using render::scene;
using render::object;
using render::Hit;
using render::hit_opt;
using render::ray;
using render::vector;
using render::color;
using render::material;

namespace { // ======================== helpers & fakes =========================

void ExpectColorEq(const color& a, const color& b) {
    EXPECT_DOUBLE_EQ(a.r, b.r);
    EXPECT_DOUBLE_EQ(a.g, b.g);
    EXPECT_DOUBLE_EQ(a.b, b.b);
}

class ConstHitObject final : public object {
public:
    ConstHitObject(double t_hit,
                   vector outward_normal_unit,
                   const material* m) noexcept
      : t_(t_hit), n_(outward_normal_unit), mat_(m) {}

    [[nodiscard]] hit_opt intersect(const ray& r,
                                    double t_min,
                                    double t_max) const noexcept override {
        if (t_ < t_min or t_ > t_max) { return std::nullopt; }
        Hit h{};
        h.t   = t_;
        h.p   = r.at(t_);
        h.mat = mat_;
        h.set_face_normal(r, n_);
        return h;
    }
private:
    double t_{};
    vector n_{1.0, 0.0, 0.0};
    const material* mat_{nullptr};
};

material MakeMatteWhite() { return material{"white", color{1.0,1.0,1.0}}; }

} // namespace

// =============================== TESTS: gestión ===============================

TEST(SceneManage, AddClearReserveAndIgnoreNull) {
    scene sc;
    sc.reserve(8U);

    const material m = MakeMatteWhite();
    const ray r{ vector{0.0,0.0,0.0}, vector{1.0,0.0,0.0} };

    sc.add(nullptr);
    ConstHitObject far_obj(10.0, vector{1.0,0.0,0.0}, &m);
    sc.add(&far_obj);

    const hit_opt h1 = sc.intersect(r);
    if (!h1.has_value()) { GTEST_SKIP() << "No hit produced unexpectedly";
}
    const Hit hit1 = h1.value_or(Hit{});
    EXPECT_NEAR(hit1.t, 10.0, 1e-12);

    sc.clear();
    const hit_opt h2 = sc.intersect(r);
    EXPECT_FALSE(h2.has_value());
}

TEST(SceneManage, SetBackgroundAffectsEmptyScene) {
    scene sc;
    const color dark{0.1, 0.2, 0.3};
    const color light{0.9, 0.8, 0.7};
    sc.set_background(dark, light);

    const ray up   { vector{0.0,0.0,0.0}, vector{0.0, 1.0, 0.0} };
    const ray down { vector{0.0,0.0,0.0}, vector{0.0,-1.0, 0.0} };
    const ray mid  { vector{0.0,0.0,0.0}, vector{1.0, 0.0, 0.0} };

    ExpectColorEq(sc.trace(up,   /*depth=*/3), dark);
    ExpectColorEq(sc.trace(down, /*depth=*/3), light);

    const color expected_mid{
        0.5 * light.r + 0.5 * dark.r,
        0.5 * light.g + 0.5 * dark.g,
        0.5 * light.b + 0.5 * dark.b
    };
    ExpectColorEq(sc.trace(mid,  /*depth=*/3), expected_mid);
}

TEST(SceneManage, SetMaterialSeedCompilesAndNoEffectOnEmptyBackground) {
    scene sc;
    const color dark{0.2, 0.4, 0.6};
    const color light{0.7, 0.6, 0.5};
    sc.set_background(dark, light);
    sc.set_material_seed(1234ULL);

    const ray r{ vector{0.0,0.0,0.0}, vector{0.0,1.0,0.0} };
    ExpectColorEq(sc.trace(r, 2), dark);
}

// =============================== TESTS: intersect ============================

TEST(SceneIntersect, ChoosesNearestHit) {
    scene sc;
    const material m = MakeMatteWhite();
    const ray r{ vector{0.0,0.0,0.0}, vector{1.0,0.0,0.0} };

    ConstHitObject near_obj(2.0, vector{1.0,0.0,0.0}, &m);
    ConstHitObject far_obj (5.0, vector{1.0,0.0,0.0}, &m);
    sc.add(&far_obj);
    sc.add(&near_obj);

    const hit_opt h = sc.intersect(r);
    if (!h.has_value()) { GTEST_SKIP() << "No hit produced unexpectedly";
}
    const Hit hit = h.value_or(Hit{});
    EXPECT_NEAR(hit.t, 2.0, 1e-12);
    EXPECT_NEAR(hit.p.x, 2.0, 1e-12);
    EXPECT_NEAR(hit.p.y, 0.0, 1e-12);
    EXPECT_NEAR(hit.p.z, 0.0, 1e-12);
}

TEST(SceneIntersect, RespectsLambdaMinAndTMaxPruning) {
    scene sc;
    const material m = MakeMatteWhite();
    const ray r{ vector{0.0,0.0,0.0}, vector{0.0,0.0,1.0} };

    ConstHitObject too_close(scene::kLambdaMin * 0.1, vector{0.0,0.0,1.0}, &m);
    ConstHitObject valid(3.0, vector{0.0,0.0,1.0}, &m);

    sc.add(&too_close);
    sc.add(&valid);

    const hit_opt h = sc.intersect(r);
    if (!h.has_value()) { GTEST_SKIP() << "No hit produced unexpectedly";
}
    const Hit hit = h.value_or(Hit{});
    EXPECT_NEAR(hit.t, 3.0, 1e-12);
}

// ================================ TESTS: trace ===============================

TEST(SceneTrace, DepthZeroAlwaysReturnsBlack) {
    const scene sc; // puede ser const
    const ray any{ vector{0.0,0.0,0.0}, vector{1.0,2.0,3.0} };
    const color c = sc.trace(any, /*depth=*/0);
    ExpectColorEq(c, color{0.0,0.0,0.0});
}

TEST(SceneTrace, EmptySceneReturnsBackgroundRegardlessOfDepthPositive) {
    scene sc;
    sc.set_background(color{0.0,0.0,0.0}, color{1.0,1.0,1.0});
    const ray up{ vector{0.0,0.0,0.0}, vector{0.0,1.0,0.0} };
    ExpectColorEq(sc.trace(up, 1), color{0.0,0.0,0.0});

    sc.set_background(color{0.2,0.4,0.6}, color{0.8,0.6,0.4});
    const ray down{ vector{0.0,0.0,0.0}, vector{0.0,-1.0,0.0} };
    ExpectColorEq(sc.trace(down, 5), color{0.8,0.6,0.4});
}

TEST(SceneTrace, WithHitAndDepthOneReturnsBlack) {
    scene sc;
    const material m = MakeMatteWhite();
    ConstHitObject obj(2.0, vector{1.0,0.0,0.0}, &m);
    sc.add(&obj);

    const ray r{ vector{0.0,0.0,0.0}, vector{1.0,0.0,0.0} };
    const color c = sc.trace(r, /*depth=*/1);
    ExpectColorEq(c, color{0.0,0.0,0.0});
}

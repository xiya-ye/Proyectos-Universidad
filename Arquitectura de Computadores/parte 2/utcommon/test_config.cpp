// utcommon/test_config.cpp
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "config.hpp"
#include "camera.hpp"
#include "vector.hpp"
#include "color.hpp"

namespace fs = std::filesystem;
using render::render_config;
using render::camera_pose;
using render::camera_optics;

namespace {

template <class F>
void ExpectThrowsRuntimeErrorContaining(F&& fn, std::string_view needle) {
    try { std::forward<F>(fn)(); FAIL() << "Se esperaba std::runtime_error"; }
    catch (const std::runtime_error& e) {
        const std::string msg = e.what();
        EXPECT_NE(msg.find(std::string(needle)), std::string::npos)
            << "Mensaje no contiene '" << needle << "'. Msg: " << msg;
    } catch (...) { FAIL() << "Excepción distinta de std::runtime_error"; }
}

void WriteTextFile(const fs::path& p, std::string_view text) {
    fs::create_directories(p.parent_path());
    std::ofstream ofs(p);
    ASSERT_TRUE(ofs.good());
    ofs << text;
    ofs.flush();
    ASSERT_TRUE(ofs.good());
}

// ======= CONFIG MÍNIMA VÁLIDA USADA EN LOS ASSERTS DE ESTE ARCHIVO =======
std::string MakeMinimalValidCfg() {
    return
        "aspect_ratio: 1 1\n"
        "image_width:  2\n"                  // <-- 2 (no 3)
        "gamma: 2.2\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"                  // <-- 1 (no 2)
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"    // <-- negro
        "background_light_color: 1 1 1\n";   // <-- blanco
}

} // namespace

// ----------------------------- RUTA “FELIZ” ----------------------------------

TEST(Config, ParseFromIstreamHappyPathAndDerivedHeight) {
    std::istringstream iss(MakeMinimalValidCfg());
    const render_config cfg = render_config::parse(iss);

    EXPECT_EQ(cfg.image_width, 2);
    EXPECT_EQ(cfg.aspect_x, 1);
    EXPECT_EQ(cfg.aspect_y, 1);
    EXPECT_EQ(cfg.image_height(), 2); // floor(2*1/1) = 2

    EXPECT_DOUBLE_EQ(cfg.gamma, 2.2);
    EXPECT_DOUBLE_EQ(cfg.field_of_view, 60.0);
    EXPECT_EQ(cfg.samples_per_pixel, 1);
    EXPECT_EQ(cfg.max_depth, 1);
    EXPECT_EQ(cfg.material_rng_seed, 1U);
    EXPECT_EQ(cfg.ray_rng_seed, 1U);          // ajustado

    // Colores y cámara (ajustados a la config mínima)
    EXPECT_DOUBLE_EQ(cfg.background_dark.r, 0.0);
    EXPECT_DOUBLE_EQ(cfg.background_dark.g, 0.0);
    EXPECT_DOUBLE_EQ(cfg.background_dark.b, 0.0);
    EXPECT_DOUBLE_EQ(cfg.background_light.r, 1.0);
    EXPECT_DOUBLE_EQ(cfg.background_light.g, 1.0);
    EXPECT_DOUBLE_EQ(cfg.background_light.b, 1.0);

    EXPECT_DOUBLE_EQ(cfg.camera_position.x, 0.0);
    EXPECT_DOUBLE_EQ(cfg.camera_target.z,  -1.0);
    EXPECT_DOUBLE_EQ(cfg.camera_north.y,    1.0);
}

TEST(Config, HeightUsesFloorNotRound) {
    std::istringstream iss(
        "aspect_ratio: 16 9\n"
        "image_width:  3\n"
        "gamma: 2.2\n"
        "field_of_view: 90\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 2\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    const render_config cfg = render_config::parse(iss);
    EXPECT_EQ(cfg.image_width, 3);
    EXPECT_EQ(cfg.aspect_x, 16);
    EXPECT_EQ(cfg.aspect_y, 9);
    EXPECT_EQ(cfg.image_height(), 1); // floor(3*9/16)
}

TEST(Config, LastValueWinsWhenRepeated) {
    std::istringstream iss(
        "image_width: 100\n"
        "image_width: 200\n"
        "samples_per_pixel: 4\n"
        "samples_per_pixel: 1\n"
        "gamma: 2.4\n"
        "gamma: 2.2\n"
        "aspect_ratio: 1 1\n"
        "field_of_view: 60\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 2\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    const render_config cfg = render_config::parse(iss);
    EXPECT_EQ(cfg.image_width, 200);
    EXPECT_EQ(cfg.samples_per_pixel, 1);
    EXPECT_DOUBLE_EQ(cfg.gamma, 2.2);
}

// ------------------------ CONVERSIÓN A TIPOS DE CÁMARA -----------------------

TEST(Config, ToCameraPoseAndOptics) {
    std::istringstream iss(MakeMinimalValidCfg());
    const render_config cfg = render_config::parse(iss);

    const camera_pose pose = cfg.to_camera_pose();
    EXPECT_DOUBLE_EQ(pose.origin.x, 0.0);
    EXPECT_DOUBLE_EQ(pose.look_at.z, -1.0);
    EXPECT_DOUBLE_EQ(pose.up.y, 1.0);

    const camera_optics opt = cfg.to_camera_optics();
    EXPECT_DOUBLE_EQ(opt.fov_deg, 60.0);
    EXPECT_EQ(opt.width, 2);                 // ajustado a la config mínima
    EXPECT_EQ(opt.height, cfg.image_height());
}

// ------------------------------- I/O POR RUTA --------------------------------

TEST(Config, ConstructFromPathReadsFile) {
    const fs::path tmp = fs::temp_directory_path() / "cfg_ok" / "cfg.txt";
    WriteTextFile(tmp, MakeMinimalValidCfg());
    ASSERT_NO_THROW(render_config(tmp.string()));
}

TEST(Config, ConstructFromPathMissingFileThrows) {
    const fs::path tmp = fs::temp_directory_path() / "cfg_missing_dir" / "nope.txt";
    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_config(tmp.string()); },
        "cannot open configuration file"
    );
}

// ------------------------------- ERRORES VARIOS -------------------------------

TEST(Config, UnknownKeyNoColon) {
    std::istringstream iss(
        "banana 123\n"
        "aspect_ratio: 1 1\n"
        "image_width:  2\n"
        "gamma: 2.2\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    ExpectThrowsRuntimeErrorContaining(
        [&]{ (void)render_config::parse(iss); },
        "Unknown configuration key: [banana:]"
    );
}

TEST(Config, UnknownKeyWithColon) {
    std::istringstream iss(
        "foo: 1\n"
        "aspect_ratio: 1 1\n"
        "image_width:  2\n"
        "gamma: 2.2\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    ExpectThrowsRuntimeErrorContaining(
        [&]{ (void)render_config::parse(iss); },
        "Unknown configuration key: [foo:]"
    );
}

TEST(Config, ExtraDataAfterValue) {
    std::istringstream iss(
        "image_width: 3 EXTRA\n"
        "aspect_ratio: 1 1\n"
        "gamma: 2.2\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    ExpectThrowsRuntimeErrorContaining(
        [&]{ (void)render_config::parse(iss); },
        "Extra data after configuration value for key: [image_width:]"
    );
}

TEST(Config, InvalidValueWrongTypeOrRange) {
    // (dejamos los cuerpos exactamente como los tenías; no dependen de la cfg mínima)
    {
        std::istringstream iss(
            "image_width: x\n"
            "aspect_ratio: 1 1\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [image_width:]"
        );
    }
    {
        std::istringstream iss(
            "gamma: 0\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [gamma:]"
        );
    }
    {
        std::istringstream iss(
            "field_of_view: 180\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [field_of_view:]"
        );
    }
    {
        std::istringstream iss(
            "samples_per_pixel: 0\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [samples_per_pixel:]"
        );
    }
    {
        std::istringstream iss(
            "max_depth: 0\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [max_depth:]"
        );
    }
    {
        std::istringstream iss(
            "material_rng_seed: 0\n"
            "ray_rng_seed: 1\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [material_rng_seed:]"
        );
    }
    {
        std::istringstream iss(
            "ray_rng_seed: 0\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_dark_color:  0 0 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [ray_rng_seed:]"
        );
    }
    {
        std::istringstream iss(
            "background_dark_color:  1.1 0 0\n"
            "aspect_ratio: 1 1\n"
            "image_width: 2\n"
            "gamma: 2.2\n"
            "field_of_view: 60\n"
            "samples_per_pixel: 1\n"
            "max_depth: 1\n"
            "material_rng_seed: 1\n"
            "ray_rng_seed: 1\n"
            "camera_position: 0 0 0\n"
            "camera_target:   0 0 -1\n"
            "camera_north:    0 1 0\n"
            "background_light_color: 1 1 1\n"
        );
        ExpectThrowsRuntimeErrorContaining(
            [&]{ (void)render_config::parse(iss); },
            "Invalid value for key: [background_dark_color:]"
        );
    }
}

TEST(Config, MissingValueAfterColonIsInvalid) {
    std::istringstream iss(
        "gamma:\n"
        "aspect_ratio: 1 1\n"
        "image_width: 2\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "background_dark_color:  0 0 0\n"
        "background_light_color: 1 1 1\n"
    );
    ExpectThrowsRuntimeErrorContaining(
        [&]{ (void)render_config::parse(iss); },
        "Invalid value for key: [gamma:]"
    );
}

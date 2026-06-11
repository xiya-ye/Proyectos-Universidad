// utaos/test_render_aos.cpp
#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <stdexcept>   // std::runtime_error
#include <utility>     // std::forward

#include "render_aos.hpp"

namespace fs = std::filesystem;
using render::render_aos;

namespace { // helpers & utilidades para pruebas

// Escribe texto en ruta (crea directorio si hace falta).
void WriteTextFile(const fs::path& p, std::string_view text) {
    fs::create_directories(p.parent_path());
    std::ofstream ofs(p);
    ASSERT_TRUE(ofs.good()) << "No se pudo abrir para escritura: " << p.string();
    ofs << text;
    ofs.flush();
    ASSERT_TRUE(ofs.good()) << "Fallo escribiendo: " << p.string();
}

// Config válido mínimo
std::string MakeValidConfigText() {
    return
        "aspect_ratio: 1 1\n"
        "image_width: 2\n"
        "gamma: 2.2\n"
        "camera_position: 0 0 0\n"
        "camera_target:   0 0 -1\n"
        "camera_north:    0 1 0\n"
        "field_of_view: 60\n"
        "samples_per_pixel: 1\n"
        "max_depth: 1\n"
        "material_rng_seed: 1\n"
        "ray_rng_seed: 1\n"
        "background_dark_color: 0 0 0\n"
        "background_light_color: 1 1 1\n";
}

// Escena mínima correcta: un material y una esfera.
std::string MakeValidSceneWithSphere() {
    return
        "# materiales\n"
        "matte: red 1 0 0\n"
        "# objetos\n"
        "sphere: 0 0 -1 0.5 red\n";
}

// Lee encabezado PPM P3.
struct PpmHeader { std::string magic; int w{0}; int h{0}; int maxv{0}; };
std::optional<PpmHeader> ReadPpmHeader(const fs::path& p) {
    std::ifstream ifs(p);
    if (!ifs.good()) { return std::nullopt;
}
    PpmHeader h{};
    ifs >> h.magic >> h.w >> h.h >> h.maxv;
    if (!ifs.good()) { return std::nullopt;
}
    return h;
}

// Verifica que lanzar y que el mensaje contenga cierto substring.
template <class F>
void ExpectThrowsRuntimeErrorContaining(F&& fn, std::string_view needle) {
    try {
        std::forward<F>(fn)();  // perfect forwarding (clang-tidy)
        FAIL() << "Se esperaba std::runtime_error";
    } catch (const std::runtime_error& e) {
        const std::string msg = e.what(); // const-correctness
        EXPECT_NE(msg.find(std::string(needle)), std::string::npos)
            << "Mensaje no contiene '" << needle << "'. Mensaje: " << msg;
    } catch (...) {
        FAIL() << "Se lanzó excepción distinta de std::runtime_error";
    }
}

fs::path MakeTempDir(const std::string& name_hint) {
    fs::path base = fs::temp_directory_path() / ("raos_" + name_hint);
    fs::create_directories(base);
    return base;
}

} // namespace

// ------------------------- CASOS "OK" (ruta feliz) ---------------------------

TEST(RenderAos, RendersSmallSceneAndWritesPpmHeader) {
    const auto tmp = MakeTempDir("ok_small");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";

    WriteTextFile(cfg, MakeValidConfigText());
    WriteTextFile(scn, MakeValidSceneWithSphere());

    ASSERT_NO_THROW(render_aos(cfg.string(), scn.string(), out.string()));

    const auto hdr_opt = ReadPpmHeader(out);
    if (!hdr_opt) {           // <-- guardia explícita para el analyzer
        FAIL() << "No se pudo leer PPM de salida";
        return;               // <-- evita acceso posterior
    }
    const PpmHeader& hdr = *hdr_opt;   // <-- ya seguro
    EXPECT_EQ(hdr.magic, "P3");
    EXPECT_EQ(hdr.w, 2);
    EXPECT_EQ(hdr.h, 2);
    EXPECT_EQ(hdr.maxv, 255);
}

TEST(RenderAos, EmptySceneFileStillRendersBackground) {
    const auto tmp = MakeTempDir("ok_empty_scene");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";

    WriteTextFile(cfg, MakeValidConfigText());
    WriteTextFile(scn, "# comentario\n   \n// otro comentario\n");

    ASSERT_NO_THROW(render_aos(cfg.string(), scn.string(), out.string()));

    const auto hdr_opt = ReadPpmHeader(out);
    if (!hdr_opt) {                           // <-- guardia explícita
        FAIL() << "No se pudo leer PPM de salida (escena vacía)";
        return;
    }
    const PpmHeader& hdr = *hdr_opt;           // <-- ya seguro
    EXPECT_EQ(hdr.magic, "P3");
    EXPECT_EQ(hdr.w, 2);
    EXPECT_EQ(hdr.h, 2);
    EXPECT_EQ(hdr.maxv, 255);
}

// --------------------------- ERRORES DE E/S ----------------------------------

TEST(RenderAos, SceneFileNotFoundThrows) {
    const auto tmp = MakeTempDir("err_scene_missing");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), (tmp / "no_existe_scene.txt").string(), out.string()); },
        "Could not open scene file"
    );
}

TEST(RenderAos, OutputPathIsDirectoryThrows) {
    const auto tmp = MakeTempDir("err_output_dir");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out_dir = tmp / "out_dir";
    fs::create_directories(out_dir);

    WriteTextFile(cfg, MakeValidConfigText());
    WriteTextFile(scn, MakeValidSceneWithSphere());

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out_dir.string()); },
        "Could not open output file"
    );
}

// ------------------------ ERRORES DE PARSING/ESCENA --------------------------

TEST(RenderAos, UnknownEntityTagThrows) {
    const auto tmp = MakeTempDir("err_unknown_tag");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";

    WriteTextFile(cfg, MakeValidConfigText());
    WriteTextFile(scn, "banana: 1 2 3\n");

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Unknown scene entity: banana:"
    );
}

TEST(RenderAos, DuplicateMaterialNameThrows) {
    const auto tmp = MakeTempDir("err_dup_mat");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";

    WriteTextFile(cfg, MakeValidConfigText());
    WriteTextFile(scn,
        "matte: m 1 0 0\n"
        "matte: m 0 1 0\n");

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "already exists"
    );
}

TEST(RenderAos, MatteInvalidParamsThrows) {
    const auto tmp = MakeTempDir("err_matte_params");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn, "matte: c 0.1 0.2\n"); // falta 'b'

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid matte material parameters"
    );
}

TEST(RenderAos, MetalInvalidParamsThrows) {
    const auto tmp = MakeTempDir("err_metal_params");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn, "metal: m 0.1 0.2 0.3\n"); // falta fuzz

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid metal material parameters"
    );
}

TEST(RenderAos, RefractiveInvalidParamsThrows) {
    const auto tmp = MakeTempDir("err_refr_params");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn, "refractive: glass\n"); // falta ior

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid refractive material parameters"
    );
}

TEST(RenderAos, ExtraTokensAfterMatteThrow) {
    const auto tmp = MakeTempDir("err_extra_tokens");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn, "matte: m 0.1 0.2 0.3 EXTRA\n");

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid matte"
    );
}

TEST(RenderAos, SphereInvalidRadiusThrows) {
    const auto tmp = MakeTempDir("err_sphere_r");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn,
        "matte: m 1 0 0\n"
        "sphere: 0 0 -1 0 m\n"); // r <= 0

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid sphere parameters"
    );
}

TEST(RenderAos, SphereMaterialNotFoundThrows) {
    const auto tmp = MakeTempDir("err_sphere_mat_missing");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn, "sphere: 0 0 -1 0.5 noSuchMat\n");

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Material not found"
    );
}

TEST(RenderAos, CylinderInvalidAxisThrows) {
    const auto tmp = MakeTempDir("err_cyl_axis");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn,
        "matte: m 1 1 1\n"
        "cylinder: 0 0 0 0.5 0 0 0 m\n"); // eje nulo

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid cylinder parameters"
    );
}

TEST(RenderAos, CylinderInvalidRadiusThrows) {
    const auto tmp = MakeTempDir("err_cyl_radius");
    const fs::path cfg = tmp / "cfg.txt";
    const fs::path scn = tmp / "scene.txt";
    const fs::path out = tmp / "out.ppm";
    WriteTextFile(cfg, MakeValidConfigText());

    WriteTextFile(scn,
        "matte: m 1 1 1\n"
        "cylinder: 0 0 0 0 1 0 0 m\n"); // r <= 0

    ExpectThrowsRuntimeErrorContaining(
        [&]{ render_aos(cfg.string(), scn.string(), out.string()); },
        "Invalid cylinder parameters"
    );
}

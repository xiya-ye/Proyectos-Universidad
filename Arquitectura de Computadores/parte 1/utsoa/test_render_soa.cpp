// utsoa/test_render_soa.cpp
// Pruebas de integración para render_soa (SOA).
// Verifica cabecera PPM, número de líneas RGB y errores típicos de escena.

#include <gtest/gtest.h>

#include "render_soa.hpp"

#include <chrono>      // std::chrono::high_resolution_clock
#include <filesystem>  // std::filesystem
#include <fstream>     // std::ifstream, std::ofstream
#include <ios>         // std::ios::binary
#include <random>      // std::mt19937_64, std::uniform_int_distribution
#include <regex>       // std::regex, std::regex_match
#include <sstream>     // std::ostringstream, std::istringstream
#include <stdexcept>   // std::runtime_error
#include <string>      // std::string
#include <utility>     // std::pair

using render::render_soa;

namespace {

namespace fs = std::filesystem;

std::string RandSuffixHex() {
  static std::mt19937_64 rng{
      static_cast<std::mt19937_64::result_type>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count())
  };
  std::uniform_int_distribution<unsigned long long> dist;
  std::ostringstream oss;
  oss << std::hex << dist(rng);
  return oss.str();
}

fs::path WriteTempFile(const std::string& base,
                       const std::string& ext,
                       const std::string& content) {
  const fs::path p = fs::temp_directory_path() / fs::path(base + "-" + RandSuffixHex() + ext);
  std::ofstream ofs(p, std::ios::binary);
  if (!ofs) {
    throw std::runtime_error("cannot create temp file: " + p.string());
  }
  ofs << content;
  ofs.close();
  return p;
}

std::string Slurp(const fs::path& p) {
  std::ifstream ifs(p, std::ios::binary);
  if (!ifs) {
    throw std::runtime_error("cannot open file: " + p.string());
  }
  ifs.exceptions(std::ifstream::badbit);
  std::ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str();
}

std::pair<int, int> ExpectAndReadPpmHeader(const std::string& ppm) {
  std::istringstream iss(ppm);
  std::string line1, line2, line3;
  std::getline(iss, line1);
  std::getline(iss, line2);
  std::getline(iss, line3);
  EXPECT_EQ(line1, "P3");
  EXPECT_EQ(line3, "255");

  int w = -1;
  int h = -1;
  {
    std::istringstream ls(line2);
    ls >> w >> h;
    EXPECT_TRUE(ls.good() or ls.eof());
    EXPECT_GT(w, 0);
    EXPECT_GT(h, 0);
  }
  return {w, h};
}

std::string BodyAfterHeader(const std::string& ppm) {
  std::istringstream iss(ppm);
  std::string line;
  for (int i = 0; i < 3 and std::getline(iss, line); ++i) {
    // saltar cabecera
  }
  std::ostringstream oss;
  while (std::getline(iss, line)) {
    oss << line << '\n';
  }
  return oss.str();
}

int CountRgbTriples(const std::string& body) {
  const std::regex rgb_line(R"(^\s*\d+\s+\d+\s+\d+\s*$)");
  std::istringstream iss(body);
  std::string line;
  int ok = 0;
  while (std::getline(iss, line)) {
    if (line.empty()) { continue; }
    if (!std::regex_match(line, rgb_line)) { continue; }
    std::istringstream ls(line);
    int r = -1, g = -1, b = -1;
    ls >> r >> g >> b;
    if (ls and (0 <= r and r <= 255) and (0 <= g and g <= 255) and (0 <= b and b <= 255)) {
      ++ok;
    }
  }
  return ok;
}

} // namespace

// -----------------------------------------------------------------------------
// Caso feliz: config mínima válida 2x2 + esfera simple. Debe escribir PPM P3.
// -----------------------------------------------------------------------------
TEST(RenderSOA, ProducesValidPpmForTinyScene) {
  const std::string cfg = R"(image_width: 2
aspect_ratio: 1 1
gamma: 2.2

camera_position: 0 0 -3
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 60

samples_per_pixel: 1
max_depth: 1

material_rng_seed: 123
ray_rng_seed: 456

background_dark_color: 0.25 0.5 1
background_light_color: 1 1 1
)";

  const std::string scn = R"(matte: m1 0.8 0.2 0.2
sphere: 0 0 0 0.5 m1
)";

  const fs::path cfgp = WriteTempFile("cfg-soa", ".txt", cfg);
  const fs::path scnp = WriteTempFile("scn-soa", ".txt", scn);
  const fs::path outp = fs::temp_directory_path() / fs::path("out-soa-" + RandSuffixHex() + ".ppm");

  ASSERT_NO_THROW(render_soa(cfgp.string(), scnp.string(), outp.string()));

  const std::string ppm = Slurp(outp);
  const auto [w, h] = ExpectAndReadPpmHeader(ppm);
  ASSERT_GT(w, 0);
  ASSERT_GT(h, 0);

  const std::string body = BodyAfterHeader(ppm);
  EXPECT_EQ(CountRgbTriples(body), w * h);
}

// -----------------------------------------------------------------------------
// Error: entidad de escena desconocida (debe lanzar).
// -----------------------------------------------------------------------------
TEST(RenderSOA, UnknownSceneEntityThrowsRuntimeError) {
  const std::string cfg = R"(image_width: 2
aspect_ratio: 1 1
)";
  const std::string scn = R"(banana: 1 2 3 4 5 6)";

  const fs::path cfgp = WriteTempFile("cfg-soa-unk", ".txt", cfg);
  const fs::path scnp = WriteTempFile("scn-soa-unk", ".txt", scn);
  const fs::path outp = fs::temp_directory_path() / fs::path("out-soa-" + RandSuffixHex() + ".ppm");

  EXPECT_THROW(render_soa(cfgp.string(), scnp.string(), outp.string()), std::runtime_error);
}

// -----------------------------------------------------------------------------
// Error: referencia a material inexistente (debe lanzar).
// -----------------------------------------------------------------------------
TEST(RenderSOA, MissingMaterialReferenceThrowsRuntimeError) {
  const std::string cfg = R"(image_width: 2
aspect_ratio: 1 1
)";
  const std::string scn = R"(sphere: 0 0 0 1.0 not_found)";

  const fs::path cfgp = WriteTempFile("cfg-soa-nomat", ".txt", cfg);
  const fs::path scnp = WriteTempFile("scn-soa-nomat", ".txt", scn);
  const fs::path outp = fs::temp_directory_path() / fs::path("out-soa-" + RandSuffixHex() + ".ppm");

  EXPECT_THROW(render_soa(cfgp.string(), scnp.string(), outp.string()), std::runtime_error);
}

// -----------------------------------------------------------------------------
// Error: fichero de escena inexistente (debe lanzar). No asume mensaje exacto.
// -----------------------------------------------------------------------------
TEST(RenderSOA, SceneFileNotFoundThrowsRuntimeError) {
  const std::string cfg = R"(image_width: 2
aspect_ratio: 1 1
)";

  const fs::path cfgp = WriteTempFile("cfg-soa-miss-scn", ".txt", cfg);
  const fs::path scnp = fs::temp_directory_path() / fs::path("no-such-" + RandSuffixHex() + ".scene");
  const fs::path outp = fs::temp_directory_path() / fs::path("out-soa-" + RandSuffixHex() + ".ppm");

  EXPECT_THROW(render_soa(cfgp.string(), scnp.string(), outp.string()), std::runtime_error);
}

// -----------------------------------------------------------------------------
// Error: la ruta de salida es un directorio (no se puede abrir como archivo).
// -----------------------------------------------------------------------------
TEST(RenderSOA, OutputPathIsDirectoryThrowsRuntimeError) {
  const std::string cfg = R"(image_width: 2
aspect_ratio: 1 1
)";
  const std::string scn; // escena vacía: si tu loader lo permite, no será el foco del test

  const fs::path cfgp = WriteTempFile("cfg-soa-outdir", ".txt", cfg);
  const fs::path scnp = WriteTempFile("scn-soa-outdir", ".txt", scn);
  const fs::path outp = fs::temp_directory_path(); // directorio

  EXPECT_THROW(render_soa(cfgp.string(), scnp.string(), outp.string()), std::runtime_error);
}

#include "render_aos.hpp"

#include "camera.hpp"
#include "config.hpp"
#include "cylinder.hpp"
#include "image_aos.hpp"
#include "material.hpp"
#include "object.hpp"
#include "ppm_writer.hpp"
#include "ray.hpp"
#include "sampler.hpp"
#include "scene.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace render {

// ============================================================================
// Tipos auxiliares
// ============================================================================
// Contexto de render: punteros no propietarios a cfg/escena/cámara.
struct render_context final {
  render_config const* cfg{nullptr};
  scene* scn{nullptr};
  camera const* cam{nullptr};
};

// Almacén de materiales/objetos (propietarios); la escena guarda punteros crudos.
using material_map = std::unordered_map<std::string, std::unique_ptr<material>>;
using object_store = std::vector<std::unique_ptr<object>>;

// Contexto de construcción de escena: dónde registrar materiales/objetos.
struct scene_build_ctx final {
  scene* scn{nullptr};
  material_map* mats{nullptr};
  object_store* objs{nullptr};
};

// ============================================================================
// Funciones auxiliares de parsing / construcción
// ============================================================================
namespace {

  // Vista de línea: stream tokenizado + cadena original (para errores).
  struct line_view {
    std::istringstream* iss;
    const std::string*   raw;
  };

  // Recorte básico (trim) sin dependencias externas.
  inline std::string trim_copy(std::string s) {
    auto not_space = [](unsigned char c){ return !std::isspace(c); };
    s.erase(s.begin(), std::ranges::find_if(s, not_space));
    s.erase(std::ranges::find_if(std::ranges::reverse_view(s), not_space).base(), s.end());
    return s;
  }

  // Lanzadores de error con línea origen.
  [[noreturn]] void fail_invalid(const std::string& what, const std::string& raw) {
    std::ostringstream oss;
    oss << "Error: " << what << "\n"
        << "Line: \"" << raw << "\"";
    throw std::runtime_error(oss.str());
  }
  [[noreturn]] void fail_unknown_entity(const std::string& tag, const std::string& raw) {
    std::ostringstream oss;
    oss << "Error: Unknown scene entity: " << tag << "\n"
        << "Line: \"" << raw << "\"";
    throw std::runtime_error(oss.str());
  }

  // Verifica que no haya tokens extra tras los esperados.
  inline void check_no_extra(const line_view lv, const char* entity_label) {
    std::string rest;
    std::istream& is = *lv.iss;
    std::getline(is >> std::ws, rest);
    if (!trim_copy(rest).empty()) {
      fail_invalid(std::string("Invalid ") + entity_label + " parameters", *lv.raw);
    }
  }

  // ---------- Materiales ----------
  // Formato: "matte: <name> r g b"
  void parse_matte(const line_view lv, material_map& mats) {
    std::string name;
    double r = 0.0, g = 0.0, b = 0.0;
    if (!((*lv.iss) >> name >> r >> g >> b)) {
      fail_invalid("Invalid matte material parameters", *lv.raw);
    }
    if (mats.contains(name)) {
      fail_invalid("Material with name [" + name + "] already exists", *lv.raw);
    }
    check_no_extra(lv, "matte");
    mats[name] = std::make_unique<material>(name, color{r, g, b});
  }

  // Formato: "metal: <name> r g b fuzz"
  void parse_metal(const line_view lv, material_map& mats) {
    std::string name;
    double r = 0.0, g = 0.0, b = 0.0, fuzz = 0.0;
    if (!((*lv.iss) >> name >> r >> g >> b >> fuzz)) {
      fail_invalid("Invalid metal material parameters", *lv.raw);
    }
    if (mats.contains(name)) {
      fail_invalid("Material with name [" + name + "] already exists", *lv.raw);
    }
    check_no_extra(lv, "metal");
    mats[name] = std::make_unique<material>(name, color{r, g, b}, fuzz);
  }

  // Formato: "refractive: <name> ior"
  void parse_refractive(const line_view lv, material_map& mats) {
    std::string name;
    double ior = 0.0;
    if (!((*lv.iss) >> name >> ior)) {
      fail_invalid("Invalid refractive material parameters", *lv.raw);
    }
    if (mats.contains(name)) {
      fail_invalid("Material with name [" + name + "] already exists", *lv.raw);
    }
    check_no_extra(lv, "refractive");
    mats[name] = std::make_unique<material>(name, ior);
  }

  // Dispatcher de materiales por etiqueta.
  void parse_material(const std::string& tag, const line_view lv, material_map& mats) {
    if      (tag == "matte:")      { parse_matte(lv, mats); }
    else if (tag == "metal:")      { parse_metal(lv, mats); }
    else if (tag == "refractive:") { parse_refractive(lv, mats); }
    else                           { fail_unknown_entity(tag, *lv.raw); }
  }

  // ---------- Objetos ----------
  // Formato: "sphere: x y z r matName"
  void parse_sphere(const line_view lv, scene_build_ctx& ctx) {
    double x = 0.0, y = 0.0, z = 0.0, r = 0.0;
    std::string matname;
    if (!((*lv.iss) >> x >> y >> z >> r >> matname)) {
      fail_invalid("Invalid sphere parameters", *lv.raw);
    }
    check_no_extra(lv, "sphere");
    if (r <= 0.0) {
      fail_invalid("Invalid sphere parameters", *lv.raw);
    }
    auto it = ctx.mats->find(matname);
    if (it == ctx.mats->end()) {
      fail_invalid("Material not found: [" + matname + "]", *lv.raw);
    }
    const material* mat = it->second.get();
    auto s = std::make_unique<sphere>(vector{x, y, z}, r, mat);
    ctx.scn->add(s.get());
    ctx.objs->push_back(std::move(s));
  }

  // Formato: "cylinder: x y z r ax ay az matName"
  void parse_cylinder(const line_view lv, scene_build_ctx& ctx) {
    double x = 0.0, y = 0.0, z = 0.0, r = 0.0;
    double ax = 0.0, ay = 0.0, az = 0.0;
    std::string matname;
    if (!((*lv.iss) >> x >> y >> z >> r >> ax >> ay >> az >> matname)) {
      fail_invalid("Invalid cylinder parameters", *lv.raw);
    }
    check_no_extra(lv, "cylinder");
    if (r <= 0.0) {
      fail_invalid("Invalid cylinder parameters", *lv.raw);
    }
    const vector axis{ax, ay, az};
    if (axis.magnitude() <= 0.0) {
      fail_invalid("Invalid cylinder parameters", *lv.raw);
    }
    auto it = ctx.mats->find(matname);
    if (it == ctx.mats->end()) {
      fail_invalid("Material not found: [" + matname + "]", *lv.raw);
    }
    const material* mat = it->second.get();
    auto c = std::make_unique<cylinder>(vector{x, y, z}, r, axis, mat);
    ctx.scn->add(c.get());
    ctx.objs->push_back(std::move(c));
  }

  // Dispatcher de objetos por etiqueta.
  void parse_object(const std::string& tag, const line_view lv, scene_build_ctx& ctx) {
    if      (tag == "sphere:")   { parse_sphere(lv, ctx); }
    else if (tag == "cylinder:") { parse_cylinder(lv, ctx); }
    else                         { fail_unknown_entity(tag, *lv.raw); }
  }

  // ---------- Carga completa de la escena ----------
  // Lee el fichero de escena; ignora líneas vacías y comentarios (# o //).
  void load_scene(const std::string& path, scene_build_ctx& ctx) {
    std::ifstream in(path);
    if (!in.is_open()) {
      throw std::runtime_error("Error: Could not open scene file: " + path);
    }

    std::string line;
    while (std::getline(in, line)) {
      std::string raw = line;

      // Saltar vacías/comentarios
      auto it = std::ranges::find_if_not(raw, [](unsigned char c){ return std::isspace(c); });
      if (it == raw.end()) { continue; }
      if (*it == '#' or (raw.size() >= 2 and raw[0] == '/' and raw[1] == '/')) { continue; }

      std::istringstream iss(raw);
      const line_view lv{&iss, &raw};

      std::string tag;
      if (!(iss >> tag)) { continue; }

      if (tag == "matte:" or tag == "metal:" or tag == "refractive:") {
        parse_material(tag, lv, *ctx.mats);
      } else if (tag == "sphere:" or tag == "cylinder:") {
        parse_object(tag, lv, ctx);
      } else {
        fail_unknown_entity(tag, raw);
      }
    }
  }

  // ---------- Render principal ----------
  // Recorre la imagen (row-major); para cada píxel hace spp muestras con jitter,
  // traza y promedia. El jitter es uniforme en [-0.5, 0.5]
  void render_image(image_aos& img, const render_context& ctx) {
    const int W = img.width();
    const int H = img.height();
    const int spp = ctx.cfg->samples_per_pixel;
    const int max_depth = ctx.cfg->max_depth;

    sampler rng_ray(ctx.cfg->ray_rng_seed);

    for (int y = 0; y < H; ++y) {
      for (int x = 0; x < W; ++x) {
        color accum{0.0, 0.0, 0.0};
        for (int s = 0; s < spp; ++s) {
          const double dx = rng_ray.next01();
          const double dy = rng_ray.next01();
          const ray r = ctx.cam->primary_ray_from_pixel(
              y, x, dx - 0.5, dy - 0.5);
          accum = accum + ctx.scn->trace(r, max_depth);
        }
        img.set_unchecked(x, y, accum * (1.0 / static_cast<double>(spp)));
      }
    }
  }

} // namespace

// ============================================================================
// Función principal del render AOS
// ============================================================================
// Pipeline: leer cfg → preparar escena/materiales/objetos → cámara → render → PPM.
void render_aos(const std::string& config_path,
                const std::string& scene_path,
                const std::string& output_path) {
  const render_config cfg(config_path);

  scene scn;
  scn.set_background(cfg.background_dark, cfg.background_light);
  scn.set_material_seed(cfg.material_rng_seed);

  material_map mats;
  object_store objs;

  scene_build_ctx build_ctx{&scn, &mats, &objs};
  load_scene(scene_path, build_ctx);

  const camera cam(cfg.to_camera_pose(), cfg.to_camera_optics());

  image_aos img(cfg.image_width, cfg.image_height());
  const render_context rctx{&cfg, &scn, &cam};

  render_image(img, rctx);

  std::ofstream ofs(output_path);
  if (!ofs) {
    throw std::runtime_error("Error: Could not open output file: " + output_path);
  }
  // writer_opts: actualmente sólo 'gamma' (corrección al exportar).
  render::ppm::writer_opts const wopts{ .gamma = cfg.gamma };
  render::ppm::write(ofs, img, wopts);
}

} // namespace render

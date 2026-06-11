#include "render_par.hpp"

#include "camera.hpp"
#include "config.hpp"
#include "cylinder.hpp"
#include "image_par.hpp"
#include "material.hpp"
#include "object.hpp"
#include "ppm_writer.hpp"
#include "ray.hpp"
#include "sampler.hpp"
#include "scene.hpp"
#include "sphere.hpp"

// TBB Headers
#include <oneapi/tbb/blocked_range.h>
#include <oneapi/tbb/enumerable_thread_specific.h>
#include <oneapi/tbb/global_control.h>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/partitioner.h>
#include <tbb/tbb.h>

// Standard Headers
#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace render {

  // ============================================================================
  //  ZONA DE EXPERIMENTACIÓN (Modifica aquí para la memoria)
  // ============================================================================

  // 1. TAMAÑO DE GRANO (Grain Size)
  // Valor 0 o 1 deja que TBB decida automáticamente (recomendado por defecto).
  // Para la gráfica de "Grain Size", prueba valores como: 1, 10, 50, 100, 500.
  constexpr int GRAIN_SIZE = 0;

  // 2. ESTRATEGIA DE PARTICIÓN (Partitioner)
  // Descomenta SOLO UNA de las siguientes líneas para elegir la estrategia.

  // using Partitioner = tbb::auto_partitioner;    // [OPCIÓN A] Por defecto (Inteligente)
  using Partitioner = tbb::simple_partitioner;  // [OPCIÓN B] Divide recursivamente siempre

  // using Partitioner = tbb::static_partitioner;  // [OPCIÓN C] Reparto fijo sin balanceo

  // ============================================================================

  // Estructuras y Tipos
  struct render_context final {
    render_config const * cfg{nullptr};
    scene * scn{nullptr};
    camera const * cam{nullptr};
  };

  using material_map = std::unordered_map<std::string, std::unique_ptr<material>>;
  using object_store = std::vector<std::unique_ptr<object>>;

  struct scene_build_ctx final {
    scene * scn{nullptr};
    material_map * mats{nullptr};
    object_store * objs{nullptr};
  };

  struct pixel_coord {
    int x;
    int y;
  };

  // ============================================================================
  // Funciones auxiliares de parsing (Copia estándar, oculto para brevedad)
  // ============================================================================
  namespace {

    struct line_view {
      std::istringstream * iss;
      std::string const * raw;
    };

    inline std::string trim_copy(std::string s) {
      auto not_space = [](unsigned char c) { return !std::isspace(c); };
      s.erase(s.begin(), std::ranges::find_if(s, not_space));
      s.erase(std::ranges::find_if(std::ranges::reverse_view(s), not_space).base(), s.end());
      return s;
    }

    [[noreturn]] void fail_invalid(std::string const & what, std::string const & raw) {
      std::ostringstream oss;
      oss << "Error: " << what << "\nLine: \"" << raw << "\"";
      throw std::runtime_error(oss.str());
    }

    [[noreturn]] void fail_unknown_entity(std::string const & tag, std::string const & raw) {
      std::ostringstream oss;
      oss << "Error: Unknown entity: " << tag << "\nLine: \"" << raw << "\"";
      throw std::runtime_error(oss.str());
    }

    inline void check_no_extra(line_view const lv, char const * entity_label) {
      std::string rest;
      (*lv.iss) >> std::ws;
      std::getline(*lv.iss, rest);
      if (!trim_copy(rest).empty()) {
        fail_invalid(std::string("Invalid ") + entity_label + " params", *lv.raw);
      }
    }

    void parse_matte(line_view const lv, material_map & mats) {
      std::string name;
      double r = 0.0, g = 0.0, b = 0.0;
      if (!((*lv.iss) >> name >> r >> g >> b)) {
        fail_invalid("Invalid matte", *lv.raw);
      }
      if (mats.contains(name)) {
        fail_invalid("Duplicate material", *lv.raw);
      }
      check_no_extra(lv, "matte");
      mats[name] = std::make_unique<material>(name, color{r, g, b});
    }

    void parse_metal(line_view const lv, material_map & mats) {
      std::string name;
      double r = 0.0, g = 0.0, b = 0.0, f = 0.0;
      if (!((*lv.iss) >> name >> r >> g >> b >> f)) {
        fail_invalid("Invalid metal", *lv.raw);
      }
      if (mats.contains(name)) {
        fail_invalid("Duplicate material", *lv.raw);
      }
      check_no_extra(lv, "metal");
      mats[name] = std::make_unique<material>(name, color{r, g, b}, f);
    }

    void parse_refractive(line_view const lv, material_map & mats) {
      std::string name;
      double ior = 0.0;
      if (!((*lv.iss) >> name >> ior)) {
        fail_invalid("Invalid refractive", *lv.raw);
      }
      if (mats.contains(name)) {
        fail_invalid("Duplicate material", *lv.raw);
      }
      check_no_extra(lv, "refractive");
      mats[name] = std::make_unique<material>(name, ior);
    }

    void parse_material(std::string const & tag, line_view const lv, material_map & mats) {
      if (tag == "matte:") {
        parse_matte(lv, mats);
      } else if (tag == "metal:") {
        parse_metal(lv, mats);
      } else if (tag == "refractive:") {
        parse_refractive(lv, mats);
      } else {
        fail_unknown_entity(tag, *lv.raw);
      }
    }

    void parse_sphere(line_view const lv, scene_build_ctx & ctx) {
      double x = 0.0, y = 0.0, z = 0.0, r = 0.0;
      std::string m;
      if (!((*lv.iss) >> x >> y >> z >> r >> m)) {
        fail_invalid("Invalid sphere", *lv.raw);
      }
      check_no_extra(lv, "sphere");
      if (r <= 0.0) {
        fail_invalid("Invalid radius", *lv.raw);
      }
      if (!ctx.mats->contains(m)) {
        fail_invalid("Material not found", *lv.raw);
      }
      auto s = std::make_unique<sphere>(vector{x, y, z}, r, ctx.mats->at(m).get());
      ctx.scn->add(s.get());
      ctx.objs->push_back(std::move(s));
    }

    void parse_cylinder(line_view const lv, scene_build_ctx & ctx) {
      double x = 0.0, y = 0.0, z = 0.0, r = 0.0, ax = 0.0, ay = 0.0, az = 0.0;
      std::string m;
      if (!((*lv.iss) >> x >> y >> z >> r >> ax >> ay >> az >> m)) {
        fail_invalid("Invalid cylinder", *lv.raw);
      }
      check_no_extra(lv, "cylinder");
      if (r <= 0.0) {
        fail_invalid("Invalid radius", *lv.raw);
      }
      if (vector{ax, ay, az}.magnitude() <= 0.0) {
        fail_invalid("Invalid axis", *lv.raw);
      }
      if (!ctx.mats->contains(m)) {
        fail_invalid("Material not found", *lv.raw);
      }
      auto c =
          std::make_unique<cylinder>(vector{x, y, z}, r, vector{ax, ay, az}, ctx.mats->at(m).get());
      ctx.scn->add(c.get());
      ctx.objs->push_back(std::move(c));
    }

    void parse_object(std::string const & tag, line_view const lv, scene_build_ctx & ctx) {
      if (tag == "sphere:") {
        parse_sphere(lv, ctx);
      } else if (tag == "cylinder:") {
        parse_cylinder(lv, ctx);
      } else {
        fail_unknown_entity(tag, *lv.raw);
      }
    }

    void load_scene(std::string const & path, scene_build_ctx & ctx) {
      std::ifstream in(path);
      if (!in.is_open()) {
        throw std::runtime_error("Error: Could not open scene file: " + path);
      }
      std::string line;
      while (std::getline(in, line)) {
        auto it = std::ranges::find_if_not(line, [](unsigned char c) { return std::isspace(c); });
        if (it == line.end() or
            *it == '#' or
            (line.size() >= 2 and line[0] == '/' and line[1] == '/'))
        {
          continue;
        }
        std::istringstream iss(line);
        line_view const lv{&iss, &line};
        std::string tag;
        if (!(iss >> tag)) {
          continue;
        }
        if (!tag.empty() and tag.back() == ':') {
          if (tag == "sphere:" or tag == "cylinder:") {
            parse_object(tag, lv, ctx);
          } else {
            parse_material(tag, lv, *ctx.mats);
          }
        } else {
          fail_unknown_entity(tag, line);
        }
      }
    }

    // ========================================================================
    // LÓGICA PARALELA (Semillas)
    // ========================================================================
    std::vector<std::uint64_t> generate_seeds(std::uint64_t master_seed, std::size_t count) {
      std::vector<std::uint64_t> seeds(count);
      std::mt19937_64 gen(master_seed);
      for (auto & s : seeds) {
        s = gen();
      }
      return seeds;
    }

    void render_pixel(pixel_coord coord, sampler & rng, render_context const & ctx,
                      image_aos & img) {
      int const spp       = ctx.cfg->samples_per_pixel;
      int const max_depth = ctx.cfg->max_depth;
      color accum{0.0, 0.0, 0.0};
      for (int s = 0; s < spp; ++s) {
        double const dx = rng.next01();
        double const dy = rng.next01();
        ray const r     = ctx.cam->primary_ray_from_pixel(coord.y, coord.x, dx - 0.5, dy - 0.5);
        accum           = accum + ctx.scn->trace(r, max_depth);
      }
      img.set_unchecked(coord.x, coord.y, accum * (1.0 / static_cast<double>(spp)));
    }

    // ========================================================================
    // BUCLE DE RENDER (Configurable)
    // ========================================================================
    void render_image(image_aos & img, render_context const & ctx) {
      int const W = img.width();
      int const H = img.height();

      // 1. Preparar semillas (Hilo seguro)
      std::size_t const max_threads =
          tbb::global_control::active_value(tbb::global_control::max_allowed_parallelism);
      auto const seeds = generate_seeds(ctx.cfg->ray_rng_seed, max_threads * 4);

      std::atomic<std::size_t> seed_counter{0};
      tbb::enumerable_thread_specific<sampler> thread_samplers([&]() {
        std::size_t const idx = seed_counter.fetch_add(1) % seeds.size();
        return sampler(seeds[idx]);
      });

      // 2. Ejecutar Parallel For
      // Usamos GRAIN_SIZE y Partitioner definidos al inicio del archivo

      // Definimos el rango según la configuración
      auto const range = (GRAIN_SIZE > 0)
                             ? tbb::blocked_range<int>(0, H, GRAIN_SIZE)  // Grano manual
                             : tbb::blocked_range<int>(0, H);             // Grano automático

      tbb::parallel_for(
          range,
          [&](tbb::blocked_range<int> const & r) {
            sampler & rng_ray = thread_samplers.local();
            for (int y = r.begin(); y != r.end(); ++y) {
              for (int x = 0; x < W; ++x) {
                render_pixel({x, y}, rng_ray, ctx, img);
              }
            }
          },
          Partitioner()  // <--- Aquí se inyecta la estrategia elegida arriba
      );
    }

  }  // namespace

  void render_par(std::string const & config_path, std::string const & scene_path,
                  std::string const & output_path) {
    render_config const cfg(config_path);
    scene scn;
    scn.set_background(cfg.background_dark, cfg.background_light);
    scn.set_material_seed(cfg.material_rng_seed);
    material_map mats;
    object_store objs;
    scene_build_ctx build_ctx{&scn, &mats, &objs};
    load_scene(scene_path, build_ctx);
    camera const cam(cfg.to_camera_pose(), cfg.to_camera_optics());
    image_aos img(cfg.image_width, cfg.image_height());
    render_context const rctx{&cfg, &scn, &cam};

    render_image(img, rctx);

    std::ofstream ofs(output_path);
    if (!ofs) {
      throw std::runtime_error("Error: Could not open output: " + output_path);
    }
    render::ppm::writer_opts const wopts{.gamma = cfg.gamma};
    render::ppm::write(ofs, img, wopts);
  }

}  // namespace render

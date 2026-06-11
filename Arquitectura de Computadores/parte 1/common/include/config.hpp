#pragma once
// ======================================================================
// config.hpp — Configuración del render
// Claves esperadas
//   aspect_ratio:, image_width:, gamma:,
//   camera_position:, camera_target:, camera_north:, field_of_view:,
//   samples_per_pixel:, max_depth:,
//   material_rng_seed:, ray_rng_seed:,
//   background_dark_color:, background_light_color:
// Reglas:
//   - Una etiqueta por línea con sus valores tras “:”.
//   - Claves desconocidas → "Unknown configuration key: [...]".
//   - Datos extra → "Extra data after configuration value for key: [...]".
//   - Valor inválido/formato incorrecto → "Invalid value for key: [...]".
//   - Parámetros repetidos → prevalece el último.
// ======================================================================

#include "vector.hpp"
#include "color.hpp"
#include <string>
#include <istream>
#include <cstdint>
#include <algorithm>
#include <cmath>

namespace render {

// Adelantos para desacoplar del header de cámara.
struct camera_pose;
struct camera_optics;

struct render_config final {
    // -------- Imagen ---------------------------------------------
    int   image_width{1920}; // >0
    int   aspect_x{16};      // >0 (numerador de la razón de aspecto)
    int   aspect_y{9};       // >0 (denominador)

    // Gamma de salida (se aplica al exportar, no en el cómputo interno).
    double gamma{2.2};

    // -------- Cámara --------------------------------------
    vector camera_position{0.0, 0.0, -10.0};
    vector camera_target  {0.0, 0.0,   0.0};
    vector camera_north   {0.0, 1.0,   0.0};
    double field_of_view  {90.0}; // válido si 0 < FOV < 180

    // -------- Muestreo / Integrador ------------------------
    int samples_per_pixel{20}; // >0
    int max_depth{5};          // >0

    // -------- RNG ------------------------------------------------
    std::uint64_t material_rng_seed{13}; // >0 (semilla materiales)
    std::uint64_t ray_rng_seed{19};      // >0 (si se usa RNG para rayos/cámara)

    // -------- Fondo gradiente ------------------------------
    color background_dark  {0.25, 0.50, 1.00}; // componentes en [0,1]
    color background_light {1.00, 1.00, 1.00}; // componentes en [0,1]

    // -------- Derivados / Helpers -------------------------------------------
    // Altura entera según width y aspect_x:aspect_y.
    // Se usa floor para mantener la proporción sin “sobrepasar” (no redondeo).
    [[nodiscard]] int image_height() const noexcept {
        const double h = static_cast<double>(image_width)
                       * static_cast<double>(aspect_y)
                       / static_cast<double>(aspect_x);
        return std::max(1, static_cast<int>(std::floor(h))); // antes: lround
    }

    // Conversión a tipos de cámara del proyecto (definidos en camera.hpp).
    [[nodiscard]] camera_pose   to_camera_pose()   const noexcept;
    [[nodiscard]] camera_optics to_camera_optics() const noexcept;

    // -------- Parsing --------------------------------------------------------
    // Carga desde fichero (lanza en caso de clave/valor inválidos).
    explicit render_config(const std::string& path);
    render_config() = default;

    // Parse desde un istream (último valor gana).
    static render_config parse(std::istream& is);
};

} // namespace render

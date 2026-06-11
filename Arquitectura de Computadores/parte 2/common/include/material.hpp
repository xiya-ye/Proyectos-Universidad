#pragma once
// ============================================================================
// material.hpp — Materiales y dispersión
// Tipos: mate, metal, refractivo. Usados por la escena
// Dispersión:
//   • Mate: dr = dn + ξ, ξi ~ U[-1,1]; si |ξ|≈0 usar dn.
//   • Metal: d1r = do − 2(do·dn)dn; dr = normalize(d1r) + ϕ, ϕi ~ U[-Φ,Φ].
//   • Refractivo: calcula refracción/reflectancia; TIR si sinθ·η>1.
//   • Todos usan el MISMO RNG compartido (semilla en config; por defecto 13).
// ============================================================================

#include <algorithm>
#include <optional>
#include <string>
#include <utility>

#include "color.hpp"
#include "object.hpp"
#include "ray.hpp"

namespace render {

  enum class MaterialType { Matte, Metal, Refractive };

  class sampler;  // RNG compartido para materiales

  // ---------------------------------------------------------------------------
  // material — describe cómo dispersa la luz un objeto en un impacto.
  //  - name_: etiqueta legible (debug/escena).
  //  - type_: tipo de material (mate/metal/refractivo).
  //  - albedo_: atenuación por canal en espacio lineal [0,1].
  //  - fuzz_: difusión del metal (Φ) para micro-aspereza.
  //  - ior_: índice de refracción (ρ).
  // ---------------------------------------------------------------------------
  class material final {
  public:
    // Umbral para decidir “vector demasiado pequeño” en mate
    static constexpr double kTiny = 1e-8;

    // --- Constructores --------------------------------
    // Mate: reflectancia RGB en [0,1]; se satura por seguridad.
    explicit material(std::string name, color albedo) noexcept
        : name_(std::move(name)),
          albedo_{std::clamp(albedo.r, 0.0, 1.0), std::clamp(albedo.g, 0.0, 1.0),
                  std::clamp(albedo.b, 0.0, 1.0)} { }

    // Metal: reflectancia y factor de difusión Φ (no se fuerza rango aquí).
    explicit material(std::string name, color albedo, double fuzz) noexcept
        : name_(std::move(name)), type_(MaterialType::Metal),
          albedo_{std::clamp(albedo.r, 0.0, 1.0), std::clamp(albedo.g, 0.0, 1.0),
                  std::clamp(albedo.b, 0.0, 1.0)},
          fuzz_(fuzz) { }

    // Refractivo: índice ρ; reflectancia fija blanca (energía conservada en RGB).
    explicit material(std::string name, double ior) noexcept
        : name_(std::move(name)), type_(MaterialType::Refractive), albedo_{1.0, 1.0, 1.0},
          ior_(ior) { }

    // Dispersión: devuelve (rayo_saliente, atenuación) o nullopt (absorción).
    [[nodiscard]] std::optional<std::pair<ray, color>> scatter(ray const & in, Hit const & h,
                                                               sampler & rng) const noexcept;

  private:
    std::string name_;
    MaterialType type_{MaterialType::Matte};
    color albedo_{1.0, 1.0, 1.0};
    double fuzz_{0.0};  // Φ (metal)
    double ior_{1.5};   // ρ (refracción)
  };

}  // namespace render

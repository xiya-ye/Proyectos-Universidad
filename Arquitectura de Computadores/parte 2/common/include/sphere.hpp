#pragma once
// ============================================================================
// sphere.hpp — Esfera
//  • Centro C, radio r>0 y puntero a material (no propietario).
//  • intersect(r, t_min, t_max): devuelve el hit más cercano en [t_min, t_max].
//  • La normal se orienta con set_face_normal (correcto para refracción).
// ============================================================================

#include "object.hpp"

namespace render {

  class sphere final : public object {
  public:
    // Crea una esfera unidad en el origen sin material.
    sphere() noexcept = default;

    // Construcción explícita: r<=0 se satura a 0 por seguridad.
    sphere(vector const & c, double r, material const * m) noexcept
        : center_{c}, radius_{r > 0.0 ? r : 0.0}, mat_ptr_{m} { }

    // Intersección rayo-esfera:
    //  - Resuelve |O + tD - C|^2 = r^2 y elige el menor t válido en [t_min, t_max].
    //  - Si hay impacto, rellena Hit (p, t, mat) y orienta la normal con set_face_normal.
    [[nodiscard]] hit_opt intersect(ray const & r, double t_min,
                                    double t_max) const noexcept override;

    // --- Acceso --------------------------------------------------------------
    [[nodiscard]] vector const & center() const noexcept { return center_; }

    [[nodiscard]] double radius() const noexcept { return radius_; }

    [[nodiscard]] material const * material_ptr() const noexcept { return mat_ptr_; }

  private:
    vector center_{0.0, 0.0, 0.0};       // C
    double radius_{1.0};                 // r > 0
    material const * mat_ptr_{nullptr};  // material asociado (no se posee)
  };

}  // namespace render

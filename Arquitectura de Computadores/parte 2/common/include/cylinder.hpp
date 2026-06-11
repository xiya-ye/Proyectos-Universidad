#pragma once
// ============================================================================
// cylinder.hpp — Cilindro finito con eje arbitrario
//  • Definición: centro C, radio r>0 y eje a⃗; h=||a⃗||, â=a⃗/h, semialtura h/2.
//  • Intersección: (1) superficie curva (cilindro infinito + filtro de altura) y (2) tapas.
//  • Orientación de la normal: si D·n < 0 queda hacia fuera; en caso contrario, se invierte.
// ============================================================================

#include "object.hpp"

namespace render {

  class cylinder final : public object {
  public:
    // Constructor conforme al formato de escena: C, r, a⃗ y material.
    cylinder(vector const & c, double r, vector const & axis, material const * m) noexcept;

    // Intersección acotada: devuelve el hit más cercano en [t_min, t_max] o nullopt.
    [[nodiscard]] hit_opt intersect(ray const & r, double t_min,
                                    double t_max) const noexcept override;

  private:
    // Plano de tapa: centro P y normal unitaria n = ±â.
    struct Cap {
      vector P;
      vector n;
    };

    // ---------------------- Auxiliares de intersección -----------------------
    // (1) Superficie curva:
    //     • Proyecta sobre el subespacio ⟂ â y resuelve la cuadrática.
    //     • Acepta solo si la altura ∈ [-h/2, h/2].
    [[nodiscard]] hit_opt intersect_curved(ray const & r, double t_min,
                                           double t_max) const noexcept;

    // (2) Tapas:
    //     • Interseca con el plano (P,n) y comprueba distancia al centro ≤ r.
    [[nodiscard]] hit_opt intersect_cap(ray const & r, Cap const & cap, double t_min,
                                        double t_max) const noexcept;

    // ------------------------ Utilidades geométricas -------------------------
    // Altura relativa respecto al eje: (p − C)·â.
    [[nodiscard]] double height_at(vector const & p) const noexcept;

    // ¿Está la altura dentro del cilindro? |h| ≤ h/2.
    [[nodiscard]] bool height_inside(double hval) const noexcept;

    // Construye el Hit y orienta la normal según el criterio del PDF.
    [[nodiscard]] hit_opt make_hit(ray const & r, double t, vector const & p,
                                   vector const & outward) const noexcept;

    // -------------------------- Parámetros del cilindro ----------------------
    vector center_{0.0, 0.0, 0.0};       // C
    double radius_{1.0};                 // r (>0; validado al parsear)
    vector axis_unit_{0.0, 1.0, 0.0};    // â (eje normalizado)
    double half_height_{0.5};            // h/2
    material const * mat_ptr_{nullptr};  // material asociado (no propietario)
  };

}  // namespace render

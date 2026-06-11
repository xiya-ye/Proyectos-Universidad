#pragma once
// ============================================================================
// ray.hpp
// Rayo 3D: P(t) = Or + t · D
// - No forzamos D unitario: se normaliza donde haga falta
// - t suele acotarse en [t_min, t_max] al resolver intersecciones.
// ============================================================================

#include "vector.hpp"

namespace render {

  /*
   * ray — representación mínima de un rayo paramétrico.
   * origin: punto de partida Or
   * direction: dirección D (no necesariamente normalizada)
   * Notas:
   *  - constexpr/noexcept para permitir uso en contextos de tiempo de compilación
   *    y facilitar optimizaciones.
   */
  struct ray {
    vector origin;     // Or: punto de origen
    vector direction;  // D: dirección (puede no ser unitaria)

    // --- Construcción --------------------------------------------------------
    constexpr ray() noexcept = default;

    constexpr ray(vector const & o, vector const & d) noexcept : origin(o), direction(d) { }

    // --- Evaluación ----------------------------------------------------------
    // Punto a distancia paramétrica t: P(t) = Or + t·D
    // Útil para recuperar posiciones de impacto tras resolver intersecciones.
    [[nodiscard]] constexpr vector at(double t) const noexcept { return origin + direction * t; }
  };

}  // namespace render

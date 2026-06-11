#pragma once
// ============================================================================
// object.hpp — Base abstracta de primitivas 3D
//  • Cada objeto expone intersect(r, t_min, t_max) y devuelve un Hit opcional.
//  • La escena se queda con el impacto más cercano y descarta λ < 1e-3.
// ============================================================================

#include "ray.hpp"
#include "vector.hpp"
#include <optional>

namespace render {

  class material;  // Declaración adelantada (puntero no propietario)

  // ------------------------------ Hit -----------------------------------------
  // Información de una intersección rayo-objeto.
  struct Hit final {
    vector p;                       // Punto de impacto I = Or + D * λ
    vector normal;                  // Normal unitaria orientada (ver set_face_normal)
    double t{};                     // Parámetro λ sobre el rayo (distancia paramétrica)
    material const * mat{nullptr};  // Material en el punto (no propiedad)
    bool front_face{true};          // true si el rayo entra desde fuera (D·n < 0)

    // Orienta la normal para coherencia en iluminación y refracción:
    //  - outward_normal debe venir unitario y apuntando hacia fuera.
    //  - Si D·n < 0, mantenemos n; si no, la invertimos.
    void set_face_normal(ray const & r, vector const & outward_normal) noexcept {
      front_face = (r.direction.dot(outward_normal) < 0.0);
      normal     = front_face ? outward_normal : -outward_normal;
    }
  };

  using hit_opt = std::optional<Hit>;  // Impacto presente/ausente

  // --------------------------- Clase base -------------------------------------
  // Interfaz mínima para primitivas. No copiable/movable para evitar slicing
  // y ambigüedades de propiedad; destrucción virtual para liberar por puntero base.
  class object {
  public:
    object()                           = default;
    object(object const &)             = delete;
    object & operator=(object const &) = delete;
    object(object &&)                  = delete;
    object & operator=(object &&)      = delete;
    virtual ~object()                  = default;

    // Intersección acotada: devuelve el hit más cercano en [t_min, t_max] o nullopt.
    // La escena aplicará su umbral de λ_min (≈ 1e-3) para evitar auto-impactos.
    [[nodiscard]] virtual hit_opt intersect(ray const & r, double t_min,
                                            double t_max) const noexcept = 0;
  };

}  // namespace render

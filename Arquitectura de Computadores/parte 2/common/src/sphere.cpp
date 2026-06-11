#include "sphere.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>
#include <optional>

namespace render {

  hit_opt sphere::intersect(ray const & r, double t_min, double t_max) const noexcept {
    // Ecuación de intersección: |O + tD - C|^2 = r^2  → cuadrática en t.
    vector const oc     = r.origin - center_;
    double const a      = r.direction.dot(r.direction);  // ≈1 si D viene unitario; se deja general.
    double const half_b = oc.dot(r.direction);  // Usamos b/2 para mejor estabilidad numérica.
    double const c      = oc.dot(oc) - radius_ * radius_;
    double const disc   = half_b * half_b - a * c;  // Discriminante
    if (disc < 0.0) {
      return std::nullopt;
    }

    double const sqrt_disc = std::sqrt(disc);

    // Elegir la raíz más pequeña válida dentro de [t_min, t_max].
    double root = (-half_b - sqrt_disc) / a;
    if (root < t_min or root > t_max) {
      root = (-half_b + sqrt_disc) / a;
      if (root < t_min or root > t_max) {
        return std::nullopt;
      }
    }

    // Rellenar el impacto.
    Hit h{};
    h.t   = root;
    h.p   = r.at(root);
    h.mat = mat_ptr_;

    // Normal geométrica hacia fuera y orientación coherente con el sentido del rayo.
    vector const outward_normal = (h.p - center_) / radius_;
    h.set_face_normal(r, outward_normal);

    return h;
  }

}  // namespace render

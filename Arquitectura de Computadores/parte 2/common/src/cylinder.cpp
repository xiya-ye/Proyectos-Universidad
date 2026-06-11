#include "cylinder.hpp"
#include "material.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <optional>

#include <algorithm>
#include <cmath>

namespace render {

  namespace {

    // Umbral para considerar rayo paralelo al plano de tapa: |D·n| < 1e-8
    constexpr double kParallelEpsPlanes = 1e-8;

    // Proyección ortogonal de v sobre el subespacio perpendicular a â: v⊥â = v − (v·â)â
    inline vector perp_to_axis(vector const & v, vector const & a_hat) {
      return v - a_hat * v.dot(a_hat);
    }

    struct Roots {
      double t0{};
      double t1{};
      bool ok{false};
    };

    inline Roots solve_curved_quadratic(vector const & rc, vector const & dir, vector const & a_hat,
                                        double radius) noexcept {
      vector const d_perp  = perp_to_axis(dir, a_hat);
      vector const rc_perp = perp_to_axis(rc, a_hat);
      double const A       = d_perp.dot(d_perp);  // a = ||D⊥||²
      if (A == 0.0) {
        return {};  // rayo paralelo a superficie lateral
      }

      double const half_b = rc_perp.dot(d_perp);                     // b/2 = r_c⊥ · D⊥
      double const C      = rc_perp.dot(rc_perp) - radius * radius;  // c = ||r_c⊥||² − r²
      double const disc   = half_b * half_b - A * C;                 // Δ = (b/2)² − a·c
      if (disc < 0.0) {
        return {};  // sin intersección
      }

      double const s    = std::sqrt(disc);
      double const invA = 1.0 / A;
      double t0         = (-half_b - s) * invA;  // raíz cercana
      double t1         = (-half_b + s) * invA;  // raíz lejana
      if (t0 > t1) {
        std::swap(t0, t1);
      }

      return {t0, t1, true};
    }

  }  // namespace

  cylinder::cylinder(vector const & c, double r, vector const & axis, material const * m) noexcept
      : center_{c}, radius_{r}, mat_ptr_{m} {
    double const len = axis.magnitude();
    if (len > 0.0) {
      axis_unit_   = axis / len;  // â
      half_height_ = 0.5 * len;   // h/2
    } else {
      axis_unit_   = {0.0, 1.0, 0.0};  // eje por defecto
      half_height_ = 0.0;
    }
  }

  double cylinder::height_at(vector const & p) const noexcept {
    // Altura relativa respecto al centro C y eje â
    return (p - center_).dot(axis_unit_);
  }

  bool cylinder::height_inside(double h) const noexcept {
    // Punto dentro del rango vertical del cilindro ⇔ |h| ≤ h/2
    return std::fabs(h) <= half_height_;
  }

  hit_opt cylinder::make_hit(ray const & r, double t, vector const & p,
                             vector const & outward) const noexcept {
    // Construye Hit y orienta normal según el sentido del rayo (set_face_normal)
    Hit h{};
    h.t   = t;
    h.p   = p;
    h.mat = mat_ptr_;
    h.set_face_normal(r, outward);
    return h;
  }

  hit_opt cylinder::intersect(ray const & r, double t_min, double t_max) const noexcept {
    // Prueba superficie curva y, después, tapas; conserva el impacto más cercano.
    hit_opt best{};
    double best_t = t_max;

    if (auto h = intersect_curved(r, t_min, best_t)) {
      best_t = h->t;
      best   = h;
    }

    Cap const top{center_ + axis_unit_ * half_height_, axis_unit_};
    Cap const bottom{center_ - axis_unit_ * half_height_, -axis_unit_};

    if (auto ht = intersect_cap(r, top, t_min, best_t)) {
      best_t = ht->t;
      best   = ht;
    }
    if (auto hb = intersect_cap(r, bottom, t_min, best_t)) {
      best_t = hb->t;
      best   = hb;
    }

    return best;
  }

  hit_opt cylinder::intersect_curved(ray const & r, double t_min, double t_max) const noexcept {
    // Cilindro degenerado: sin radio o sin altura
    if (radius_ <= 0.0 or half_height_ <= 0.0) {
      return std::nullopt;
    }

    // Resolver cuadrática en el subespacio ⟂ â (misma matemática)
    vector const rc   = r.origin - center_;
    Roots const roots = solve_curved_quadratic(rc, r.direction, axis_unit_, radius_);
    if (!roots.ok) {
      return std::nullopt;
    }

    // Comprueba raíz si cae en el cilindro finito (filtro de altura)
    auto try_root = [&](double t) -> hit_opt {
      if (t < t_min or t > t_max) {
        return std::nullopt;
      }

      vector const p = r.at(t);
      double const h = height_at(p);  // (I − C)·â
      if (!height_inside(h)) {
        return std::nullopt;
      }

      // Normal lateral: componente ⟂ â de (I − C).
      vector const outward = (p - center_) - axis_unit_ * h;
      if (outward.length_squared() == 0.0) {
        return std::nullopt;
      }

      return make_hit(r, t, p, outward);
    };

    if (auto h = try_root(roots.t0)) {
      return h;  // primero, raíz cercana
    }
    return try_root(roots.t1);  // si no, la otra
  }

  hit_opt cylinder::intersect_cap(ray const & r, Cap const & cap, double t_min,
                                  double t_max) const noexcept {
    // Rayo–plano: (P0 + tD − P)·n = 0 ⇒ t = ((P − P0)·n) / (D·n)
    double const denom = r.direction.dot(cap.n);
    if (std::fabs(denom) <= kParallelEpsPlanes) {
      return std::nullopt;
    }

    double const t = (cap.P - r.origin).dot(cap.n) / denom;
    if (t < t_min or t > t_max) {
      return std::nullopt;
    }

    // Punto y test de radio dentro del disco
    vector const p     = r.at(t);
    double const dist2 = (p - cap.P).length_squared();
    double const r2    = radius_ * radius_;
    if (dist2 > r2) {
      return std::nullopt;
    }

    // La normal de la tapa ya es ±â; set_face_normal ajustará el signo final.
    return make_hit(r, t, p, cap.n);
  }

}  // namespace render

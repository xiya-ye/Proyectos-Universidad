#include "cylinder.hpp"
#include "object.hpp"
#include "vector.hpp"
#include "ray.hpp"
#include "material.hpp"
#include <optional>

#include <cmath>
#include <algorithm>

namespace render {

namespace {
// Umbral para considerar rayo paralelo al plano de tapa: |D·n| < 1e-8
constexpr double kParallelEpsPlanes = 1e-8;

// Proyección ortogonal de v sobre el subespacio perpendicular a â: v⊥â = v − (v·â)â
inline vector perp_to_axis(const vector& v, const vector& a_hat) {
    return v - a_hat * v.dot(a_hat);
}
} // namespace

cylinder::cylinder(const vector& c, double r, const vector& axis, const material* m) noexcept
    : center_{c}, radius_{r}, mat_ptr_{m} {
    const double len = axis.magnitude();
    if (len > 0.0) {
        axis_unit_   = axis / len;       // â
        half_height_ = 0.5 * len;        // h/2
    } else {
        axis_unit_   = {0.0, 1.0, 0.0};  // eje por defecto
        half_height_ = 0.0;
    }
}

double cylinder::height_at(const vector& p) const noexcept {
    // Altura relativa respecto al centro C y eje â
    return (p - center_).dot(axis_unit_);
}

bool cylinder::height_inside(double h) const noexcept {
    // Punto dentro del rango vertical del cilindro ⇔ |h| ≤ h/2
    return std::fabs(h) <= half_height_;
}

hit_opt cylinder::make_hit(const ray& r, double t,
                           const vector& p, const vector& outward) const noexcept {
    // Construye Hit y orienta normal según el sentido del rayo (set_face_normal)
    Hit h{};
    h.t   = t;
    h.p   = p;
    h.mat = mat_ptr_;
    h.set_face_normal(r, outward);
    return h;
}

hit_opt cylinder::intersect(const ray& r, double t_min, double t_max) const noexcept {
    // Prueba superficie curva y, después, tapas; conserva el impacto más cercano.
    hit_opt best{};
    double  best_t = t_max;

    if (auto h = intersect_curved(r, t_min, best_t)) {
        best_t = h->t;
        best   = h;
    }

    const Cap top   { center_ + axis_unit_ * half_height_,  axis_unit_ };
    const Cap bottom{ center_ - axis_unit_ * half_height_, -axis_unit_ };

    if (auto ht = intersect_cap(r, top,    t_min, best_t)) { best_t = ht->t; best = ht; }
    if (auto hb = intersect_cap(r, bottom, t_min, best_t)) { best_t = hb->t; best = hb; }

    return best;
}

hit_opt cylinder::intersect_curved(const ray& r, double t_min, double t_max) const noexcept {
    // Cilindro degenerado: sin radio o sin altura
    if (radius_ <= 0.0 or half_height_ <= 0.0) { return std::nullopt; }
    // Parámetros para la cuadrática en el subespacio ⟂ â
    const vector rc      = r.origin - center_;            // r_c = Or − C
    const vector d_perp  = perp_to_axis(r.direction, axis_unit_);
    const vector rc_perp = perp_to_axis(rc,          axis_unit_);
    const double A = d_perp.dot(d_perp);                  // a = ||D⊥||²
    if (A == 0.0) { return std::nullopt; }                // Rayo paralelo a superficie lateral

    const double half_b = rc_perp.dot(d_perp);            // b/2 = r_c⊥ · D⊥
    const double C      = rc_perp.dot(rc_perp) - radius_ * radius_; // c = ||r_c⊥||² − r²
    const double disc   = half_b * half_b - A * C;        // Δ = (b/2)² − a·c
    if (disc < 0.0) { return std::nullopt; }

    const double s    = std::sqrt(disc);
    const double invA = 1.0 / A;
    double t0 = (-half_b - s) * invA;                     // raíz cercana
    double t1 = (-half_b + s) * invA;                     // raíz lejana
    if (t0 > t1) { std::swap(t0, t1); }

    // Comprueba raíz si cae en el cilindro finito (filtro de altura)
    auto try_root = [&](double t) -> hit_opt {
        if (t < t_min or t > t_max) { return std::nullopt; }

        const vector p = r.at(t);
        const double h = height_at(p);                     // (I − C)·â
        if (!height_inside(h)) { return std::nullopt; }

        // Normal lateral: componente ⟂ â de (I − C).
        // IMPORTANTE: no normalizar aquí (coherente con la práctica y evita artefactos).
        const vector outward = (p - center_) - axis_unit_ * h;
        if (outward.length_squared() == 0.0) { return std::nullopt; }

        return make_hit(r, t, p, outward);
    };

    if (auto h = try_root(t0)) { return h; }              // primero, raíz cercana
    return try_root(t1);                                   // si no, la otra
}

hit_opt cylinder::intersect_cap(const ray& r, const Cap& cap,
                                double t_min, double t_max) const noexcept {
    // Rayo–plano: (P0 + tD − P)·n = 0 ⇒ t = ((P − P0)·n) / (D·n)
    const double denom = r.direction.dot(cap.n);
    if (std::fabs(denom) <= kParallelEpsPlanes) { return std::nullopt; }

    const double t = (cap.P - r.origin).dot(cap.n) / denom;
    if (t < t_min or t > t_max) { return std::nullopt; }

    // Punto y test de radio dentro del disco
    const vector p     = r.at(t);
    const double dist2 = (p - cap.P).length_squared();
    const double r2    = radius_ * radius_;
    if (dist2 > r2) { return std::nullopt; }

    // La normal de la tapa ya es ±â; set_face_normal ajustará el signo final.
    return make_hit(r, t, p, cap.n);
}

} // namespace render

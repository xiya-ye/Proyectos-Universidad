#include "material.hpp"
#include "sampler.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include "color.hpp"
#include "object.hpp"

#include <optional>
#include <cmath>
#include <utility>
#include <algorithm>

namespace render {

namespace {

// --- Utilidades internas ----------------------------------------------------

// Reflexión especular: d_r = d - 2(d·n)n
inline vector reflect(const vector& d, const vector& n) noexcept {
    return d - 2.0 * d.dot(n) * n;
}

// Criterio “vector demasiado pequeño” para el caso mate
inline bool too_small(const vector& v) noexcept {
    return (std::fabs(v.x) < material::kTiny) and
           (std::fabs(v.y) < material::kTiny) and
           (std::fabs(v.z) < material::kTiny);
}

// Uniforme en [a,b] usando el RNG compartido (mate: [-1,1], metal: [-Φ,Φ])
inline double rand_in(sampler& rng, double a, double b) {
    return rng.uniform(a, b);
}

} // namespace

// ============================================================================
// scatter() — Dispersión
//  • Or del nuevo rayo = I (h.p).
//  • La atenuación depende del material (refractivo es blanco).
// ============================================================================
std::optional<std::pair<ray, color>>
material::scatter(const ray& r_in, const Hit& h, sampler& rng) const noexcept
{
    const vector n      = h.normal;                 // Unitario y ya orientado (set_face_normal)
    const vector do_hat = r_in.direction.normalize();
    if (type_ == MaterialType::Matte) {
        const vector jitter{ rand_in(rng, -1.0,  1.0),
                             rand_in(rng, -1.0,  1.0),
                             rand_in(rng, -1.0,  1.0) };
        vector dr = n + jitter;
        if (too_small(dr)) { dr = n; }             // Evita direcciones degeneradas
        return std::make_pair(ray{ h.p, dr }, albedo_);
    }
    if (type_ == MaterialType::Metal) {
        const vector d1r    = reflect(r_in.direction, n);
        const vector d1rhat = d1r.normalize();
        const vector phi{ rand_in(rng, -fuzz_, +fuzz_),
                          rand_in(rng, -fuzz_, +fuzz_),
                          rand_in(rng, -fuzz_, +fuzz_) };
        const vector dr = d1rhat + phi;
        return std::make_pair(ray{ h.p, dr }, albedo_);
    }
    if (type_ == MaterialType::Refractive) {
        const double cos_theta = std::min(-do_hat.dot(n), 1.0);
        const double sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta * cos_theta));
        const double rho_p = h.front_face ? (1.0 / ior_) : ior_; // η = n1/n2
        vector dr;
        // TIR si η·sinθ > 1
        if (rho_p * sin_theta > 1.0) {
            dr = reflect(do_hat, n);
        } else {
            // u = η(do + cosθ n); v = -sqrt(|1 - ||u||^2|) n; dr = u + v
            const vector u  = rho_p * (do_hat + cos_theta * n);
            const double u2 = u.dot(u);
            const double w  = std::sqrt(std::max(0.0, std::abs(1.0 - u2)));
            const vector v  = -w * n;
            dr = u + v;
        }
        return std::make_pair(ray{ h.p, dr }, color{ 1.0, 1.0, 1.0 });
    }
    // Tipo no reconocido (defensivo)
    return std::nullopt;
}
} // namespace render

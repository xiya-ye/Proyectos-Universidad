// ============================================================================
// scene.cpp
//  - material.hpp solo se necesita aquí (scene.hpp no lo incluye).
//  - Búsqueda del hit más cercano con λ_min = 1e-3
//  - Trazado recursivo con límite de profundidad y fondo gradiente
// ============================================================================
#include "scene.hpp"
#include "material.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "color.hpp"
#include <limits>

namespace render {

hit_opt scene::intersect(const ray& r) const noexcept {
    hit_opt best{};
    double t_best = std::numeric_limits<double>::infinity();

    // Recorre los objetos pidiendo intersecciones acotadas.
    // t_best se usa como podado: si aparece un hit más cercano, estrechamos t_max.
    for (const object* obj : objects_) {
        if (obj == nullptr) { continue; }
        const hit_opt h = obj->intersect(r, scene::kLambdaMin, t_best);
        if (h) {
            t_best = h->t;   // nuevo “mejor hasta ahora”
            best   = h;
        }
    }
    return best; // nullopt si no hay impactos
}

color scene::trace(const ray& r, int depth) const noexcept {
    // Caso base: sin presupuesto de rebotes → sin contribución directa (negro).
    if (depth <= 0) {
        return color{0.0, 0.0, 0.0};
    }

    const hit_opt h = intersect(r);
    if (!h) {
        // Fondo gradiente según d̂_y
        return background_color(r.direction);
    }

    // Dispersión en el material del punto (RNG compartido)
    const auto scatter = h->mat->scatter(r, *h, material_rng_);
    if (!scatter) {
        // Absorción total (no hay rayo saliente).
        return color{0.0, 0.0, 0.0};
    }

    const ray&   r_scattered = scatter->first;  // Or = I (lo fija scatter)
    const color& atten       = scatter->second; // atenuación por material

    // Contribución recursiva del camino restante
    // kLambdaMin evita autointersecciones en el siguiente rebote.
    return atten * trace(r_scattered, depth - 1);
}

} // namespace render

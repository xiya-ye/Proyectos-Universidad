#include "object.hpp" 
#include "ray.hpp"
#include "color.hpp"
#include "sampler.hpp"
#include <vector>
#include <cstdint>

namespace render {

class scene final {
public:
    // Umbral para ignorar auto-impactos y “acné”
    static constexpr double kLambdaMin = 1e-3;

    // Ctor: inyecta la semilla del RNG de materiales (defecto 13 del enunciado).
    explicit scene(std::uint64_t material_seed = 13) noexcept
      : material_rng_{material_seed} {}

    // ------------------------- Gestión de objetos (mínima) -------------------
    void add(const object* obj) {
        if (obj != nullptr) { objects_.push_back(obj); }
    }
    void clear() noexcept { objects_.clear(); }
    void reserve(std::size_t n) { objects_.reserve(n); }

    // --------------------------- Configuración -------------------------------
    // Colores del gradiente de fondo (claro/oscuro).
    void set_background(const color& dark, const color& light) noexcept {
        background_dark_  = dark;
        background_light_ = light;
    }

    // Reconfigura la semilla del RNG compartido de materiales
    void set_material_seed(std::uint64_t seed) noexcept {
        material_rng_ = sampler{seed};
    }

    // ------------------ Intersección y trazado (definidas en .cpp) -----------
    [[nodiscard]] hit_opt intersect(const ray& r) const noexcept;
    [[nodiscard]] color   trace(const ray& r, int depth) const noexcept;

private:
    // Fondo gradiente
    // c = (1 − m)·c_light + m·c_dark, con m = (d̂_y + 1)/2.
    [[nodiscard]] color background_color(const vector& dir) const noexcept {
        const vector unit = dir.normalize();
        const double m = 0.5 * (unit.y + 1.0);
        return background_light_ * (1.0 - m) + background_dark_ * m;
    }

    // ----------------------------- Atributos ---------------------------------
    // RNG de materiales (único y compartido)
    // mutable: se usa dentro de trace() marcado como const.
    mutable sampler material_rng_;

    // Objetos de la escena (no propietarios).
    std::vector<const object*> objects_;

    // Colores de fondo (valores por defecto acordes a la práctica).
    color background_dark_  {0.25, 0.5, 1.0};
    color background_light_ {1.0,  1.0, 1.0};
};

} // namespace render

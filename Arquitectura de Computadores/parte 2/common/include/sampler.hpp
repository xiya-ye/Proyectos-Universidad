#pragma once
// ============================================================================
// sampler.hpp — Generador de números aleatorios
//  • RNG: std::mt19937_64 con semilla externa (config)
//  • Jitter por píxel: δ ∈ [-0.5, 0.5]
//  • Materiales: mate (ξ ∈ [-1,1]), metal (ϕ ∈ [-Φ,Φ])
// ============================================================================

#include "vector.hpp"
#include <cstdint>
#include <random>

namespace render {

  // Pequeño envoltorio sobre <random> con utilidades a medida del enunciado.
  class sampler final {
  public:
    // Semilla externa para reproducibilidad (viene de la config).
    explicit sampler(std::uint64_t seed) noexcept
        : rng_{static_cast<std::mt19937_64::result_type>(seed)}, dist01_{0.0, 1.0} { }

    // Uniforme en [0,1)
    [[nodiscard]] double next01() noexcept { return dist01_(rng_); }

    // Uniforme en [-1,1)
    [[nodiscard]] double next_signed01() noexcept { return next01() * 2.0 - 1.0; }

    [[nodiscard]] double next_m11() noexcept { return next_signed01(); }  // alias práctico

    // Uniforme en [a,b)
    [[nodiscard]] double uniform(double a, double b) noexcept { return a + (b - a) * next01(); }

    // Jitter por píxel: δ ∈ [-0.5, 0.5] (para submuestreo de píxeles).
    [[nodiscard]] double jitter_half() noexcept { return uniform(-0.5, 0.5); }

    // Vector con componentes en [-1,1] (útil en mate/metal).
    [[nodiscard]] vector random_m11_vector() noexcept {
      return {next_m11(), next_m11(), next_m11()};
    }

  private:
    std::mt19937_64 rng_;                            // motor (Mersenne 64)
    std::uniform_real_distribution<double> dist01_;  // base [0,1)
  };

}  // namespace render

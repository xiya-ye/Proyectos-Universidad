#pragma once
// ======================================================================
// color.hpp — Color RGB lineal y utilidades
//  • El cómputo interno está en espacio lineal [0,1] (sin gamma).
//  • Al exportar (PPM P3): aplicar gamma y luego escalar a [0,255]
//    usando truncado
// ======================================================================

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <tuple>

namespace render {

// Representa un color RGB lineal.
// Nota: no se impone ningún invariante; usa clamp() cuando proceda.
struct color final {
    double r{};
    double g{};
    double b{};

    // --- Construcción --------------------------------------------------------
    constexpr color() noexcept = default;
    constexpr color(double r_, double g_, double b_) noexcept : r(r_), g(g_), b(b_) {}

    // --- Normalización a [0,1] ----------------------------------------------
    // Satura cada canal al rango válido para evitar valores fuera de gama.
    [[nodiscard]] constexpr color clamp() const noexcept {
        return {
            std::clamp(r, 0.0, 1.0),
            std::clamp(g, 0.0, 1.0),
            std::clamp(b, 0.0, 1.0)
        };
    }

    // --- Corrección gamma ----------------------------------------------------
    // Aplica c' = clamp(c)^(1/gamma). Se asume gamma > 0.
    // No es constexpr por el uso de std::pow.
    [[nodiscard]] color apply_gamma(double gamma) const noexcept {
        const color c = clamp();
        const double inv_gamma = 1.0 / gamma;
        return {
            std::pow(c.r, inv_gamma),
            std::pow(c.g, inv_gamma),
            std::pow(c.b, inv_gamma)
        };
    }

    // --- Conversión a 8-bit por canal ---------------------------------------
    // Devuelve (R,G,B) en [0,255] TRUNCANDO tras escalar. Útil para PPM P3.
    [[nodiscard]] constexpr std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>
    to_rgb255() const noexcept {
        const color c = clamp();
        return {
            static_cast<std::uint8_t>(c.r * 255.0),
            static_cast<std::uint8_t>(c.g * 255.0),
            static_cast<std::uint8_t>(c.b * 255.0)
        };
    }

    // --- Aritmética en espacio lineal ---------------------------------------
    // Suma por canal (acumulación de contribuciones).
    [[nodiscard]] constexpr color operator+(const color& rhs) const noexcept {
        return {r + rhs.r, g + rhs.g, b + rhs.b};
    }

    // Escalado uniforme (exposición/ponderación).
    [[nodiscard]] constexpr color operator*(double s) const noexcept {
        return {r * s, g * s, b * s};
    }

    // Producto Hadamard por canal — típico para albedo/reflectancia (§3.5).
    [[nodiscard]] constexpr color operator*(const color& rhs) const noexcept {
        return {r * rhs.r, g * rhs.g, b * rhs.b};
    }
};

// Escalar * color (soporta el orden habitual s * c).
[[nodiscard]] constexpr color operator*(double s, const color& c) noexcept {
    return {s * c.r, s * c.g, s * c.b};
}

} // namespace render

#pragma once
// ============================================================================
// image_soa.hpp — Imagen 2D en memoria SOA (Structure of Arrays)
//  • (fila 0, col 0) en la esquina superior-izquierda; Y crece hacia abajo.
//  • Recorrido row-major: idx = y * w + x.
//  • SOA: tres arrays contiguos R[], G[], B[] en espacio lineal [0,1].
//  • La corrección gamma y el escalado a [0,255] se aplican al exportar (PPM).
// ============================================================================

#include "color.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace render {

class image_soa final {
public:
    // Crea una imagen w×h en espacio lineal (sin gamma aplicada).
    // PRE: w>0, h>0. Lanza std::invalid_argument si no se cumple.
    explicit image_soa(int w, int h) {
        if (w <= 0 or h <= 0) {
            throw std::invalid_argument("image_soa: invalid size");
        }
        w_ = w;
        h_ = h;
        const std::size_t n = static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_);
        r_.resize(n);
        g_.resize(n);
        b_.resize(n);
    }

    // ------------------------------- Consultas --------------------------------
    [[nodiscard]] int  width()  const noexcept { return w_; }
    [[nodiscard]] int  height() const noexcept { return h_; }
    [[nodiscard]] std::size_t size() const noexcept {
        return static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_);
    }

    // -------- Acceso con comprobación de rango (lanza std::out_of_range) ------
    void set(int x, int y, const color& c) {
        const std::size_t i = index_(x, y);
        r_.at(i) = c.r;
        g_.at(i) = c.g;
        b_.at(i) = c.b;
    }

    [[nodiscard]] color get(int x, int y) const {
        const std::size_t i = index_(x, y);
        return color{ r_.at(i), g_.at(i), b_.at(i) };
    }

    // ---- Acceso rápido sin comprobación (PRE: coordenadas dentro de rango) ---
    void set_unchecked(int x, int y, const color& c) noexcept {
        const std::size_t i = static_cast<std::size_t>(y) * static_cast<std::size_t>(w_)
                            + static_cast<std::size_t>(x);
        r_[i] = c.r; g_[i] = c.g; b_[i] = c.b;
    }

    [[nodiscard]] color get_unchecked(int x, int y) const noexcept {
        const std::size_t i = static_cast<std::size_t>(y) * static_cast<std::size_t>(w_)
                            + static_cast<std::size_t>(x);
        return color{ r_[i], g_[i], b_[i] };
    }

    // -------------------------------- Utilidades ------------------------------
    // Rellena la imagen con 'c' (color lineal, sin corrección de gamma).
    void clear(const color& c) noexcept {
        std::ranges::fill(r_, c.r);
        std::ranges::fill(g_, c.g);
        std::ranges::fill(b_, c.b);
    }

    // Acceso directo a los canales (para kernels SOA o exportadores personalizados).
    [[nodiscard]] const std::vector<double>& r_channel() const noexcept { return r_; }
    [[nodiscard]] const std::vector<double>& g_channel() const noexcept { return g_; }
    [[nodiscard]] const std::vector<double>& b_channel() const noexcept { return b_; }
    [[nodiscard]]       std::vector<double>& r_channel()       noexcept { return r_; }
    [[nodiscard]]       std::vector<double>& g_channel()       noexcept { return g_; }
    [[nodiscard]]       std::vector<double>& b_channel()       noexcept { return b_; }

private:
    int w_{0};
    int h_{0};
    std::vector<double> r_, g_, b_;  // SOA: canales separados contiguos

    // Índice row-major. (0,0) es la esquina superior-izquierda.
    // Valida coordenadas y lanza std::out_of_range si están fuera.
    [[nodiscard]] std::size_t index_(int x, int y) const {
        if (x < 0 or y < 0 or x >= w_ or y >= h_) {
            throw std::out_of_range("image_soa: coordinates out of range");
        }
        return static_cast<std::size_t>(y) * static_cast<std::size_t>(w_)
             + static_cast<std::size_t>(x);
    }
};

} // namespace render

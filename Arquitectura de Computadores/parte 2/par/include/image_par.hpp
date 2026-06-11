#pragma once
// ============================================================================
// image_aos.hpp — Imagen 2D en memoria AOS (Array of Structs)
//  • (fila 0, col 0) en la esquina superior-izquierda; Y crece hacia abajo.
//  • Almacenamiento lineal row-major: idx = y * w + x.
//  • La imagen guarda color lineal [0,1]. La gamma (1/γ) y el escalado a
//    [0,255] con truncado se aplican al exportar (PPM P3).
// ============================================================================

#include "color.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace render {

  class image_aos final {
  public:
    // Crea una imagen w×h en espacio lineal (sin gamma aplicada).
    // PRE: w>0, h>0. Lanza std::invalid_argument si no se cumple.
    explicit image_aos(int w, int h) {
      if (w <= 0 or h <= 0) {
        throw std::invalid_argument("image_aos: invalid size");
      }
      w_ = w;
      h_ = h;
      // AOS (Array of Structs): cada píxel es un 'color' contiguo en memoria.
      px_.resize(static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_));
    }

    // ------------------------------- Consultas --------------------------------
    [[nodiscard]] int width() const noexcept { return w_; }

    [[nodiscard]] int height() const noexcept { return h_; }

    [[nodiscard]] std::size_t size() const noexcept {
      return static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_);
    }

    // -------- Acceso con comprobación de rango (lanza std::out_of_range) ------
    void set(int x, int y, color const & c) {
      px_.at(index_(x, y)) = c;  // at() valida el índice del vector
    }

    [[nodiscard]] color get(int x, int y) const { return px_.at(index_(x, y)); }

    // ---- Acceso rápido sin comprobación (PRE: coordenadas dentro de rango) ---
    void set_unchecked(int x, int y, color const & c) noexcept {
      px_[static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) +
          static_cast<std::size_t>(x)] = c;
    }

    [[nodiscard]] color get_unchecked(int x, int y) const noexcept {
      return px_[static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) +
                 static_cast<std::size_t>(x)];
    }

    // -------------------------------- Utilidades ------------------------------
    // Rellena la imagen con 'c' (color lineal, sin corrección de gamma).
    void clear(color const & c) noexcept { std::ranges::fill(px_, c); }

    // Acceso directo al buffer AOS (útil para exportadores o kernels AOS).
    [[nodiscard]] std::vector<color> const & pixels() const noexcept { return px_; }

    [[nodiscard]] std::vector<color> & pixels() noexcept { return px_; }

  private:
    int w_{0};
    int h_{0};
    std::vector<color> px_;

    // Índice row-major. (0,0) es la esquina superior-izquierda.
    // Valida coordenadas y lanza std::out_of_range si están fuera.
    [[nodiscard]] std::size_t index_(int x, int y) const {
      if (x < 0 or y < 0 or x >= w_ or y >= h_) {
        throw std::out_of_range("image_aos: coordinates out of range");
      }
      return static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) +
             static_cast<std::size_t>(x);
    }
  };

}  // namespace render

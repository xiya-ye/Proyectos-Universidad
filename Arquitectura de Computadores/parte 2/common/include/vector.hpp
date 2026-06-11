#pragma once
#include <cmath>

namespace render {

  /*
   * vector — utilidades 3D mínimas para geometría del trazador.
   * - Representa un vector/pto en R^3 con aritmética básica.
   * - Incluye norma, normalización, dot y cross.
   * - Diseño simple y constexpr/noexcept cuando procede.
   */
  struct vector {
    double x{};
    double y{};
    double z{};

    // --- Construcción --------------------------------------------------------
    // Por defecto deja el vector en (0,0,0).
    constexpr vector() noexcept = default;

    // Construcción directa con componentes.
    constexpr vector(double x_, double y_, double z_) noexcept : x(x_), y(y_), z(z_) { }

    // --- Magnitud y normalización -------------------------------------------
    // Norma al cuadrado: evita el coste de sqrt cuando solo se compara.
    [[nodiscard]] constexpr double length_squared() const noexcept { return x * x + y * y + z * z; }

    // Magnitud euclídea (||v||).
    [[nodiscard]] double magnitude() const noexcept { return std::sqrt(length_squared()); }

    // Normalizado: devuelve v/||v||. Si ||v|| es muy pequeño, devuelve (0,0,0)
    // para evitar divisiones numéricamente inestables (eps ~ 1e-12).
    [[nodiscard]] vector normalize(double eps = 1e-12) const noexcept {
      double const m = magnitude();
      return (m > eps) ? (*this / m) : vector{0.0, 0.0, 0.0};
    }

    // --- Aritmética básica ---------------------------------------------------
    // Operaciones que generan un nuevo vector (no modifican *this).
    [[nodiscard]] constexpr vector operator+(vector const & r) const noexcept {
      return {x + r.x, y + r.y, z + r.z};
    }

    [[nodiscard]] constexpr vector operator-(vector const & r) const noexcept {
      return {x - r.x, y - r.y, z - r.z};
    }

    [[nodiscard]] constexpr vector operator*(double s) const noexcept {
      return {x * s, y * s, z * s};
    }

    [[nodiscard]] constexpr vector operator/(double s) const noexcept {
      return {x / s, y / s, z / s};
    }

    // Operaciones en el propio objeto (más eficientes en bucles).
    constexpr vector & operator+=(vector const & r) noexcept {
      x += r.x;
      y += r.y;
      z += r.z;
      return *this;
    }

    constexpr vector & operator-=(vector const & r) noexcept {
      x -= r.x;
      y -= r.y;
      z -= r.z;
      return *this;
    }

    constexpr vector & operator*=(double s) noexcept {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    constexpr vector & operator/=(double s) noexcept {
      x /= s;
      y /= s;
      z /= s;
      return *this;
    }

    // --- Producto escalar y vectorial ----------------------------------------
    // Escalar (dot): mide proyección y sirve para ángulos/energía.
    [[nodiscard]] constexpr double dot(vector const & r) const noexcept {
      return x * r.x + y * r.y + z * r.z;
    }

    // Vectorial (cross): normal al plano (regla de la mano derecha).
    [[nodiscard]] constexpr vector cross(vector const & r) const noexcept {
      return {y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x};
    }

    // --- Negación ------------------------------------------------------------
    // Opuesto: (-x, -y, -z).
    [[nodiscard]] constexpr vector operator-() const noexcept { return {-x, -y, -z}; }
  };

  // Escalar * vector (orden canónico para permitir 2.0 * v).
  [[nodiscard]] constexpr vector operator*(double s, vector const & v) noexcept {
    return {v.x * s, v.y * s, v.z * s};
  }

}  // namespace render

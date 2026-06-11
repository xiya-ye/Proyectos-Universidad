#include "color.hpp"
#include <algorithm>
#include <cmath>
#include <ostream>

namespace render::ppm {

// Tamaño lógico de imagen PPM (ancho x alto).
struct image_size {
  int w{0}; // columnas (anchura)
  int h{0}; // filas (altura)
};

// Opciones del escritor (solo gamma).
struct writer_opts {
  double gamma{2.2}; // gamma por defecto 2.2
};

// ------------------------------ Cabecera P3 ---------------------------------
// Emite la cabecera del formato PPM P3 (ASCII).
inline void write_header(std::ostream& os, image_size sz) noexcept {
  os << "P3\n" << sz.w << ' ' << sz.h << "\n255\n";
}

// --------------------------- Utilidades internas ----------------------------
// Convierte un canal en [0,1] a entero [0,255] aplicando c' = c^(1/gamma)
// y TRUNCANDO (cast a int); se asume gamma>0 (con respaldo 2.2).
inline int to_u8(double v, double inv_gamma) noexcept {
  v = std::clamp(v, 0.0, 1.0);     // saturación a [0,1]
  v = std::pow(v, inv_gamma);      // corrección gamma
  return static_cast<int>(v * 255.0); // escalado + truncado
}

// --------------------------- Núcleo layout-agnóstico ------------------------
// Escribe píxeles usando un getter get(x,y) -> color, sin asumir layout.
template <class Getter>
inline void write_pixels_ascii(std::ostream& os,
                               image_size sz,
                               const Getter& get,
                               writer_opts opts = {}) {
  const double inv_gamma = (opts.gamma > 0.0) ? (1.0 / opts.gamma) : (1.0 / 2.2);
  for (int y = 0; y < sz.h; ++y) {           // filas de arriba a abajo
    for (int x = 0; x < sz.w; ++x) {         // columnas de izquierda a derecha
      const color c = get(x, y);
      const int ir = to_u8(c.r, inv_gamma);
      const int ig = to_u8(c.g, inv_gamma);
      const int ib = to_u8(c.b, inv_gamma);
      os << ir << ' ' << ig << ' ' << ib << '\n'; // un píxel por línea (PPM P3)
    }
  }
}

// ------------------------------ Overloads write -----------------------------
// 1) Imagen con width/height/get_unchecked (AOS/SOA).
template <class Image>
inline void write(std::ostream& os, const Image& img, writer_opts opts = {}) {
  const image_size sz{ img.width(), img.height() };
  write_header(os, sz);
  write_pixels_ascii(os, sz,
    [&img](int x, int y) { return img.get_unchecked(x, y); }, // acceso sin bounds-check
    opts
  );
}

// 2) Versión genérica: tamaño + getter explícito (ideal para SOA o buffers planos).
template <class Getter>
inline void write(std::ostream& os, image_size sz, const Getter& get, writer_opts opts = {}) {
  write_header(os, sz);
  write_pixels_ascii(os, sz, get, opts);
}

} // namespace render::ppm

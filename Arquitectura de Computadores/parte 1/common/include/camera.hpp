#pragma once
// ============================================================================
// camera.hpp — Cámara pinhole
//  • Construcción por pasos 1→7: v_f, d_f, hp, wp, base (u,v), p_h, p_v, O.
//  • Rayos primarios desde (f,c) con jitter δx,δy ∈ [-1/2, 1/2].
// ============================================================================

#include "vector.hpp"
#include "ray.hpp"
#include <cmath>
#include <numbers>

namespace render {

// ----------------------------- Parámetros de cámara --------------------------
struct camera_pose final {
    vector origin;   // P: posición del punto de vista
    vector look_at;  // D: punto destino
    vector up;       // n: “norte”
};

struct camera_optics final {
    double fov_deg{90.0};     // FOV vertical (grados)
    double aspect{16.0 / 9.0};
    int width{1920};
    int height{1080};

    constexpr camera_optics() noexcept = default;
    constexpr camera_optics(double fov, double asp) noexcept
        : fov_deg(fov), aspect(asp) {}

    // Si w,h > 0, la aspect efectiva es w/h
    constexpr camera_optics(double fov, int w, int h) noexcept
        : fov_deg(fov),
          aspect((h > 0) ? static_cast<double>(w) / static_cast<double>(h)
                         : (16.0 / 9.0)),
          width(w), height(h) {}
};

// ============================================================================
// camera — genera rayos primarios en coordenadas normalizadas o por píxel
// ============================================================================
class camera final {
public:
    // Construye la cámara y precomputa la ventana de proyección.
    camera(const camera_pose& pose, const camera_optics& opt) noexcept
        : origin_{pose.origin}, img_w_{opt.width}, img_h_{opt.height}
    {
        // (1) Vector focal v_f = P − D
        const vector vf = pose.origin - pose.look_at;
        // (2) Distancia focal d_f = ||v_f||
        const double df = vf.magnitude();
        // (3) Semialtura: hp/2 = tan(FOV/2)·d_f   (FOV en radianes)
        const double half_h = std::tan((opt.fov_deg * 0.5) * (std::numbers::pi / 180.0)) * df;
        // (4) Semianchura: wp/2 = (hp/2)·aspect
        const double half_w = half_h * opt.aspect;

        // (5) Base ortonormal (u, v) con forward apuntando hacia look_at
        const vector vf_hat = vf.normalize();
        forward_ = (-vf_hat);                         // de P hacia D
        const vector north = pose.up.normalize();     // n
        right_  = (forward_.cross(north)).normalize();// u = normalize(n × forward)
        up_     = (right_.cross(forward_)).normalize();// v = forward × u

        // (6) Vectores de ventana; eje Y crece hacia abajo
        ph_ = right_ * (2.0 * half_w);   // p_h (horizontal)
        pv_ = up_    * (-2.0 * half_h);  // p_v (vertical invertido)

        // (7) Origen de la ventana en el centro del píxel (0,0)
        const vector dx = ph_ / static_cast<double>(std::max(1, img_w_));
        const vector dy = pv_ / static_cast<double>(std::max(1, img_h_));
        origin_window_ = pose.origin - vf - 0.5 * (ph_ + pv_) + 0.5 * (dx + dy);

        // Incrementos por píxel
        delta_x_ = dx;
        delta_y_ = dy;
    }

    // ----------------------------- API normalizada ---------------------------
    // Q = O + u·p_h + v·p_v ; rayo = (P, Q − P)
    [[nodiscard]] ray primary_ray(double u, double v) const noexcept {
        const vector q = origin_window_ + ph_ * u + pv_ * v;
        return ray{origin_, q - origin_};
    }

    // ----------------------------- API por píxel + jitter --------
    // f=fila, c=columna; δx,δy ∈ [-0.5, 0.5] alrededor del centro del píxel.
    // Nota: O ya está centrado en el píxel (0,0) ⇒ no sumar +0.5 aquí.
    [[nodiscard]] ray primary_ray_from_pixel(int f, int c,
                                             double dx, double dy) const noexcept {
        const double u = (static_cast<double>(c) + dx) / static_cast<double>(img_w_);
        const double v = (static_cast<double>(f) + dy) / static_cast<double>(img_h_);
        return primary_ray(u, v);
    }

    // ----------------------------- Getters ----------------------------------
    [[nodiscard]] vector origin()  const noexcept { return origin_; }
    [[nodiscard]] vector forward() const noexcept { return forward_; }
    [[nodiscard]] vector right()   const noexcept { return right_; }
    [[nodiscard]] vector up()      const noexcept { return up_; }
    [[nodiscard]] int image_width()  const noexcept { return img_w_; }
    [[nodiscard]] int image_height() const noexcept { return img_h_; }

private:
    // Estado geométrico precomputado
    vector origin_;          // P
    vector forward_;         // dirección hacia D
    vector right_;           // u
    vector up_;              // v
    vector ph_;              // p_h (vector horizontal de ventana)
    vector pv_;              // p_v (vector vertical de ventana, Y hacia abajo)
    vector origin_window_;   // O (esquina superior-izquierda compensada a centro de píxel)
    vector delta_x_;         // Δx por píxel en horizontal
    vector delta_y_;         // Δy por píxel en vertical
    int img_w_{};
    int img_h_{};
};

} // namespace render

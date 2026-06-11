// common/src/config.cpp
// ============================================================================
// Implementación del parser de configuración
//  - El header scene/config.hpp no incluye <camera.hpp>; aquí sí (conv. tipos).
//  - Regla general de parsing:
//      * "clave: valores..." (una clave por línea).
//      * Último valor repetido prevalece.
//      * Errores con mensajes específicos: unknown key / invalid value / extra data.
//  - Validación final de rangos obligatorios.
// ============================================================================

#include "config.hpp"
#include "color.hpp"
#include "vector.hpp"
#include "camera.hpp"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <istream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace render {

namespace { // ----------------- helpers internos, no exportados -----------------

// Elimina espacios en blanco iniciales/finales in-place.
inline void trim_inplace(std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b and std::isspace(static_cast<unsigned char>(s[a])) != 0) { ++a; }
    while (b > a and std::isspace(static_cast<unsigned char>(s[b - 1])) != 0) { --b; }
    s.assign(s, a, b - a);
}

// Divide por espacios en tokens (sin comillas/escape; lo que requiere el enunciado).
inline std::vector<std::string> split_ws(const std::string& v) {
    std::istringstream iss(v);
    std::vector<std::string> out;
    std::string tok;
    while (iss >> tok) { out.push_back(tok); }
    return out;
}

// Lanzadores de errores con mensajes acordes
[[noreturn]] inline void throw_unknown_key(const std::string& raw_key_no_colon) {
    throw std::runtime_error("Error: Unknown configuration key: [" + raw_key_no_colon + ":]");
}
[[noreturn]] inline void throw_invalid_value(const std::string& key_with_colon,
                                             const std::string& whole_line) {
    std::string msg = "Error: Invalid value for key: [" + key_with_colon + "]\n";
    msg += "Line: \"" + whole_line + "\"";
    throw std::runtime_error(msg);
}
[[noreturn]] inline void throw_extra_data(const std::string& key_with_colon,
                                          const std::string& extra) {
    std::string msg = "Error: Extra data after configuration value for key: [" + key_with_colon + "]\n";
    msg += "Extra: \"" + extra + "\"";
    throw std::runtime_error(msg);
}

// Contexto mínimo para asociar errores a la clave/línea original.
struct ParseCtx {
    std::string key;
    std::string line;
};

// Verifica número de parámetros esperado y construye el texto "extra" si sobran.
inline void ensure_no_extra(const std::vector<std::string>& t, std::size_t expected,
                            const ParseCtx& ctx) {
    if (t.size() > expected) {
        std::ostringstream oss;
        for (std::size_t i = expected; i < t.size(); ++i) {
            if (i > expected) { oss << ' '; }
            oss << t[i];
        }
        throw_extra_data(ctx.key, oss.str());
    }
    if (t.size() < expected) {
        throw_invalid_value(ctx.key, ctx.line);
    }
}

// Parsers atómicos con comprobaciones básicas de formato/rango.
inline int parse_int(const std::string& s) {
    std::size_t idx = 0;
    long long tmp = 0;
    try {
        tmp = std::stoll(s, &idx);
        if (idx != s.size()) { throw std::invalid_argument("g"); }
    } catch (...) { throw std::invalid_argument("bad int"); }
    if (tmp < std::numeric_limits<int>::min() or tmp > std::numeric_limits<int>::max()) {
        throw std::out_of_range("range");
    }
    return static_cast<int>(tmp);
}
inline std::uint64_t parse_u64(const std::string& s) {
    std::size_t idx = 0;
    unsigned long long tmp = 0ULL;
    try {
        tmp = std::stoull(s, &idx);
        if (idx != s.size()) { throw std::invalid_argument("g"); }
    } catch (...) { throw std::invalid_argument("bad u64"); }
    return static_cast<std::uint64_t>(tmp);
}
inline double parse_double(const std::string& s) {
    std::size_t idx = 0;
    double v = 0.0;
    try {
        v = std::stod(s, &idx);
        if (idx != s.size()) { throw std::invalid_argument("g"); }
    } catch (...) { throw std::invalid_argument("bad double"); }
    return v;
}
inline vector parse_vec3(const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 3, ctx);
    vector v{};
    try {
        v.x = parse_double(t[0]);
        v.y = parse_double(t[1]);
        v.z = parse_double(t[2]);
    } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    return v;
}
inline color parse_color3(const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 3, ctx);
    color c{};
    try {
        c.r = parse_double(t[0]);
        c.g = parse_double(t[1]);
        c.b = parse_double(t[2]);
    } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    const bool out = (c.r < 0.0 or c.r > 1.0 or c.g < 0.0 or c.g > 1.0 or c.b < 0.0 or c.b > 1.0);
    if (out) { throw_invalid_value(ctx.key, ctx.line); }
    return c;
}

// Clasificación de claves soportadas (exact match con “:” final).
enum class KeyType {
    AspectRatio, ImageWidth, Gamma,
    CamPos, CamTarget, CamNorth, FOV,
    SPP, MaxDepth,
    SeedMat, SeedRay,
    BgDark, BgLight,
    Unknown
};
inline KeyType classify_key(const std::string& k) {
    static const std::unordered_map<std::string, KeyType> map{
        {"aspect_ratio:", KeyType::AspectRatio},
        {"image_width:",  KeyType::ImageWidth},
        {"gamma:",        KeyType::Gamma},
        {"camera_position:", KeyType::CamPos},
        {"camera_target:",   KeyType::CamTarget},
        {"camera_north:",    KeyType::CamNorth},
        {"field_of_view:",   KeyType::FOV},
        {"samples_per_pixel:", KeyType::SPP},
        {"max_depth:",      KeyType::MaxDepth},
        {"material_rng_seed:", KeyType::SeedMat},
        {"ray_rng_seed:",      KeyType::SeedRay},
        {"background_dark_color:",  KeyType::BgDark},
        {"background_light_color:", KeyType::BgLight}
    };
    auto it = map.find(k);
    return (it == map.end()) ? KeyType::Unknown : it->second;
}

// Handlers por clave (0–4 parámetros) con validación y saturación donde aplique.
inline void handle_aspect_ratio(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 2, ctx);
    int ax = 0, ay = 0;
    try { ax = parse_int(t[0]); ay = parse_int(t[1]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (ax <= 0 or ay <= 0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.aspect_x = ax; cfg.aspect_y = ay;
}
inline void handle_image_width(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 1, ctx);
    int w = 0; try { w = parse_int(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (w <= 0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.image_width = w;
}
inline void handle_gamma(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 1, ctx);
    double g = 0.0; try { g = parse_double(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (g <= 0.0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.gamma = g;
}
inline void handle_fov(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 1, ctx);
    double fov = 0.0; try { fov = parse_double(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (fov <= 0.0 or fov >= 180.0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.field_of_view = fov;
}
inline void handle_spp(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 1, ctx);
    int spp = 0; try { spp = parse_int(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (spp <= 0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.samples_per_pixel = spp;
}
inline void handle_max_depth(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx) {
    ensure_no_extra(t, 1, ctx);
    int md = 0; try { md = parse_int(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (md <= 0) { throw_invalid_value(ctx.key, ctx.line); }
    cfg.max_depth = md;
}
inline void handle_seed(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx, bool material) {
    ensure_no_extra(t, 1, ctx);
    std::uint64_t s = 0ULL; try { s = parse_u64(t[0]); } catch (...) { throw_invalid_value(ctx.key, ctx.line); }
    if (s == 0ULL) { throw_invalid_value(ctx.key, ctx.line); }
    if (material) { cfg.material_rng_seed = s; } else { cfg.ray_rng_seed = s; }
}
inline void handle_background(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx, bool dark) {
    const color c = parse_color3(t, ctx);
    if (dark) { cfg.background_dark = c; } else { cfg.background_light = c; }
}
inline void handle_camera_vec(render_config& cfg, const std::vector<std::string>& t, const ParseCtx& ctx, int which) {
    const vector v = parse_vec3(t, ctx);
    if      (which == 0) { cfg.camera_position = v; }
    else if (which == 1) { cfg.camera_target   = v; }
    else                 { cfg.camera_north    = v; }
}

// Dispatcher central: clasifica la clave y llama a su handler.
inline void apply_kv(render_config& cfg, const std::string& key_with_colon,
                     const std::string& raw_value, const std::string& whole_line) {
    const ParseCtx ctx{key_with_colon, whole_line};
    const auto tokens = split_ws(raw_value);
    switch (classify_key(key_with_colon)) {
        case KeyType::AspectRatio: handle_aspect_ratio(cfg, tokens, ctx); break;
        case KeyType::ImageWidth:  handle_image_width(cfg, tokens, ctx);  break;
        case KeyType::Gamma:       handle_gamma(cfg, tokens, ctx);        break;
        case KeyType::FOV:         handle_fov(cfg, tokens, ctx);          break;
        case KeyType::SPP:         handle_spp(cfg, tokens, ctx);          break;
        case KeyType::MaxDepth:    handle_max_depth(cfg, tokens, ctx);    break;
        case KeyType::SeedMat:     handle_seed(cfg, tokens, ctx, true);   break;
        case KeyType::SeedRay:     handle_seed(cfg, tokens, ctx, false);  break;
        case KeyType::BgDark:      handle_background(cfg, tokens, ctx, true);  break;
        case KeyType::BgLight:     handle_background(cfg, tokens, ctx, false); break;
        case KeyType::CamPos:      handle_camera_vec(cfg, tokens, ctx, 0);     break;
        case KeyType::CamTarget:   handle_camera_vec(cfg, tokens, ctx, 1);     break;
        case KeyType::CamNorth:    handle_camera_vec(cfg, tokens, ctx, 2);     break;
        default:
            throw_unknown_key(key_with_colon.substr(0, key_with_colon.size() - 1));
    }
}

// Parsing de una línea completa “clave: valor...”, con recorte y checks.
inline void parse_line(render_config& cfg, const std::string& original, std::string line) {
    trim_inplace(line);
    if (line.empty()) { return; } // líneas en blanco: ignorar

    const auto pos = line.find(':');
    if (pos == std::string::npos) {
        std::string first = line;
        if (auto sp = first.find_first_of(" \t"); sp != std::string::npos) {
            first = first.substr(0, sp);
        }
        throw_unknown_key(first);
    }

    std::string key   = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    trim_inplace(key);
    trim_inplace(value);

    if (key.empty() or key.find_first_of(" \t") != std::string::npos) {
        throw_unknown_key(key);
    }
    const std::string key_with_colon = key + ":";
    if (value.empty()) {
        throw_invalid_value(key_with_colon, original);
    }
    apply_kv(cfg, key_with_colon, value, original);
}

// Comprobaciones finales de rangos/coherencia (tras leer todo el archivo).
inline void validate_final(const render_config& cfg) {
    if (cfg.image_width <= 0 or cfg.aspect_x <= 0 or cfg.aspect_y <= 0) {
        throw_invalid_value("image_width:", "image_width/aspect_ratio invalid");
    }
    if (cfg.field_of_view <= 0.0 or cfg.field_of_view >= 180.0) {
        throw_invalid_value("field_of_view:", "field_of_view: out of range");
    }
    if (cfg.gamma <= 0.0) {
        throw_invalid_value("gamma:", "gamma: out of range");
    }
    if (cfg.samples_per_pixel <= 0) {
        throw_invalid_value("samples_per_pixel:", "samples_per_pixel: out of range");
    }
    if (cfg.max_depth <= 0) {
        throw_invalid_value("max_depth:", "max_depth: out of range");
    }
    if (cfg.material_rng_seed == 0ULL) {
        throw_invalid_value("material_rng_seed:", "material_rng_seed: out of range");
    }
    if (cfg.ray_rng_seed == 0ULL) {
        throw_invalid_value("ray_rng_seed:", "ray_rng_seed: out of range");
    }
}

} // namespace (anónimo)

// ------------------------------ API pública ---------------------------------

render_config render_config::parse(std::istream& is) {
    // Aplica defaults y luego sobreescribe según el fichero (último valor gana).
    render_config cfg{};
    std::string line;
    while (std::getline(is, line)) {
        const std::string& original = line; // conserva la línea para errores
        parse_line(cfg, original, line);
    }
    validate_final(cfg);
    return cfg;
}

// Conversión a tipos de cámara (definidas aquí para no contaminar el .hpp).
camera_pose render_config::to_camera_pose() const noexcept {
    return camera_pose{camera_position, camera_target, camera_north};
}
camera_optics render_config::to_camera_optics() const noexcept {
    return camera_optics{field_of_view, image_width, image_height()};
}

} // namespace render

namespace render { // ctor que carga desde ruta (I/O y delega en parse)

render_config::render_config(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Error: cannot open configuration file: " + path);
    }
    *this = parse(file);
}

} // namespace render

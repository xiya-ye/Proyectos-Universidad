#pragma once
// ============================================================================
// render_aos.hpp — Lógica principal de renderizado (versión AOS)
//  • Carga configuración y escena, construye cámara, traza y vuelca PPM.
//  • Esta cabecera solo declara la API; la implementación vive en .cpp.
// ============================================================================

#include <string>

namespace render {

/**
 * @brief Ejecuta el renderizado completo en formato AOS.
 * @param config_path Ruta al archivo de configuración.
 * @param scene_path  Ruta al archivo de escena.
 * @param output_path Ruta del fichero PPM de salida.
 *
 * Flujo esperado (implementación en .cpp):
 *  1) Parsear config y escena.
 *  2) Construir cámara según la config.
 *  3) Trazar la imagen en AOS (row-major, colores en [0,1]).
 *  4) Exportar a PPM P3 aplicando gamma y truncado a [0,255].
 */
void render_aos(const std::string& config_path,
                const std::string& scene_path,
                const std::string& output_path);

} // namespace render

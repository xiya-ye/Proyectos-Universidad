#pragma once
// ============================================================================
// render_soa.hpp — Lógica principal de renderizado (versión SOA)
//  • Carga configuración y escena, prepara cámara, traza y escribe PPM.
//  • Solo declara la API pública; la implementación está en el .cpp.
// ============================================================================
#include <string>

namespace render {

/**
 * Ejecuta el renderizado completo en formato SOA (Structure of Arrays).
 *
 * @param config_path Ruta al archivo de configuración.
 * @param scene_path  Ruta al archivo de escena.
 * @param output_path Ruta del PPM de salida.
 */
void render_soa(const std::string& config_path,
                const std::string& scene_path,
                const std::string& output_path);

} // namespace render

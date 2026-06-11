// aos/src/main.cpp
// ============================================================================
// render-aos — Punto de entrada (AOS)
//  • Argumentos (en orden): <config.txt> <scene.txt> <output.ppm>
//  • Si argc-1 != 3 → imprimir exactamente:
//      "Error: Invalid number of arguments: N"
//    y terminar con código de error (sin línea de uso).
//  • El resto (cargar cfg/escena, trazar y escribir PPM) se delega a render_aos(...).
// ============================================================================

#include "render_aos.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>  
#include <span> 
#include <string>

using namespace render;

int main(int argc, char** argv) {
    try {
        // Deben recibirse exactamente 3 argumentos de usuario.
        // "Error: Invalid number of arguments: N".
        const int n_args = argc - 1;
        if (n_args != 3) {
            std::cerr << "Error: Invalid number of arguments: " << n_args << '\n';
            return EXIT_FAILURE;
        }

        // argv[1]=config, argv[2]=scene, argv[3]=output
        const std::span<char*> args(argv, static_cast<std::size_t>(argc));
        const std::string config_path(args[1]);
        const std::string scene_path(args[2]);
        const std::string output_path(args[3]);

        // Render AOS: carga configuración/escena, traza y guarda PPM (P3).
        render_aos(config_path, scene_path, output_path);

        // Mensaje informativo opcional (a stderr).
        std::cerr << "Imagen guardada en: " << output_path << '\n';
        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        // Errores → stderr y código de salida con fallo.
        std::cerr << "Excepción: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

// soa/src/main.cpp
// ============================================================================
// render-soa — Punto de entrada (SOA)
//  • Argumentos (en orden): <config.txt> <scene.txt> <output.ppm>
//  • Si argc-1 != 3: imprimir exactamente
//      "Error: Invalid number of arguments: N"
//    y salir con error (sin línea de uso).
//  • El render real se delega a render_soa(...).
// ============================================================================

#include "render_soa.hpp"

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
        // Mensaje literal exigido por el enunciado.
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

        // Render SOA: carga config/escena, traza y guarda PPM (P3).
        render_soa(config_path, scene_path, output_path);

        // Mensaje informativo opcional (por stderr para no mezclar con stdout).
        std::cerr << "Imagen guardada en: " << output_path << '\n';
        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        // Cualquier excepción se reporta por stderr y se sale con fallo.
        std::cerr << "Excepción: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

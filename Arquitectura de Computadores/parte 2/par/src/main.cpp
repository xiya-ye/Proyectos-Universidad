#include "render_par.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <string>

// TBB
#include <oneapi/tbb/global_control.h>

#include <tbb/tbb.h>

int main(int argc, char * argv[]) {
  try {
    // Usamos std::span para envolver argv de forma segura.
    // Esto evita la aritmética de punteros directa (argv[i]) que molesta a clang-tidy.
    auto const args = std::span(argv, static_cast<std::size_t>(argc));

    // Validamos el número de argumentos.
    // Esperamos exactamente 4: ejecutable + config + scene + output
    if (args.size() != 4) {
      // Usamos args[0] en lugar de argv[0] para mantener la seguridad
      std::cerr << "Usage: " << args[0] << " <config.txt> <scene.txt> <output.ppm>\n";
      return EXIT_FAILURE;
    }

    // Número de hilos
    int const n_threads = 256;

    // Forzamos a TBB a usar este número de hilos ignorando los argumentos
    std::unique_ptr<tbb::global_control> const gc = std::make_unique<tbb::global_control>(
        tbb::global_control::max_allowed_parallelism, n_threads);
    std::cout << "Running with " << n_threads << " threads.\n";

    // Llamada al render paralelo
    // Convertimos los char* a std::string explícitamente para mayor claridad
    render::render_par(std::string(args[1]), std::string(args[2]), std::string(args[3]));

    return EXIT_SUCCESS;

  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}

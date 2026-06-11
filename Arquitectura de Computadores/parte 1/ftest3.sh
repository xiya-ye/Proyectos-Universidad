#!/bin/bash
# Crea la imagen de prueba 3, tanto con render-aos como con render-soa
# Adapatado para ejecución en Avignon

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> RENDER-AOS: Creando imagen de prueba 3..."
out/build/clang-tidy/aos/Release/render-aos config3.txt scene3.txt out3-aos.ppm

echo "==> RENDER-SOA: Creando imagen de prueba 3..."
out/build/clang-tidy/soa/Release/render-soa config3.txt scene3.txt out3-soa.ppm

echo "Imágenes creadas con éxito"

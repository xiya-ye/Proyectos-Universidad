#!/bin/bash
# Lanza todas las pruebas unitarias (utest-common, utest-soa, utest-aos)
# Adapatado para ejecución en Avignon

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> Ejecutando pruebas de utcommon..."
out/build/clang-tidy/utcommon/Release/utest-common

echo "==> Ejecutando pruebas de utaos..."
out/build/clang-tidy/utaos/Release/utest-aos

echo "==> Ejecutando pruebas de utsoa..."
out/build/clang-tidy/utsoa/Release/utest-soa

echo "==> ¡Pruebas unitarias completadas!"

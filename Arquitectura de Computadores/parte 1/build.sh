#!/bin/bash
# Crea y compila las diferentes builds del proyecto
# Adapatado para ejecución en Avignon

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> Creando directorio "default""
cmake --preset default

echo "==> Compilando con preset "gcc-debug""
cmake --build --preset gcc-debug --config Debug --target all --parallel

echo "==> Compilando con preset "gcc-release""
cmake --build --preset gcc-release --config Release --target all --parallel

echo "==> Creando directorio "clang-tidy""
cmake --preset clang-tidy

echo "==> Compilando con preset "clang-tidy-debug""
cmake --build --preset clang-tidy-debug --config Debug --target all --parallel

echo "==> Compilando con preset "clang-tidy-release""
cmake --build --preset clang-tidy-release --config Release --target all --parallel

echo "==> Builds creadas con éxito"
#!/bin/bash
# Crea la imagen de prueba 5 con render-par
# Adapatado para ejecución en Avignon
#SBATCH --job-name=render-run
#SBATCH --output=./logs/run-stan-%j.txt
#SBATCH --partition=stan
#SBATCH --exclusive

set -Eeuo pipefail
mkdir -p logs
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> RENDER-PAR: Creando imagen de prueba 5..."
out/build/clang-tidy/par/Release/render-par config5.txt scene5.txt out5-par.ppm

echo "Imágenes creadas con éxito"

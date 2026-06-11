#!/bin/bash
# Crea la imagen de prueba 1 con render-par
# Adapatado para ejecución en Avignon
#SBATCH --job-name=render-run
#SBATCH --output=./logs/run-stan-%j.txt
#SBATCH --partition=stan
#SBATCH --exclusive

set -Eeuo pipefail
mkdir -p logs
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> RENDER-PAR: Creando imagen de prueba 1..."
out/build/clang-tidy/par/Release/render-par config1.txt scene1.txt out1-par.ppm

echo "Imágenes creadas con éxito"

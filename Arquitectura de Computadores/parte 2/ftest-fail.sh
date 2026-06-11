#!/bin/bash
# Test para comprobar el correcto tratamiento de errores a la hora de lanzar los render
# Adapatado para ejecución en Avignon

# Sin set -Eeuo pipefail para evitar que el script pare tras el primer fallo
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "==> PRUEBA-AOS: faltan argumentos"
echo "==> out/build/clang-tidy/aos/Release/render-aos"
out/build/clang-tidy/aos/Release/render-aos

echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt"
out/build/clang-tidy/aos/Release/render-aos config1.txt

echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt"
out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt

echo "==> PRUEBA-AOS: sobran argumentos"
echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt out1-aos.ppm prueba.txt"
out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt out1-aos.ppm prueba.txt

echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt out1-aos.ppm prueba.txt holamundo.txt"
out/build/clang-tidy/aos/Release/render-aos config1.txt scene1.txt out1-aos.ppm prueba.txt holamundo.txt

echo "==> PRUEBA-AOS: etiqueta de config desconocida"
echo "==> out/build/clang-tidy/aos/Release/render-aos cfg.txt scene1.txt out1-aos.ppm"
out/build/clang-tidy/aos/Release/render-aos cfg.txt scene1.txt out1-aos.ppm

echo "==> out/build/clang-tidy/aos/Release/render-aos config-fail.txt scene1.txt out1-aos.ppm"
out/build/clang-tidy/aos/Release/render-aos config-fail.txt scene1.txt out1-aos.ppm

echo "==> PRUEBA-AOS: etiqueta de scene desconocida"
echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt scn.txt out1-aos.ppm"
out/build/clang-tidy/aos/Release/render-aos config1.txt scn.txt out1-aos.ppm

echo "==> out/build/clang-tidy/aos/Release/render-aos config1.txt scene-fail.txt out1-aos.ppm"
out/build/clang-tidy/aos/Release/render-aos config1.txt scene-fail.txt out1-aos.ppm

echo "==> PRUEBA-SOA: faltan argumentos"
echo "==> out/build/clang-tidy/soa/Release/render-soa"
out/build/clang-tidy/soa/Release/render-soa

echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt"
out/build/clang-tidy/soa/Release/render-soa config1.txt

echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt"
out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt

echo "==> PRUEBA-SOA: sobran argumentos"
echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt out1-soa.ppm prueba.txt"
out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt out1-soa.ppm prueba.txt

echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt out1-soa.ppm prueba.txt holamundo.txt"
out/build/clang-tidy/soa/Release/render-soa config1.txt scene1.txt out1-soa.ppm prueba.txt holamundo.txt

echo "==> PRUEBA-SOA: etiqueta de config desconocida"
echo "==> out/build/clang-tidy/soa/Release/render-soa cfg.txt scene1.txt out1-soa.ppm"
out/build/clang-tidy/soa/Release/render-soa cfg.txt scene1.txt out1-soa.ppm

echo "==> out/build/clang-tidy/soa/Release/render-soa config-fail.txt scene1.txt out1-soa.ppm"
out/build/clang-tidy/soa/Release/render-soa config-fail.txt scene1.txt out1-soa.ppm

echo "==> PRUEBA-SOA: etiqueta de scene desconocida"
echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt scn.txt out1-soa.ppm"
out/build/clang-tidy/soa/Release/render-soa config1.txt scn.txt out1-soa.ppm

echo "==> out/build/clang-tidy/soa/Release/render-soa config1.txt scene-fail.txt out1-soa.ppm"
out/build/clang-tidy/soa/Release/render-soa config1.txt scene-fail.txt out1-soa.ppm

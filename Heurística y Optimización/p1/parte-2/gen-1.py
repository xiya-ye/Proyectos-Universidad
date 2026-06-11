"""
Uso: ./gen-1.py fichero_entrada.in fichero_salida.dat
"""
#!/usr/bin/env python3
import re
import subprocess
import sys
import os
import tempfile

fd_mod = 'parte-2-1.mod'

def ruta_absoluta(nombre, carpeta_script):
    """Este metodo devuelve la ruta abs del archivo"""
    if os.path.dirname(nombre) == "":
        return os.path.abspath(os.path.join(carpeta_script, nombre))
    return os.path.abspath(nombre)

def leer_entrada(fichero):
    """Este metodo lee el fichero de entrada .in y devuelve los datos del
    problema"""
    # Leemos el fichero de entrada
    with open(fichero, "r", encoding="utf-8") as f:
        lineas = []
        for linea in f:
            linea = linea.strip()
            if linea != "" and not linea.startswith("#"):
                lineas.extend(linea.split())

    if len(lineas) < 6:
        raise ValueError("El fichero de entrada no tiene suficientes datos.")

    # Asignamos los datos correspondientes por separado
    n = int(lineas[0])
    m = int(lineas[1])
    k_d = float(lineas[2])
    k_p = float(lineas[3])

    d = []
    for i in range(m):
        d.append(float(lineas[4 + i]))

    p = []
    for i in range(m):
        p.append(float(lineas[4 + m + i]))

    return n, m, k_d, k_p, d, p

def escribir_dat(fichero, n, m, k_d, k_p, d, p):
    """Este metodo escribe en el fichero de salida .dat los datos que se han
    leido del fichero de entrada"""
    if os.path.dirname(fichero) != "":
        os.makedirs(os.path.dirname(fichero), exist_ok=True)

    with open(fichero, "w", encoding="utf-8") as f:
        f.write("data;\n\n")

        f.write("set AUTOBUSES := ")
        for i in range(m):
            f.write("a" + str(i+1) + " ")
        f.write(";\n")

        f.write("set FRANJAS := ")
        for j in range(n):
            f.write("s" + str(j+1) + " ")
        f.write(";\n\n")

        f.write("param Distancia :=\n")
        for i in range(m):
            if i == m - 1:
                f.write("   a" + str(i + 1) + " " + str(d[i]) + ";\n\n")
            else:
                f.write("   a" + str(i+1) + " " + str(d[i]) + "\n")

        f.write("param Pasajeros :=\n")
        for i in range(m):
            if i == m - 1:
                f.write("   a" + str(i + 1) + " " + str(p[i]) + ";\n\n")
            else:
                f.write("   a" + str(i+1) + " " + str(p[i]) + "\n")

        f.write("param k_d := " + str(k_d) + ";\n")
        f.write("param k_p := " + str(k_p) + ";\n")
        f.write("end;\n")

def ejecutar_glpk(mod, datos):
    """Este metodo ejecuta GLPK con el modelo y el fichero .dat dado"""
    # Creamos un directorio temporal para ejecutar glpsol ahi
    with tempfile.TemporaryDirectory() as tmpdir:
        salida = os.path.join(tmpdir, "salida.txt")
        comando = ["glpsol", "-m", mod, "-d", datos, "-o", salida]

        resultado = subprocess.run(comando, capture_output=True, text=True)

        if resultado.returncode != 0:
            print(resultado.stdout)
            print(resultado.stderr, file=sys.stderr)
            raise RuntimeError("Error al ejecutar glpsol")

        # Leemos el resultado generado por glpsol sin tener en cuenta los
        # errores
        texto_completo = resultado.stdout + "\n" + "=" * 50 + "\n"
        with open(salida, "r", encoding="utf-8", errors="ignore") as f:
            texto_completo += f.read()
    return texto_completo

def leer_resultados(texto, n, m):
    """Este metodo busca el valor de la funcion objetivo y las asignaciones"""
    obj = None
    patrones = [
        r"Coste_real\s*=\s*([\-0-9.eE]+)",
        r"COST\.val\s*=\s*([\-0-9.eE]+)"
    ]

    for patron in patrones:
        encontrado = re.search(patron, texto)
        if encontrado:
            obj = float(encontrado.group(1))
            break

    if obj is None:
        raise RuntimeError("No se pudo leer el valor del coste total")

    asignaciones = {}
    # Buscamos las asignaciones en stdout del modelo
    patron_asign = re.compile(r"Bus\s+(a\d+)\s+asignado\s+a\s+franja\s+(s\d+)")

    for linea in texto.splitlines():
        m2 = patron_asign.search(linea.strip())
        if m2:
            asignaciones[m2.group(1)] = m2.group(2)

    num_vars = n * m
    num_restr = n + m

    return obj, asignaciones, num_vars, num_restr

def mostrar_resultado(obj, asignaciones, num_vars, num_restr, m):
    """Este metodo imprime el resultado por pantalla"""
    print("Impacto de averías:", obj)
    print("Número de variables:", num_vars)
    print("Número de restricciones:", num_restr)

    for i in range(1, m+1):
        nombre = "a" + str(i)
        if nombre in asignaciones:
            print("Bus " + nombre + " asignado a franja " + asignaciones[
                nombre])
        else:
            print("Bus " + nombre + " sin asignar")


def main():
    if len(sys.argv) != 3:
        print("Uso: ./gen-2.py fichero_entrada fichero_salida\n")
        sys.exit(-1)

    fd_ent = sys.argv[1]
    fd_sal = sys.argv[2]

    # Cargamos la carpeta y las rutas de los ficheros
    carpeta = os.path.abspath(os.path.dirname(__file__))
    entrada = ruta_absoluta(fd_ent, carpeta)
    salida = ruta_absoluta(fd_sal, carpeta)
    mod = os.path.join(carpeta, fd_mod)

    # Leemos el fich de entrada, generamos el fich de salida y resolvemos
    # con GLPK
    n, m, k_d, k_p, d, p = leer_entrada(entrada)
    escribir_dat(salida, n, m, k_d, k_p, d, p)
    texto_salida = ejecutar_glpk(mod, salida)
    obj, asignaciones, vars_totales, restr_totales = leer_resultados(
        texto_salida, n, m)
    mostrar_resultado(obj, asignaciones, vars_totales, restr_totales, m)




if __name__ == '__main__':
    main()
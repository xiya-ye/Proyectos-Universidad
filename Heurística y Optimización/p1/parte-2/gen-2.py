"""
Uso: ./gen-2.py fichero_entrada.in fichero_salida.dat
"""
#!/usr/bin/env python3
import re
import subprocess
import sys
import os
import tempfile

fd_mod = 'parte-2-2.mod'

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
    # Número min de valores en el fichero de entrada
    if len(lineas) < 6:
        raise ValueError("El fichero de entrada no tiene suficientes datos.")

    # Asignamos las dimensiones de los conjuntos
    n = int(lineas[0])  # franjas
    m = int(lineas[1])  # autobuses
    u = int(lineas[2])  # talleres

    # Leemos la matriz c_ij (mxm)
    c_ij = []
    idx = 3
    for _ in range(m):
        fila = []
        for j in range(m):
            fila.append(int(lineas[idx]))
            idx += 1
        c_ij.append(fila)


    # Leemos la matriz o_ij (nxu)
    o_ij = []
    for _ in range(n):
        fila = []
        for j in range(u):
            fila.append(int(lineas[idx]))
            idx += 1
        o_ij.append(fila)

    return n, m, u, c_ij, o_ij


def escribir_dat(fichero, n, m, u, c_ij, o_ij):
    """Este metodo escribe en el fichero de salida .dat los datos que se han
    leido del fichero de entrada"""
    if os.path.dirname(fichero) != "":
        os.makedirs(os.path.dirname(fichero), exist_ok=True)

    with open(fichero, "w", encoding="utf-8") as f:
        f.write("data;\n\n")

        # Sets
        f.write("set TALLERES := ")
        for i in range(u):
            f.write("t" + str(i+1) + " ")
        f.write(";\n")

        f.write("set AUTOBUSES := ")
        for i in range(m):
            f.write("a" + str(i+1) + " ")
        f.write(";\n\n")

        f.write("set FRANJAS := ")
        for i in range(n):
            f.write("s" + str(i+1) + " ")
        f.write(";\n")

        # Param c[i, j]
        f.write("param num_pasajeros_misma_franja:\n")
        for j in range(m):
            f.write("   " + "a" + str(j+1))
        f.write(" :=\n")
        for i in range(m):
            f.write("a" + str(i+1))
            for j in range(m):
                if i == (m-1) and j == (m-1):
                    f.write("   " + str(c_ij[i][j]) + ";")
                else:
                    f.write("   " + str(c_ij[i][j]))
            f.write("\n")
        f.write("\n")

        # Param o[i, j]
        f.write("param disponibilidad_franja:\n")
        for j in range(u):
            f.write("   " + "t" + str(j+1))
        f.write(" :=\n")
        for i in range(n):
            f.write("s" + str(i+1))
            for j in range(u):
                if i == (n-1) and j == (u-1):
                    f.write("   " + str(o_ij[i][j]) + ";")
                else:
                    f.write("   " + str(o_ij[i][j]))
            f.write("\n")
        f.write("\nend;\n")

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

def leer_resultados(texto, n, m, u):
    """Este metodo busca el valor de la funcion objetivo y las asignaciones"""
    obj = None
    patron_obj = re.search(r"Impacto_total\s*=\s*([\-0-9.eE]+)", texto)

    if patron_obj:
        obj = float(patron_obj.group(1))
    else:
        raise RuntimeError("No se encontró el valor del Impacto total")

    asignaciones = []
    # Buscamos las asignaciones en stdout del modelo
    patron_asign = re.compile(r"Bus\s+(a\d+)\s+->\s+Taller\s+(t\d+)\s+Franja\s+(s\d+)")

    for linea in texto.splitlines():
        m2 = patron_asign.search(linea.strip())
        if m2:
            asignaciones.append((m2.group(1), m2.group(2), m2.group(3)))

    comb_m2 = m * (m - 1) // 2
    num_vars = m * n * u + comb_m2 * n
    num_restr = m + n * u + 3 * comb_m2 * n

    return obj, asignaciones, num_vars, num_restr


def mostrar_resultado(obj, asignaciones, num_vars, num_restr, m):
    """Este metodo imprime el resultado por pantalla"""
    print("Impacto sobre los clientes:", obj)
    print("Número de variables:", num_vars)
    print("Número de restricciones:", num_restr)

    asignaciones_dict = {bus: (taller, franja) for bus, taller, franja in asignaciones}

    for bus, taller, franja in asignaciones:
        print("Bus", bus, "-> Taller", taller, "Franja", franja)

    for i in range(1, m+1):
        nombre = "a" + str(i)
        if nombre in asignaciones_dict:
            taller, franja = asignaciones_dict[nombre]
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
    n, m, u, c_ij, o_ij = leer_entrada(entrada)
    escribir_dat(salida, n, m, u, c_ij, o_ij)
    texto_salida = ejecutar_glpk(mod, salida)
    obj, asignaciones, vars_totales, restr_totales = leer_resultados(
        texto_salida, n, m, u)
    mostrar_resultado(obj, asignaciones, vars_totales, restr_totales, m)




if __name__ == '__main__':
    main()
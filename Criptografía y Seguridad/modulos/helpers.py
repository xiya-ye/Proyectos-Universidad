"helpers.py"
import json
import os

class JsonStore:
    """Clase que vamos a usar para el manejo de entrada/salida de datos."""
    # -------------------- CARGAR/GUARDAR JSON --------------------
    @staticmethod
    def cargar_json(ruta):
        """carga de datos desde un archivo JSON"""
        if not os.path.exists(ruta):
            return []
        try:
            with open(ruta, "r", encoding="utf-8") as f:
                return json.load(f)
        except json.JSONDecodeError:
            return []

    @staticmethod
    def guardar_json(ruta, datos):
        """guarda los datos en un archivo JSON con formato legible"""
        directorio = os.path.dirname(ruta)
        if directorio and not os.path.exists(directorio):
            os.makedirs(directorio, exist_ok=True)
        with open(ruta, "w", encoding="utf-8") as f:
            json.dump(datos, f, indent=4, ensure_ascii=False)
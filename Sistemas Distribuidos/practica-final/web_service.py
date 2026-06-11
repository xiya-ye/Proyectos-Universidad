import logging
from flask import Flask, request

app = Flask(__name__)

@app.route('/remove_nwhitespaces', methods=['POST'])
def remove_nwhitespaces():
    """
    Elimina los espacios en blanco repetidos de un mensaje
    """
    try:
        #Obtener el JSON de la peticion
        req = request.get_json()

        message = req['message'].split()
        normalized = ' '.join(message)
        return normalized, 202
    except Exception as e:
        return {"error": str(e)}, 415

if __name__ == '__main__':
    app.run(debug=False, host='127.0.0.1', port=8000)
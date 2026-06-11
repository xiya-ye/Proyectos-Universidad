class Consejo {
    constructor(titulo, descripcion, usuario, fecha) {
        this.titulo = titulo;
        this.descripcion = descripcion;
        this.usuario = usuario;
        this.fecha = fecha || new Date().toISOString();
    }
}

function guardarConsejo(consejo) {
    const consejos = JSON.parse(localStorage.getItem('consejos') || '[]');
    consejos.push(consejo);
    localStorage.setItem('consejos', JSON.stringify(consejos));
}

function cargarConsejos() {
    return JSON.parse(localStorage.getItem('consejos') || '[]');
}

function obtenerUltimosConsejos(cantidad = 3) {
    const consejos = cargarConsejos();
    // Ordenar por fecha (mas recientes primero)
    consejos.sort((a, b) => new Date(b.fecha) - new Date(a.fecha));
    return consejos.slice(0, cantidad);
}

//Nueva funcion para validar consejo
function validarConsejo(titulo, descripcion) {
    const errores = [];

    if (titulo.length < 15) {
        errores.push("El título debe tener al menos 15 caracteres.");
    }

    if (descripcion.length < 30) {
        errores.push("La descripción debe tener al menos 30 caracteres.");
    }

    return errores;
}

export { Consejo, guardarConsejo, cargarConsejos, obtenerUltimosConsejos, validarConsejo};


// Elementos del DOM
const inputOrigen = document.getElementById('input-origen');
const inputDestino = document.getElementById('input-destino');
const inputFechaIda = document.getElementById('input-fecha-ida');
const inputFechaVuelta = document.getElementById('input-fecha-vuelta');
const selectClase = document.getElementById('select-clase');
const selectPersonas = document.getElementById('select-personas');
const btnContinuar = document.getElementById('btn-continuar');
const travelForm = document.querySelector('.travel-form');

// Elementos del resumen
const resumenOrigen = document.querySelector('.summary-list li:nth-child(1) strong');
const resumenDestino = document.querySelector('.summary-list li:nth-child(2) strong');
const resumenFechas = document.querySelector('.summary-list li:nth-child(3) strong');
const resumenPersonas = document.querySelector('.summary-list li:nth-child(4) strong');
const totalPrice = document.querySelector('.total-price');

// Precios base por tipo de viaje
const preciosBase = {
    turista: 100,
    business: 200,
    primera: 400
};

// --- Helpers de idioma / traducción para este JS ---

function getCurrentLang() {
    const active = document.querySelector('.language-selector .lang-option.active');
    return active ? active.dataset.lang || 'es' : 'es';
}

function t(key) {
    if (typeof translations === 'undefined') return '';
    const lang = getCurrentLang();
    const langPack = translations[lang] || translations.es || {};
    return langPack[key] || (translations.es && translations.es[key]) || '';
}

// Evitar que el formulario se envíe al pulsar Enter
if (travelForm) {
    travelForm.addEventListener('submit', (event) => {
        event.preventDefault();
    });
}

// Cargar datos guardados al inicio
window.addEventListener('DOMContentLoaded', cargarDatos);

// Event listeners para actualizar en tiempo real
inputOrigen.addEventListener('input', actualizarResumen);
inputDestino.addEventListener('input', actualizarResumen);
inputFechaIda.addEventListener('change', actualizarResumen);
inputFechaVuelta.addEventListener('change', actualizarResumen);
selectClase.addEventListener('change', actualizarResumen);
selectPersonas.addEventListener('change', actualizarResumen);

// Botones de navegación
btnContinuar.addEventListener('click', () => {
    guardarDatos();
    window.location.href = 'buy-acomp.html';
});

document.querySelector('.btn-action.secondary').addEventListener('click', () => {
    window.location.href = 'home.html';
});

// Funciones auxiliares
function cargarDatos() {
    const datos = obtenerCookie('datosViaje');
    if (datos) {
        try {
            const viaje = JSON.parse(datos);
            inputOrigen.value = viaje.origen || '';
            inputDestino.value = viaje.destino || '';
            inputFechaIda.value = viaje.fechaIda || '';
            inputFechaVuelta.value = viaje.fechaVuelta || '';
            selectClase.value = viaje.clase || '';
            selectPersonas.value = viaje.personas || '';
        } catch (e) {
            console.warn('No se pudieron parsear los datos de viaje almacenados', e);
        }
        actualizarResumen();
    } else {
        // Si no había datos, igualmente refrescamos el resumen inicial
        actualizarResumen();
    }
}

function actualizarResumen() {
    // Actualizar origen
    resumenOrigen.textContent = inputOrigen.value || '—';
    
    // Actualizar destino
    resumenDestino.textContent = inputDestino.value || '—';
    
    // Actualizar fechas
    if (inputFechaIda.value && inputFechaVuelta.value) {
        const fechaIda = formatearFecha(inputFechaIda.value);
        const fechaVuelta = formatearFecha(inputFechaVuelta.value);
        resumenFechas.textContent = `${fechaIda} - ${fechaVuelta}`;
    } else {
        resumenFechas.textContent = '—';
    }
    
    // Actualizar personas (usando las traducciones)
    if (selectPersonas.value) {
        const num = parseInt(selectPersonas.value, 10);

        const unitKey = num === 1
            ? 'buy_summary_people_unit_singular'
            : 'buy_summary_people_unit_plural';

        const unit = t(unitKey) || '';
        resumenPersonas.textContent = unit ? `${num} ${unit}` : String(num);
    } else {
        resumenPersonas.textContent = '—';
    }
    
    // Calcular y actualizar precio
    calcularPrecio();
    
    // Habilitar o deshabilitar botón continuar
    validarFormulario();
}

function calcularPrecio() {
    if (selectClase.value && selectPersonas.value) {
        const precioBase = preciosBase[selectClase.value];
        const numPersonas = parseInt(selectPersonas.value, 10);
        const total = precioBase * numPersonas;
        totalPrice.textContent = `${total}€`;
    } else {
        totalPrice.textContent = '—';
    }
}

function validarFormulario() {
    const formularioCompleto = 
        inputOrigen.value.trim() !== '' &&
        inputDestino.value.trim() !== '' &&
        inputFechaIda.value !== '' &&
        inputFechaVuelta.value !== '' &&
        selectClase.value !== '' &&
        selectPersonas.value !== '';
    
    btnContinuar.disabled = !formularioCompleto;

    if (formularioCompleto) {
        btnContinuar.classList.add('active');
    } else {
        btnContinuar.classList.remove('active');
    }
}

function guardarDatos() {
    const datosViaje = {
        origen: inputOrigen.value,
        destino: inputDestino.value,
        fechaIda: inputFechaIda.value,
        fechaVuelta: inputFechaVuelta.value,
        clase: selectClase.value,
        personas: selectPersonas.value,
        precioBase: totalPrice.textContent
    };
    
    guardarCookie('datosViaje', JSON.stringify(datosViaje), 7);
}

function formatearFecha(fecha) {
    const date = new Date(fecha + 'T00:00:00');
    const opciones = { day: '2-digit', month: '2-digit', year: 'numeric' };

    return date.toLocaleDateString('es-ES', opciones);
}

// Funciones para manejo de cookies
function guardarCookie(nombre, valor, dias) {
    const fecha = new Date();
    fecha.setTime(fecha.getTime() + (dias * 24 * 60 * 60 * 1000));
    const expira = 'expires=' + fecha.toUTCString();
    document.cookie = `${nombre}=${valor};${expira};path=/`;
}

function obtenerCookie(nombre) {
    const nombreCookie = nombre + '=';
    const cookies = document.cookie.split(';');
    for (let i = 0; i < cookies.length; i++) {
        let cookie = cookies[i].trim();
        if (cookie.indexOf(nombreCookie) === 0) {
            return cookie.substring(nombreCookie.length);
        }
    }
    return null;
}

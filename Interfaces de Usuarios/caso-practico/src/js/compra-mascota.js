document.addEventListener('DOMContentLoaded', () => {
   
    // 1. HEADER: enlace "Mi cuenta"
   
    const accountLink = document.querySelector('.user-link');
    if (accountLink) {
        const currentUser = localStorage.getItem('msf_currentUser');
        if (currentUser) {
            accountLink.href = 'account-dashboard.html';
            accountLink.textContent = 'Mi cuenta';
        } else {
            accountLink.href = 'login.html';
        }
    }

    
    // 2. SELECTORES DEL DOM
   
    const radioSi = document.getElementById('radio-si');
    const radioNo = document.getElementById('radio-no');
    const petDetails = document.getElementById('pet-details');
    const tipoMascota = document.getElementById('tipo-mascota');
    const tamanoMascota = document.getElementById('tamano-mascota');
    const observacionesMascota = document.getElementById('observaciones-mascota');
    
    const btnVolver = document.getElementById('btn-volver-acomp');
    const btnContinuar = document.getElementById('btn-continuar-intolerancias');
    const mascotaForm = document.getElementById('mascota-form');

    const resumenOrigen = document.querySelector('.summary-list li:nth-child(1) strong');
    const resumenDestino = document.querySelector('.summary-list li:nth-child(2) strong');
    const resumenFechas = document.querySelector('.summary-list li:nth-child(3) strong');
    const resumenPersonas = document.querySelector('.summary-list li:nth-child(4) strong');
    const resumenAcompanantes = document.querySelector('.summary-list li:nth-child(5) strong');
    const resumenMascota = document.getElementById('summary-mascota');
    const totalPriceElem = document.querySelector('.total-price');


    // 3. UTILIDADES (COOKIES + FECHAS)

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

    function formatearFecha(fecha) {
        if (!fecha) return '—';
        const date = new Date(fecha + 'T00:00:00');
        const opciones = { day: '2-digit', month: '2-digit', year: 'numeric' };
        return date.toLocaleDateString('es-ES', opciones);
    }

    function obtenerNumeroAcompanantesGuardados() {
        try {
            const guardados = JSON.parse(localStorage.getItem('compra_acompanantes') || '[]');
            if (!Array.isArray(guardados)) return 0;
            // Contamos solo los que tengan nombre no vacío
            return guardados.filter(c => c && c.nombre && c.nombre.trim() !== '').length;
        } catch (e) {
            console.warn('Error leyendo compra_acompanantes', e);
            return 0;
        }
    }


    // 4. CARGAR DATOS DEL VIAJE + RESUMEN
 
    function cargarDatosViajeYResumen() {
        let personasTotal = 1;
        const numAcompGuardados = obtenerNumeroAcompanantesGuardados();
        let datosViaje = null;

        const cookieDatos = obtenerCookie('datosViaje');
        if (cookieDatos) {
            try {
                datosViaje = JSON.parse(cookieDatos);
            } catch (e) {
                console.warn('No se pudo parsear la cookie datosViaje', e);
            }
        }

        if (datosViaje) {
            if (resumenOrigen) resumenOrigen.textContent = datosViaje.origen || '—';
            if (resumenDestino) resumenDestino.textContent = datosViaje.destino || '—';

            if (resumenFechas) {
                if (datosViaje.fechaIda && datosViaje.fechaVuelta) {
                    resumenFechas.textContent = `${formatearFecha(datosViaje.fechaIda)} - ${formatearFecha(datosViaje.fechaVuelta)}`;
                } else {
                    resumenFechas.textContent = '—';
                }
            }

            if (datosViaje.personas) {
                const n = parseInt(datosViaje.personas, 10);
                personasTotal = isNaN(n) ? 1 : n;
            } else {
                personasTotal = 1 + numAcompGuardados;
            }

            if (totalPriceElem) {
                if (datosViaje.precioBase && datosViaje.precioBase !== '—') {
                    totalPriceElem.textContent = datosViaje.precioBase;
                } else {
                    totalPriceElem.textContent = '—';
                }
            }
        } else {
            // Fallback: search_data por si entra desde otro flujo
            let searchData = {};
            try {
                searchData = JSON.parse(localStorage.getItem('search_data') || '{}');
            } catch (e) {
                searchData = {};
            }

            if (resumenOrigen) resumenOrigen.textContent = searchData.origen || '—';
            if (resumenDestino) resumenDestino.textContent = searchData.destination || '—';
            if (resumenFechas) resumenFechas.textContent = searchData.dates || '—';

            personasTotal = 1 + numAcompGuardados;

            if (totalPriceElem) totalPriceElem.textContent = '—';
        }

        if (resumenPersonas) resumenPersonas.textContent = personasTotal;
        if (resumenAcompanantes) resumenAcompanantes.textContent = numAcompGuardados;
    }

   
    // 5. MASCOTA: VISUAL + RESUMEN
    
    function actualizarResumenMascota() {
        if (!resumenMascota) return;

        // Detectamos idioma
        const currentLang = localStorage.getItem('selectedLang') || 'es';

        // 2. Diccionario de traducciones simples
        const textos = {
            es: {
                si: 'Sí', no: 'No',
                perro: 'Perro', gato: 'Gato', otro: 'Otro',
                pequeno: 'Pequeño (hasta 5kg)', 
                mediano: 'Mediano (5-15kg)', 
                grande: 'Grande (más de 15kg)'
            },
            en: {
                si: 'Yes', no: 'No',
                perro: 'Dog', gato: 'Cat', otro: 'Other',
                pequeno: 'Small (up to 5kg)', 
                mediano: 'Medium (5-15kg)', 
                grande: 'Large (over 15kg)'
            },
            it: {
                si: 'Sì', no: 'No',
                perro: 'Cane', gato: 'Gatto', otro: 'Altro',
                pequeno: 'Piccolo (fino a 5kg)', 
                mediano: 'Medio (5-15kg)', 
                grande: 'Grande (oltre 15kg)'
            }
        };

        // Seleccionamos el idioma actual
        const t = textos[currentLang];

        if (radioSi && radioSi.checked) {
            let resultado = t.si;

            // Obtenemos los valores internos
            const valTipo = tipoMascota ? tipoMascota.value : '';
            const valTamano = tamanoMascota ? tamanoMascota.value : '';

            // Buscamos la traducción directa en nuestro diccionario
            const txtTipo = valTipo ? t[valTipo] : ''; 
            const txtTamano = valTamano ? t[valTamano] : '';

            if (txtTipo && txtTamano) {
                resultado = `${t.si} (${txtTipo}, ${txtTamano})`;
            } else if (txtTipo) {
                resultado = `${t.si} (${txtTipo})`;
            }
            
            resumenMascota.textContent = resultado;
        } else {
            resumenMascota.textContent = t.no;
        }
    }

    function handleRadioChange() {
        if (!radioSi || !radioNo || !petDetails) return;

        if (radioSi.checked) {
            petDetails.style.display = 'block';
        } else {
            petDetails.style.display = 'none';
            if (tipoMascota) tipoMascota.value = '';
            if (tamanoMascota) tamanoMascota.value = '';
            if (observacionesMascota) observacionesMascota.value = '';
        }
        actualizarResumenMascota();
    }


    // 6. GUARDAR / RECUPERAR DATOS DE MASCOTA
  
    function guardarMascota() {
        const formData = {
            viajaMascota: radioSi && radioSi.checked ? 'si' : 'no',
            tipo: tipoMascota ? tipoMascota.value : '',
            tamano: tamanoMascota ? tamanoMascota.value : '',
            observaciones: observacionesMascota ? observacionesMascota.value : ''
        };
        localStorage.setItem('compra_mascota', JSON.stringify(formData));
    }

    function cargarMascotaGuardada() {
        let savedData = null;
        try {
            savedData = JSON.parse(localStorage.getItem('compra_mascota') || 'null');
        } catch (e) {
            savedData = null;
        }
        if (!savedData) {
            // Por defecto: no viaja con mascota
            if (radioNo) radioNo.checked = true;
            handleRadioChange();
            return;
        }

        if (savedData.viajaMascota === 'si' && radioSi && radioNo) {
            radioSi.checked = true;
            petDetails.style.display = 'block';
        } else if (radioNo) {
            radioNo.checked = true;
            petDetails.style.display = 'none';
        }

        if (tipoMascota) tipoMascota.value = savedData.tipo || '';
        if (tamanoMascota) tamanoMascota.value = savedData.tamano || '';
        if (observacionesMascota) observacionesMascota.value = savedData.observaciones || '';

        actualizarResumenMascota();
    }


    // 7. EVENTOS

    if (radioSi) radioSi.addEventListener('change', handleRadioChange);
    if (radioNo) radioNo.addEventListener('change', handleRadioChange);

    if (tipoMascota) tipoMascota.addEventListener('change', actualizarResumenMascota);
    if (tamanoMascota) tamanoMascota.addEventListener('change', actualizarResumenMascota);

    if (observacionesMascota) {
        observacionesMascota.addEventListener('input', () => {
            // No lo mostramos en el resumen, pero podrías extenderlo aquí si quisieras
        });
    }

    if (btnContinuar) {
        btnContinuar.addEventListener('click', (e) => {
            e.preventDefault();

            // Validación básica si viaja con mascota
            if (radioSi && radioSi.checked) {
                if (tipoMascota && !tipoMascota.value) {
                    alert('Por favor, selecciona el tipo de mascota');
                    tipoMascota.focus();
                    return;
                }
                if (tamanoMascota && !tamanoMascota.value) {
                    alert('Por favor, selecciona el tamaño de la mascota');
                    tamanoMascota.focus();
                    return;
                }
            }

            guardarMascota();
            window.location.href = 'compra-intolerancias.html';
        });
    }

    if (btnVolver) {
        btnVolver.addEventListener('click', (e) => {
            e.preventDefault();
            guardarMascota();
            window.location.href = 'buy-acomp.html';
        });
    }

    if (mascotaForm) {
        mascotaForm.addEventListener('submit', (e) => e.preventDefault());
    }


    // 8. INICIALIZACIÓN

    cargarDatosViajeYResumen();
    cargarMascotaGuardada();
    window.scrollTo({ top: 0, behavior: 'instant' });

    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            setTimeout(() => {
                actualizarResumenMascota();
                cargarDatosViajeYResumen();
            }, 50);
        });
    });
});

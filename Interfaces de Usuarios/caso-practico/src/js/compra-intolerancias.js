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


    // 2. SELECTORES DEL FORMULARIO
    
    const checkboxes = document.querySelectorAll('input[name="alergias"]');
    const otrasAlergias = document.getElementById('otrasAlergias');
    const summaryIntolerancias = document.getElementById('summaryIntolerancias');
    const btnVolver = document.getElementById('btn-volver-mascota');
    const btnContinuar = document.getElementById('btn-continuar-resumen');
    const intolerancyForm = document.getElementById('intolerancyForm');


    // 3. SELECTORES DEL RESUMEN LATERAL

    const resumenOrigen = document.querySelector('.summary-list li:nth-child(1) strong');
    const resumenDestino = document.querySelector('.summary-list li:nth-child(2) strong');
    const resumenFechas = document.querySelector('.summary-list li:nth-child(3) strong');
    const resumenPersonas = document.querySelector('.summary-list li:nth-child(4) strong');
    const resumenAcompanantes = document.querySelector('.summary-list li:nth-child(5) strong');
    const resumenMascota = document.querySelector('.summary-list li:nth-child(6) strong');
    const totalPriceElem = document.querySelector('.total-price');


    // 4. UTILIDADES (COOKIES, FECHAS, ACOMPAÑANTES, MASCOTA)

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
            return guardados.filter(c => c && c.nombre && c.nombre.trim() !== '').length;
        } catch (e) {
            console.warn('Error leyendo compra_acompanantes', e);
            return 0;
        }
    }

function cargarMascotaEnResumen() {
        if (!resumenMascota) return;

        const currentLang = localStorage.getItem('selectedLang') || 'es';
        
        const textosMascota = {
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

        let saved = null;
        try {
            saved = JSON.parse(localStorage.getItem('compra_mascota') || 'null');
        } catch (e) { saved = null; }

        const t = textosMascota[currentLang];

        if (!saved || saved.viajaMascota !== 'si') {
            resumenMascota.textContent = t.no;
            return;
        }

        const tipo = saved.tipo && t[saved.tipo] ? t[saved.tipo] : '';
        const tamano = saved.tamano && t[saved.tamano] ? t[saved.tamano] : '';

        let texto = t.si;
        if (tipo && tamano) {
            texto = `${t.si} (${tipo}, ${tamano})`;
        } else if (tipo) {
            texto = `${t.si} (${tipo})`;
        }

        resumenMascota.textContent = texto;
    }

    function cargarDatosViajeYResumen() {
        const numAcompanantes = obtenerNumeroAcompanantesGuardados();
        let personasTotal = 1;
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
                    resumenFechas.textContent =
                        `${formatearFecha(datosViaje.fechaIda)} - ${formatearFecha(datosViaje.fechaVuelta)}`;
                } else {
                    resumenFechas.textContent = '—';
                }
            }

            if (datosViaje.personas) {
                const n = parseInt(datosViaje.personas, 10);
                personasTotal = isNaN(n) ? 1 + numAcompanantes : n;
            } else {
                personasTotal = 1 + numAcompanantes;
            }

            if (totalPriceElem) {
                if (datosViaje.precioBase && datosViaje.precioBase !== '—') {
                    totalPriceElem.textContent = datosViaje.precioBase;
                } else {
                    totalPriceElem.textContent = '—';
                }
            }
        } else {
            // Fallback a search_data si no hubiera cookie
            let searchData = {};
            try {
                searchData = JSON.parse(localStorage.getItem('search_data') || '{}');
            } catch (e) {
                searchData = {};
            }

            if (resumenOrigen) resumenOrigen.textContent = searchData.origen || '—';
            if (resumenDestino) resumenDestino.textContent = searchData.destination || '—';
            if (resumenFechas) resumenFechas.textContent = searchData.dates || '—';
            personasTotal = 1 + numAcompanantes;
            if (totalPriceElem) totalPriceElem.textContent = '—';
        }

        if (resumenPersonas) resumenPersonas.textContent = personasTotal;
        if (resumenAcompanantes) resumenAcompanantes.textContent = numAcompanantes;

        cargarMascotaEnResumen();
    }

    // 5. INTOLERANCIAS: RESUMEN + ESTADO VISUAL

    function buildSelectedAllergiesList() {
        const selected = [];
        checkboxes.forEach(cb => {
            if (cb.checked) {
                const labelEl =
                    cb.closest('.checkbox-option')?.querySelector('.checkbox-label') ||
                    cb.nextElementSibling;
                if (labelEl) selected.push(labelEl.textContent.trim());
            }
        });
        return selected;
    }


    function updateSummaryIntolerancias() {
        if (!summaryIntolerancias) return;

        const currentLang = localStorage.getItem('selectedLang') || 'es';

        const textosAlergias = {
            es: {
                ninguna: 'Ninguna',
                otras: 'Otras especificadas',
                'sin-gluten': 'Sin gluten',
                'sin-lactosa': 'Sin lactosa',
                'frutos-secos': 'Frutos secos',
                'marisco': 'Marisco',
                'huevo': 'Huevo',
                'soja': 'Soja',
                'vegano': 'Vegano',
                'vegetariano': 'Vegetariano'
            },
            en: {
                ninguna: 'None',
                otras: 'Others specified',
                'sin-gluten': 'Gluten-free',
                'sin-lactosa': 'Lactose-free',
                'frutos-secos': 'Nuts',
                'marisco': 'Seafood',
                'huevo': 'Egg',
                'soja': 'Soy',
                'vegano': 'Vegan',
                'vegetariano': 'Vegetarian'
            },
            it: {
                ninguna: 'Nessuna',
                otras: 'Altre specificate',
                'sin-gluten': 'Senza glutine',
                'sin-lactosa': 'Senza lattosio',
                'frutos-secos': 'Frutta secca',
                'marisco': 'Frutti di mare',
                'huevo': 'Uova',
                'soja': 'Soia',
                'vegano': 'Vegano',
                'vegetariano': 'Vegetariano'
            }
        };

        const t = textosAlergias[currentLang];
        const selectedLabels = [];

        checkboxes.forEach(cb => {
            if (cb.checked) {
                const traduccion = t[cb.value];
                selectedLabels.push(traduccion || cb.value);
            }
        });

        if (selectedLabels.length > 0) {
            summaryIntolerancias.textContent = selectedLabels.join(', ');
        } else if (otrasAlergias && otrasAlergias.value.trim()) {
            summaryIntolerancias.textContent = t.otras;
        } else {
            summaryIntolerancias.textContent = t.ninguna;
        }
    }

    function updateCheckboxVisual(checkbox) {
        const wrapper = checkbox.closest('.checkbox-option');
        if (!wrapper) return;
        if (checkbox.checked) {
            wrapper.classList.add('active');
        } else {
            wrapper.classList.remove('active');
        }
    }


    // 6. GUARDADO AUTOMÁTICO EN localStorage

    function getFormData() {
        const selectedAllergies = [];

        checkboxes.forEach(cb => {
            if (cb.checked) {
                const labelEl =
                    cb.closest('.checkbox-option')?.querySelector('.checkbox-label') ||
                    cb.nextElementSibling;
                selectedAllergies.push({
                    value: cb.value,
                    label: labelEl ? labelEl.textContent.trim() : cb.value
                });
            }
        });

        return {
            alergiasFrecuentes: selectedAllergies,
            otrasAlergias: (otrasAlergias?.value || '').trim(),
            timestamp: new Date().toISOString()
        };
    }

    function autoSave() {
        try {
            const data = getFormData();
            localStorage.setItem('compra_intolerancias', JSON.stringify(data));
        } catch (e) {
            console.error('Error al guardar compra_intolerancias:', e);
        }
    }

    function loadSavedData() {
        let data = null;
        try {
            data = JSON.parse(localStorage.getItem('compra_intolerancias') || 'null');
        } catch (e) {
            data = null;
        }

        if (data) {
            // Restaurar checkboxes
            if (Array.isArray(data.alergiasFrecuentes)) {
                data.alergiasFrecuentes.forEach(item => {
                    const cb = document.querySelector(
                        `input[name="alergias"][value="${item.value}"]`
                    );
                    if (cb) cb.checked = true;
                });
            }

            // Restaurar textarea
            if (otrasAlergias && data.otrasAlergias) {
                otrasAlergias.value = data.otrasAlergias;
            }
        }

        // Actualizar visual y resumen tras restaurar
        checkboxes.forEach(cb => updateCheckboxVisual(cb));
        updateSummaryIntolerancias();
        updateCharCounter();
    }


    // 7. EVENTOS

    checkboxes.forEach(cb => {
        cb.addEventListener('change', () => {
            updateCheckboxVisual(cb);
            updateSummaryIntolerancias();
            autoSave();
        });
    });

    const maxLength = 500;
    let charCounter = null;

    function updateCharCounter() {
        if (!otrasAlergias) return;
        if (!charCounter) {
            charCounter = document.createElement('div');
            charCounter.className = 'textarea-counter';
            otrasAlergias.parentNode.appendChild(charCounter);
        }
        const length = otrasAlergias.value.length;
        charCounter.textContent = `${length} / ${maxLength} caracteres`;
    }

    if (otrasAlergias) {
        otrasAlergias.setAttribute('maxlength', maxLength.toString());

        otrasAlergias.addEventListener('input', () => {
            updateSummaryIntolerancias();
            updateCharCounter();
            autoSave();
        });
    }

    if (btnVolver) {
        btnVolver.addEventListener('click', (e) => {
            e.preventDefault();
            autoSave();
            window.location.href = 'compra-mascota.html';
        });
    }

    if (btnContinuar) {
        btnContinuar.addEventListener('click', (e) => {
            e.preventDefault();
            autoSave();
            window.location.href = 'compra-resumen.html';
        });
    }

    if (intolerancyForm) {
        intolerancyForm.addEventListener('submit', (e) => e.preventDefault());
    }

    // Guardado periódico
    setInterval(autoSave, 30000);

  
    // 8. INICIALIZACIÓN
 
    cargarDatosViajeYResumen();
    loadSavedData();
    window.scrollTo({ top: 0, behavior: 'auto' });
});

// Función auxiliar global (por si la necesitas desde otro script)
function getIntolerancyData() {
    const checkboxes = document.querySelectorAll('input[name="alergias"]:checked');
    const selectedAllergies = Array.from(checkboxes).map(cb => ({
        value: cb.value,
        label:
            cb.closest('.checkbox-option')?.querySelector('.checkbox-label')?.textContent.trim() ||
            cb.nextElementSibling.textContent.trim()
    }));

    return {
        alergiasFrecuentes: selectedAllergies,
        otrasAlergias: (document.getElementById('otrasAlergias')?.value || '').trim()
    };
}

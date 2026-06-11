
document.addEventListener('DOMContentLoaded', function() {

  
    // 1. Enlace "Mi cuenta" en el header

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


    // 2. VARIABLES Y SELECTORES

    let companionsData = [];
    let maxCompanions = 0;
    let companionCount = 0;
    
    const companionsList = document.getElementById('companions-list');
    const btnAdd = document.getElementById('btn-add-companion');
    const btnContinuar = document.getElementById('btn-continuar-pets');

    const resumenOrigen = document.querySelector('.summary-list li:nth-child(1) strong');
    const resumenDestino = document.querySelector('.summary-list li:nth-child(2) strong');
    const resumenFechas = document.querySelector('.summary-list li:nth-child(3) strong');
    const resumenPersonas = document.querySelector('.summary-list li:nth-child(4) strong');
    const resumenAcomp = document.querySelector('.summary-list li:nth-child(5) strong');
    const totalPriceElem = document.querySelector('.total-price');


    // 3. COOKIES (para leer datos de buy-p1)

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

 
    // 4. DATOS DEL VIAJE (Resumen lateral)

    function inferirPersonasDesdeSearch(peopleStr) {
        if (!peopleStr) return 1;
        const match = peopleStr.match(/\d+/);
        if (match) {
            return parseInt(match[0], 10);
        }
        const lower = peopleStr.toLowerCase();
        if (lower.includes('grupo')) return 5;
        if (lower.includes('familia')) return 4;
        if (peopleStr.includes('1')) return 1;
        return 1;
    }

    function cargarDatosViaje() {
        let personasTotal = 1;

        // 1) Intentar leer de la cookie creada en buy-p1
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
                const num = parseInt(datosViaje.personas, 10);
                personasTotal = isNaN(num) ? 1 : num;
            }

            if (totalPriceElem && datosViaje.precioBase && datosViaje.precioBase !== '—') {
                totalPriceElem.textContent = datosViaje.precioBase;
            } else if (totalPriceElem) {
                totalPriceElem.textContent = '—';
            }
        } else {
            // 2) Fallback: datos del buscador (search_data)
            const searchData = JSON.parse(localStorage.getItem('search_data') || '{}');

            if (resumenOrigen) resumenOrigen.textContent = searchData.origen || '—';
            if (resumenDestino) resumenDestino.textContent = searchData.destination || '—';
            if (resumenFechas) resumenFechas.textContent = searchData.dates || '—';

            personasTotal = inferirPersonasDesdeSearch(searchData.people);
            
            if (totalPriceElem) {
                totalPriceElem.textContent = '—';
            }
        }

        if (resumenPersonas) {
            resumenPersonas.textContent = personasTotal;
        }

        // El usuario principal cuenta como 1
        maxCompanions = Math.max(0, personasTotal - 1);
        if (resumenAcomp) {
            resumenAcomp.textContent = '0';
        }

        console.log('Total Personas:', personasTotal, 'Máx Acompañantes:', maxCompanions);
    }

    function formatearFecha(fecha) {
        if (!fecha) return '—';
        const date = new Date(fecha + 'T00:00:00');
        const opciones = { day: '2-digit', month: '2-digit', year: 'numeric' };
        return date.toLocaleDateString('es-ES', opciones);
    }


    // 5. GESTIÓN DE TARJETAS DE ACOMPAÑANTES

    
    function crearYAgregarTarjeta(datosPrevios = null) {
        if (!companionsList) return;

        // Verificación de límite antes de crear
        if (companionsList.children.length >= maxCompanions && maxCompanions > 0) {
            alert(`Para este viaje seleccionaste ${maxCompanions + 1} personas. No puedes añadir más acompañantes.`);
            return;
        }

        const index = companionCount;
        companionCount++;

        const card = document.createElement('div');
        card.className = 'companion-card';
        card.dataset.index = index;

        card.innerHTML = `
            <div class="companion-header">
                <h4><span data-i18n="buy_companion_label_prefix">Acompañante</span> ${companionsList.children.length + 1}</h4>
                <button type="button" class="btn-delete" data-i18n="buy_companion_delete">ELIMINAR</button>
            </div>
            <div class="travel-form">
                <div class="form-row">
                    <input type="text" placeholder="Nombre..." class="input-field companion-nombre" value="${datosPrevios?.nombre || ''}" data-i18n-placeholder="buy_companion_name_ph">
                    <input type="email" placeholder="Email..." class="input-field companion-email" value="${datosPrevios?.email || ''}" data-i18n-placeholder="buy_companion_email_ph">
                </div>
                <div class="form-row">
                    <input type="text" placeholder="Apellidos..." class="input-field companion-apellidos" value="${datosPrevios?.apellidos || ''}" data-i18n-placeholder="buy_companion_surname_ph">
                    <div class="input-spacer"></div> 
                </div>
            </div>
        `;

        // Event Listeners Inputs
        const inputs = card.querySelectorAll('input');
        inputs.forEach(input => {
            input.addEventListener('input', () => {
                actualizarArrayDatos();
                actualizarResumenLateral();
            });
        });

        // Event Listener Eliminar
        const btnDelete = card.querySelector('.btn-delete');
        btnDelete.addEventListener('click', function() {
            card.remove();
            renumerarTarjetas();
            actualizarArrayDatos();
            actualizarResumenLateral();
            checkBotonAnadir();
        });

        companionsList.appendChild(card);
        
        checkBotonAnadir();
        actualizarArrayDatos();
        actualizarResumenLateral();
    }

    function renumerarTarjetas() {
        const cards = companionsList.querySelectorAll('.companion-card');
        cards.forEach((card, i) => {
            const h4 = card.querySelector('h4');
            if (h4) h4.textContent = `Acompañante ${i + 1}`;
        });
    }

    function checkBotonAnadir() {
        if (!btnAdd) return;
        const currentCards = companionsList.children.length;
        if (maxCompanions > 0 && currentCards >= maxCompanions) {
            btnAdd.style.display = 'none';
        } else {
            btnAdd.style.display = 'inline-flex';
        }
    }

    function actualizarArrayDatos() {
        companionsData = [];
        const cards = companionsList.querySelectorAll('.companion-card');
        
        cards.forEach(card => {
            const nombre = card.querySelector('.companion-nombre')?.value || '';
            const apellidos = card.querySelector('.companion-apellidos')?.value || '';
            const email = card.querySelector('.companion-email')?.value || '';
            
            companionsData.push({ nombre, apellidos, email });
        });
    }

    function actualizarResumenLateral() {
        if (!resumenAcomp) return;
        const activos = companionsData.filter(c => c.nombre.trim() !== '').length;
        resumenAcomp.textContent = String(activos);
    }


    // 6. CARGAR DATOS PREVIOS

    function cargarAcompanantesGuardados() {
        let guardados = [];
        const raw = localStorage.getItem('compra_acompanantes');
        
        if (raw) {
            try {
                guardados = JSON.parse(raw) || [];
            } catch (e) {
                console.warn('Error al parsear compra_acompanantes', e);
            }
        }

        if (guardados.length > 0) {
            const limite = maxCompanions > 0 ? maxCompanions : guardados.length;
            guardados.slice(0, limite).forEach(dato => {
                crearYAgregarTarjeta(dato);
            });
        } else if (maxCompanions > 0) {
            // Una tarjeta vacía por defecto si hay acompañantes posibles
            crearYAgregarTarjeta();
        }

        actualizarArrayDatos();
        actualizarResumenLateral();
        checkBotonAnadir();
    }


    // 7. EVENTOS BOTONES

    
    if (btnAdd) {
        btnAdd.addEventListener('click', function(e) {
            e.preventDefault();
            crearYAgregarTarjeta();
        });
    }

    if (btnContinuar) {
        btnContinuar.addEventListener('click', function(e) {
            e.preventDefault();
            
            actualizarArrayDatos();
            localStorage.setItem('compra_acompanantes', JSON.stringify(companionsData));
            
            window.location.href = 'compra-mascota.html';
        });
    }


    // 8. INICIALIZACIÓN

    cargarDatosViaje();
    cargarAcompanantesGuardados();
});

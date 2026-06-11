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


    // 2. SELECTORES PAGO

    const radioTarjeta = document.getElementById('radioTarjeta');
    const radioPaypal = document.getElementById('radioPaypal');
    const tarjetaDetails = document.getElementById('tarjetaDetails');
    const paypalDetails = document.getElementById('paypalDetails');
    const paymentForm = document.getElementById('paymentForm');
    const nombreTitular = document.getElementById('nombreTitular');
    const numeroTarjeta = document.getElementById('numeroTarjeta');
    const caducidad = document.getElementById('caducidad');
    const cvc = document.getElementById('cvc');
    const acceptTerms = document.getElementById('acceptTerms');
    const btnVolverIntolerancias = document.getElementById('btn-volver-intolerancias');
    const btnConfirmarReserva = document.getElementById('btn-confirmar-reserva');
    const paymentTotalSpan = document.getElementById('payment-total');

   
    // 3. SELECTORES RESUMEN

    const resOrigen = document.getElementById('res-origen');
    const resDestino = document.getElementById('res-destino');
    const resFechas = document.getElementById('res-fechas');
    const resPersonas = document.getElementById('res-personas');
    const resAcompList = document.getElementById('res-acompanantes-list');
    const resMascota = document.getElementById('res-mascota');
    const resIntolerancias = document.getElementById('res-intolerancias');
    const resBilletes = document.getElementById('res-billetes');
    const resTotal = document.getElementById('res-total');


    // 4. UTILIDADES (COOKIES + DATOS)

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

    function getViajeData() {
        const base = {
            origen: '—',
            destino: '—',
            fechaIda: '',
            fechaVuelta: '',
            clase: '',
            personas: null,
            precioBase: '—'
        };

        const cookie = obtenerCookie('datosViaje');
        if (!cookie) return base;

        try {
            const data = JSON.parse(cookie);
            return Object.assign(base, data);
        } catch (e) {
            console.warn('No se pudo parsear datosViaje', e);
            return base;
        }
    }

    function getCompanionsData() {
        try {
            const raw = localStorage.getItem('compra_acompanantes');
            if (!raw) return [];
            const arr = JSON.parse(raw);
            return Array.isArray(arr) ? arr : [];
        } catch (e) {
            console.warn('Error al leer compra_acompanantes', e);
            return [];
        }
    }

function getMascotaSummary() {
        const currentLang = localStorage.getItem('selectedLang') || 'es';
        
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

        const t = textos[currentLang];

        try {
            const raw = localStorage.getItem('compra_mascota');
            if (!raw) return t.no;
            const data = JSON.parse(raw);
            if (!data || data.viajaMascota !== 'si') return t.no;

            const tipo = data.tipo && t[data.tipo] ? t[data.tipo] : '';
            const tam = data.tamano && t[data.tamano] ? t[data.tamano] : '';

            let texto = t.si;
            if (tipo && tam) texto = `${t.si} (${tipo}, ${tam})`;
            else if (tipo) texto = `${t.si} (${tipo})`;

            return texto;
        } catch (e) {
            console.warn('Error al leer compra_mascota', e);
            return t.no;
        }
    }

function getIntoleranciasSummary() {
        const currentLang = localStorage.getItem('selectedLang') || 'es';

        const textosAlergias = {
            es: {
                ninguna: 'Ninguna',
                otras: 'Otras especificadas',
                'sin-gluten': 'Sin gluten', 'sin-lactosa': 'Sin lactosa',
                'frutos-secos': 'Frutos secos', 'marisco': 'Marisco',
                'huevo': 'Huevo', 'soja': 'Soja',
                'vegano': 'Vegano', 'vegetariano': 'Vegetariano'
            },
            en: {
                ninguna: 'None',
                otras: 'Others specified',
                'sin-gluten': 'Gluten-free', 'sin-lactosa': 'Lactose-free',
                'frutos-secos': 'Nuts', 'marisco': 'Seafood',
                'huevo': 'Egg', 'soja': 'Soy',
                'vegano': 'Vegan', 'vegetariano': 'Vegetarian'
            },
            it: {
                ninguna: 'Nessuna',
                otras: 'Altre specificate',
                'sin-gluten': 'Senza glutine', 'sin-lactosa': 'Senza lattosio',
                'frutos-secos': 'Frutta secca', 'marisco': 'Frutti di mare',
                'huevo': 'Uova', 'soja': 'Soia',
                'vegano': 'Vegano', 'vegetariano': 'Vegetariano'
            }
        };

        const t = textosAlergias[currentLang];

        try {
            const raw = localStorage.getItem('compra_intolerancias');
            if (!raw) return t.ninguna;
            const data = JSON.parse(raw);

            const labels = (data.alergiasFrecuentes || [])
                .map(item => t[item.value] || item.label)
                .filter(Boolean);

            if (labels.length > 0) {
                const prefix = { es: 'Seleccionadas: ', en: 'Selected: ', it: 'Selezionato: ' };
                return prefix[currentLang] + labels.join(', ');
            } else if (data.otrasAlergias && data.otrasAlergias.trim() !== '') {
                return t.otras;
            }
            return t.ninguna;
        } catch (e) {
            console.warn('Error al leer compra_intolerancias', e);
            return t.ninguna;
        }
    }

   
    // 5. RELLENAR RESUMEN FINAL

    function rellenarResumen() {
        const viaje = getViajeData();
        const companions = getCompanionsData();

        const acompValidos = companions.filter(c => c && (c.nombre || c.apellidos));
        const numAcomp = acompValidos.length;

        let personas = 1;
        if (viaje.personas && !isNaN(parseInt(viaje.personas, 10))) {
            personas = parseInt(viaje.personas, 10);
        } else {
            personas = 1 + numAcomp;
        }

        if (resOrigen) resOrigen.textContent = viaje.origen || '—';
        if (resDestino) resDestino.textContent = viaje.destino || '—';

        if (resFechas) {
            if (viaje.fechaIda && viaje.fechaVuelta) {
                resFechas.textContent =
                    `${formatearFecha(viaje.fechaIda)} - ${formatearFecha(viaje.fechaVuelta)}`;
            } else {
                resFechas.textContent = '—';
            }
        }

        if (resPersonas) {
            let personasText = `${personas}`;
            if (numAcomp > 0) {
                personasText += ` (1 titular + ${numAcomp} acompañante${numAcomp > 1 ? 's' : ''})`;
            }
            resPersonas.textContent = personasText;
        }

        // Acompañantes
        if (resAcompList) {
            resAcompList.innerHTML = '';
            if (numAcomp === 0) {
                resAcompList.innerHTML = '<p>Sin acompañantes añadidos.</p>';
            } else {
                acompValidos.forEach((c, idx) => {
                    const p = document.createElement('p');
                    const nombreCompleto = `${c.nombre || ''} ${c.apellidos || ''}`.trim() || 'Sin nombre';
                    const email = c.email && c.email.trim() !== '' ? c.email : 'Sin email';
                    p.textContent = `Acompañante ${idx + 1}: ${nombreCompleto} – ${email}`;
                    resAcompList.appendChild(p);
                });
            }
        }

        // Mascota
        if (resMascota) {
            resMascota.textContent = getMascotaSummary();
        }

        // Intolerancias
        if (resIntolerancias) {
            resIntolerancias.textContent = getIntoleranciasSummary();
        }

        // Precio
        const precioBase = viaje.precioBase && viaje.precioBase.trim() !== ''
            ? viaje.precioBase
            : null;

        if (resBilletes) {
            if (precioBase) {
                const textoPersonas = personas === 1
                    ? '1 pasajero'
                    : `${personas} pasajeros`;
                resBilletes.textContent = `Billetes (${textoPersonas}): ${precioBase}`;
            } else {
                resBilletes.textContent = 'Billetes: —';
            }
        }

        if (resTotal) {
            resTotal.textContent = precioBase || '—';
        }

        if (paymentTotalSpan) {
            paymentTotalSpan.textContent = precioBase || '—';
        }
    }

   
    // 6. LÓGICA DE PAGO

    function updatePaymentMethod() {
        if (!radioTarjeta || !radioPaypal || !tarjetaDetails || !paypalDetails) return;
        if (radioTarjeta.checked) {
            tarjetaDetails.classList.add('active');
            paypalDetails.classList.remove('active');
        } else {
            tarjetaDetails.classList.remove('active');
            paypalDetails.classList.add('active');
        }
    }

    if (radioTarjeta) radioTarjeta.addEventListener('change', updatePaymentMethod);
    if (radioPaypal) radioPaypal.addEventListener('change', updatePaymentMethod);

    // Formateo número de tarjeta XXXX XXXX XXXX XXXX
    if (numeroTarjeta) {
        numeroTarjeta.addEventListener('input', (e) => {
            let value = e.target.value.replace(/\s/g, '').replace(/\D/g, '');
            const grupos = value.match(/.{1,4}/g);
            e.target.value = grupos ? grupos.join(' ') : value;
        });
    }

    // Formateo caducidad MM/AA
    if (caducidad) {
        caducidad.addEventListener('input', (e) => {
            let value = e.target.value.replace(/\D/g, '');
            if (value.length > 4) value = value.slice(0, 4);
            if (value.length >= 3) {
                value = value.slice(0, 2) + '/' + value.slice(2);
            }
            e.target.value = value;
        });
    }

    // CVC solo números
    if (cvc) {
        cvc.addEventListener('input', (e) => {
            e.target.value = e.target.value.replace(/\D/g, '').slice(0, 3);
        });
    }

    // Validación Luhn
    function validateCardNumber(number) {
        const cleaned = number.replace(/\s/g, '');
        if (!/^\d+$/.test(cleaned)) return false;

        let sum = 0;
        let isEven = false;

        for (let i = cleaned.length - 1; i >= 0; i--) {
            let digit = parseInt(cleaned[i], 10);
            if (isEven) {
                digit *= 2;
                if (digit > 9) digit -= 9;
            }
            sum += digit;
            isEven = !isEven;
        }
        return sum % 10 === 0;
    }

    function getPaymentData() {
        const metodoPago = (document.querySelector('input[name="metodoPago"]:checked') || {}).value || 'tarjeta';
        const data = {
            metodoPago,
            timestamp: new Date().toISOString()
        };

        if (metodoPago === 'tarjeta') {
            data.tarjeta = {
                nombreTitular: nombreTitular ? nombreTitular.value.trim() : '',
                numeroTarjeta: numeroTarjeta ? numeroTarjeta.value.replace(/\s/g, '') : '',
                caducidad: caducidad ? caducidad.value.trim() : '',
                cvc: cvc ? cvc.value.trim() : ''
            };
        }

        return data;
    }

    function procesarPago() {
        const metodoPago = (document.querySelector('input[name="metodoPago"]:checked') || {}).value || 'tarjeta';

        if (!acceptTerms || !acceptTerms.checked) {
            alert('Debes aceptar las condiciones de uso y la política de cancelación.');
            return;
        }

        if (metodoPago === 'tarjeta') {
            const numero = numeroTarjeta ? numeroTarjeta.value.replace(/\s/g, '') : '';
            if (!numero || !validateCardNumber(numero)) {
                alert('Por favor, introduce un número de tarjeta válido.');
                if (numeroTarjeta) numeroTarjeta.focus();
                return;
            }
            if (!caducidad || caducidad.value.trim().length < 4) {
                alert('Introduce la caducidad de la tarjeta.');
                if (caducidad) caducidad.focus();
                return;
            }
            if (!cvc || cvc.value.trim().length < 3) {
                alert('Introduce un CVC válido.');
                if (cvc) cvc.focus();
                return;
            }
        }

        const data = getPaymentData();
        console.log('Datos de pago (simulado):', data);

        if (metodoPago === 'paypal') {
            alert('Redirigiendo a PayPal para completar el pago...');
            // window.location.href = 'https://paypal.com/checkout';
        } else {
            alert('¡Reserva confirmada! Se ha enviado un email de confirmación.');
            // window.location.href = 'confirmacion.html';
        }
    }

    // Submit del formulario (por si el usuario pulsa Enter)
    if (paymentForm) {
        paymentForm.addEventListener('submit', (e) => {
            e.preventDefault();
            procesarPago();
        });
    }

    // Botón inferior "CONFIRMAR RESERVA"
    if (btnConfirmarReserva) {
        btnConfirmarReserva.addEventListener('click', (e) => {
            e.preventDefault();
            procesarPago();
        });
    }

    // Botón inferior "VOLVER A INTOLERANCIAS"
    if (btnVolverIntolerancias) {
        btnVolverIntolerancias.addEventListener('click', (e) => {
            e.preventDefault();
            window.location.href = 'compra-intolerancias.html';
        });
    }

    
    // 7. INICIALIZACIÓN
    
    rellenarResumen();
    updatePaymentMethod();
    window.scrollTo({ top: 0, behavior: 'auto' });
});

// Función opcional global por si necesitas leer los datos desde otro script
function getPaymentData() {
    const metodoPago = (document.querySelector('input[name="metodoPago"]:checked') || {}).value || 'tarjeta';
    const data = { metodoPago };

    if (metodoPago === 'tarjeta') {
        data.tarjeta = {
            nombreTitular: document.getElementById('nombreTitular')?.value.trim() || '',
            numeroTarjeta: document.getElementById('numeroTarjeta')?.value || '',
            caducidad: document.getElementById('caducidad')?.value || '',
            cvc: document.getElementById('cvc')?.value || ''
        };
    }

    return data;
}

document.addEventListener('DOMContentLoaded', function() {
    
    // Buscar el enlace "Mi cuenta"
    const accountLink = document.querySelector('.user-link');
    
    if (accountLink) {
        // Comprobar si hay usuario
        const currentUser = localStorage.getItem('msf_currentUser');
        
        if (currentUser) {
            // Si hay usuario, cambiamos el enlace al dashboard
            accountLink.href = 'account-dashboard.html';
            accountLink.textContent = 'Mi cuenta'; // Opcional: Cambiar el texto
        } else {
            // Si no, lo dejamos como estaba (login.html)
            accountLink.href = 'login.html';
        }
    }

    // 1. VARIABLES DEL DOM
    const btnLista = document.getElementById('btnLista');
    const btnMapa = document.getElementById('btnMapa');
    const listaView = document.getElementById('listaView');
    const mapaView = document.getElementById('mapaView');
    const listingsContainer = document.getElementById('listaView');
    const sortSelect = document.getElementById('sortSelect');
    let mapInstance = null;

    // 2. SEGURIDAD: ¿Cargaron los datos?
    if (typeof alojamientos === 'undefined') {
        console.error("Error: info-alojamientos.js no cargado.");
        return;
    }

    // 3. LECTURA DE BÚSQUEDA Y FILTRADO (LA CLAVE)
    const params = new URLSearchParams(window.location.search);
    const searchDest = params.get('destination'); // Ej: "Barcelona"
    const searchDates = params.get('dates');
    const searchPeople = params.get('people');

    // Variable donde guardaremos los resultados filtrados
    let currentData = [];

    if (searchDest && searchDest.trim() !== "") {
        // A. SI HAY BÚSQUEDA: Filtramos
        // Comparamos la ubicación del hotel con lo que escribió el usuario (ignorando mayúsculas)
        currentData = alojamientos.filter(hotel => 
            hotel.ubicacion.toLowerCase().includes(searchDest.toLowerCase())
        );

        // Actualizamos los textos de la página
        const pageTitle = document.querySelector('.page-header h1');
        if(pageTitle) {
            pageTitle.removeAttribute('data-i18n'); // Quitamos la auto-traducción
            pageTitle.textContent = `Alojamientos en ${capitalizeFirstLetter(searchDest)}`;
        }

        const resultsInfo = document.querySelector('.results-info');
        if(resultsInfo) {
            resultsInfo.innerHTML = `Resultados para ${searchPeople || '2 personas'} · ${searchDates || 'Fechas flexibles'}`;
        }

        // Aviso si no encontramos nada
        if (currentData.length === 0) {
            listingsContainer.innerHTML = `
                <div style="padding: 3rem; text-align: center;">
                    <h3>😕 No encontramos alojamientos en "${searchDest}"</h3>
                    <p>Prueba buscando: <strong>El Cairo</strong>, <strong>Barcelona</strong> o <strong>París</strong>.</p>
                    <button onclick="window.location.href='alojamiento.html'" style="margin-top:1rem; padding:10px 20px; cursor:pointer;">Ver todos los destinos</button>
                </div>
            `;
            return; // Paramos aquí para no intentar pintar listas vacías
        }

    } else {
        // B. SI NO HAY BÚSQUEDA: Mostramos todo
        currentData = [...alojamientos];
        
        // Ponemos un título genérico
        const pageTitle = document.querySelector('.page-header h1');
        if(pageTitle) {
            pageTitle.textContent = "Todos los Alojamientos";
            pageTitle.setAttribute('data-i18n', 'page_title_all'); // (Tendrías que crear esta clave en translations)
        }
    }

    // Función auxiliar para poner mayúscula la primera letra (barcelona -> Barcelona)
    function capitalizeFirstLetter(string) {
        return string.charAt(0).toUpperCase() + string.slice(1);
    }


    // 4. FUNCIÓN PARA PINTAR LISTA (RENDER)
    function renderListings(listaDatos) {
        listingsContainer.innerHTML = ''; 

        // Usamos la lista pasada O la global filtrada
        const datos = listaDatos || currentData; 
        const currentLang = localStorage.getItem('selectedLang') || 'es';

        // --- A. ACTUALIZAR CONTADOR Y TEXTO (Singular/Plural) ---
        const countSpan = document.getElementById('count');
        
        // Buscamos el span del texto (puede tener cualquiera de las dos claves)
        const textSpan = document.querySelector('[data-i18n="lodging_found"]') || 
                         document.querySelector('[data-i18n="lodging_found_singular"]');

        if (countSpan) {
            countSpan.textContent = datos.length;
        }

        if (textSpan) {
            if (datos.length === 1) {
                // Si es 1, usamos la clave singular
                textSpan.setAttribute('data-i18n', 'lodging_found_singular');
            } else {
                // Si es 0 o más de 1, usamos plural
                textSpan.setAttribute('data-i18n', 'lodging_found');
            }
        }

        // --- B. MENSAJE SI NO HAY RESULTADOS ---
        if (datos.length === 0) {
            listingsContainer.innerHTML = `
                <div style="padding:3rem; text-align:center; color:#666;">
                    <h3>😕 No encontramos nada en esta zona.</h3>
                    <p>Intenta buscar: <strong>Barcelona</strong>, <strong>París</strong>, <strong>Roma</strong> o <strong>El Cairo</strong>.</p>
                    <button onclick="window.location.href='alojamiento.html'" style="margin-top:10px; padding:10px 20px; cursor:pointer;">Ver todos</button>
                </div>`;
            return; // Paramos aquí para no intentar pintar tarjetas
        }

        // --- C. PINTAR TARJETAS ---
        datos.forEach(hotel => {
            const desc = hotel.descripcion[currentLang] || hotel.descripcion['es'];
            const servs = hotel.servicios[currentLang] || hotel.servicios['es'];

            const html = `
                <div class="listing-card">
                    <img src="${hotel.imagenPrincipal}" alt="${hotel.nombre}" class="listing-image">
                    
                    <div class="listing-content">
                        <h3>${hotel.nombre}</h3>
                        <p class="listing-location">${hotel.ubicacion}</p>
                        <div class="listing-rating">
                            <span class="rating-badge">${hotel.rating}</span>
                            <span><span data-i18n="rate_excellent">Excelente</span> · ${hotel.resenas} <span data-i18n="reviews_count">reseñas</span></span>
                        </div>
                        <div class="listing-features">
                            ${servs.slice(0, 3).map(s => `<span class="feature-tag">${s}</span>`).join('')}
                        </div>
                        <p class="listing-description">${desc.substring(0, 100)}...</p>
                    </div>
                    
                    <div class="listing-price">
                        <div class="price-info">
                            <span class="price-amount">${hotel.precio} €</span>
                            <span class="price-label" data-i18n="price_per_night">por noche</span>
                            <span class="price-total">${hotel.precio * 7} € <span data-i18n="price_total_text">en total</span> (7 <span data-i18n="nights_text">noches</span>)</span>
                        </div>
                        <a href="alojamiento-detalle.html?id=${hotel.id}" class="btn-details" data-i18n="btn_see_details">VER DETALLES</a>
                        <button class="btn-save"><span data-i18n="btn_save">Guardar</span> ❤</button>
                    </div>
                </div>
            `;
            listingsContainer.insertAdjacentHTML('beforeend', html);
        });
        
        attachSaveEvents();
        if (window.updateLanguage) window.updateLanguage(currentLang);
    }

    // 5. FUNCIÓN PARA PINTAR MAPA (SIDEBAR)
    function renderMapListings(listaDatos) {
        const container = document.getElementById('mapItemsContainer');
        if (!container) return;
        container.innerHTML = '';
        
        const datos = listaDatos || currentData;
        const currentLang = localStorage.getItem('selectedLang') || 'es';

        datos.forEach(hotel => {
            const html = `
                <div class="map-listing-item" onclick="window.location.href='alojamiento-detalle.html?id=${hotel.id}'">
                    <img src="${hotel.imagenPrincipal}" alt="${hotel.nombre}">
                    <div class="map-listing-info">
                        <h4>${hotel.nombre}</h4>
                        <p>${hotel.ubicacion}</p>
                        <div class="map-listing-rating">
                            <span class="rating-badge">${hotel.rating}</span>
                            <span data-i18n="rate_excellent">Excelente</span>
                        </div>
                        <p class="map-listing-price"><strong>${hotel.precio} €</strong> <span data-i18n="price_per_night">/noche</span></p>
                    </div>
                </div>
            `;
            container.insertAdjacentHTML('beforeend', html);
        });
        
        if (window.updateLanguage) window.updateLanguage(currentLang);
    }

    // 6. INICIALIZAR EL MAPA (LEAFLET)
    function initMap() {
        if (mapInstance) {
            setTimeout(() => mapInstance.invalidateSize(), 100);
            return;
        }
        
        // Centramos el mapa en el PRIMER resultado de la búsqueda
        const centro = currentData.length > 0 ? currentData[0].coordenadas : [30.0444, 31.2357];
        
        mapInstance = L.map('map').setView(centro, 12);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '© OpenStreetMap'
        }).addTo(mapInstance);

        currentData.forEach(hotel => {
            const marker = L.marker(hotel.coordenadas).addTo(mapInstance);
            marker.bindPopup(`<b>${hotel.nombre}</b><br>${hotel.precio} €`);
        });
    }

    // 7. ORDENAR (SORT)
    if (sortSelect) {
        sortSelect.addEventListener('change', function() {
            const criterio = this.value;
            // Ordenamos SOBRE los resultados actuales (filtrados)
            let listaOrdenada = [...currentData];

            if (criterio === 'price-asc') listaOrdenada.sort((a, b) => a.precio - b.precio);
            if (criterio === 'price-desc') listaOrdenada.sort((a, b) => b.precio - a.precio);
            if (criterio === 'rating') listaOrdenada.sort((a, b) => b.rating - a.rating);

            renderListings(listaOrdenada);
            renderMapListings(listaOrdenada);
        });
    }

    // 8. EVENTOS DE BOTONES
    btnLista.addEventListener('click', () => {
        btnLista.classList.add('active'); btnMapa.classList.remove('active');
        listaView.style.display = 'flex'; mapaView.style.display = 'none';
    });
    btnMapa.addEventListener('click', () => {
        btnMapa.classList.add('active'); btnLista.classList.remove('active');
        listaView.style.display = 'none'; mapaView.style.display = 'flex';
        initMap();
    });

    // 9. ESCUCHAR CAMBIO DE IDIOMA
    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            setTimeout(() => {
                renderListings(currentData);
                renderMapListings(currentData);
            }, 50);
        });
    });

    function attachSaveEvents() {
        document.querySelectorAll('.btn-save').forEach(btn => {
            btn.addEventListener('click', function() {
                this.style.color = (this.style.color === 'red') ? '' : 'red';
            });
        });
    }

    // 10. ARRANQUE FINAL
    renderListings();
    renderMapListings();
});
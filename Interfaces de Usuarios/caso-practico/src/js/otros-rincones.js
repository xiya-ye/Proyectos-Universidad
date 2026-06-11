document.addEventListener('DOMContentLoaded', () => {

    // --- 1. GESTIÓN DE USUARIO (Mantenemos tu lógica) ---
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

    // --- 2. REFERENCIAS DOM ---
    const gridContainer = document.getElementById('dynamic-cities-grid');
    const searchInput = document.querySelector('.filter-search');
    const filterSelect = document.querySelector('.filter-select');
    const filterTags = document.querySelectorAll('.filter-tag');
    const noResultsMsg = document.getElementById('no-results-msg');

    let allCityCards = []; // Guardaremos referencias a las tarjetas aquí
    let currentCategory = 'todos';
    let currentSearch = '';

    // --- 3. CARGAR DATOS DEL JSON ---
    // Ajusta la ruta si tu json está en otra carpeta, ej: '../data/ciudades-del-mundo.json'
    fetch('/js/bases-datos/ciudades-del-mundo.json')
        .then(response => response.json())
        .then(data => {
            renderCities(data);
        })
        .catch(error => console.error('Error cargando ciudades:', error));


    // --- 4. FUNCIÓN PARA PINTAR LAS TARJETAS ---
    function renderCities(data) {
        gridContainer.innerHTML = ''; // Limpiar por si acaso

        // El JSON tiene estructura: Continentes -> Países -> Ciudades
        // Necesitamos 3 bucles anidados para sacar las ciudades
        data.continents.forEach(continent => {
            continent.countries.forEach(country => {
                country.cities.forEach(city => {
                    
                    // Crear el HTML de la tarjeta
                    const card = document.createElement('article');
                    card.classList.add('city-card');
                    
                    // Guardamos datos clave en atributos data- para filtrar rápido luego
                    card.setAttribute('data-continent', continent.name.toLowerCase());
                    card.setAttribute('data-country', country.name.toLowerCase());
                    card.setAttribute('data-city', city.name.toLowerCase());

                    // Construimos el contenido
                    // Usamos city.image.url y city.image.alt del JSON
                    card.innerHTML = `
                        <div class="city-image-container">
                            <img src="${city.image.url}" alt="${city.image.alt}" class="city-img" loading="lazy">
                        </div>
                        <div class="city-content">
                            <h4>${city.name}</h4>
                            <span class="city-meta">
                                ${country.name} · ${continent.name}
                            </span>
                            <p class="city-desc">${city.description}</p>
                        </div>
                    `;

                    gridContainer.appendChild(card);
                });
            });
        });

        // Una vez creadas, actualizamos nuestra referencia
        allCityCards = document.querySelectorAll('.city-card');
    }


    // --- 5. LÓGICA DE FILTRADO ---
    function applyFilters() {
        const searchText = currentSearch.toLowerCase().trim();
        let visibleCount = 0;

        allCityCards.forEach(card => {
            // Recuperamos datos del DOM
            const continent = card.getAttribute('data-continent');
            const country = card.getAttribute('data-country');
            const city = card.getAttribute('data-city');
            const desc = card.querySelector('.city-desc').textContent.toLowerCase();

            // 1. Filtro de TEXTO (busca en ciudad, país o descripción)
            const matchesSearch = 
                city.includes(searchText) || 
                country.includes(searchText) || 
                desc.includes(searchText);

            

            // 2. Filtro de CATEGORÍA (Continente)
            let matchesCategory = false;
            if (currentCategory === 'todos') {
                matchesCategory = true;
            } else {
                const safeContinent = continent.normalize("NFD").replace(/[\u0300-\u036f]/g, "");
                const safeCategory = currentCategory.normalize("NFD").replace(/[\u0300-\u036f]/g, "");
                
                matchesCategory = (safeContinent === safeCategory);
            }

            // MOSTRAR U OCULTAR
            if (matchesSearch && matchesCategory) {
                card.style.display = 'flex'; // O 'block'
                visibleCount++;
            } else {
                card.style.display = 'none';
            }
        });

        // Estado vacío
        if (visibleCount === 0) {
            noResultsMsg.style.display = 'block';
        } else {
            noResultsMsg.style.display = 'none';
        }
    }


    // --- 6. EVENT LISTENERS ---

    // A. Buscador
    searchInput.addEventListener('input', (e) => {
        currentSearch = e.target.value;
        applyFilters();
    });

    // B. Tags (Botones)
    filterTags.forEach(tag => {
        tag.addEventListener('click', () => {
            // Visual
            filterTags.forEach(t => t.classList.remove('active'));
            tag.classList.add('active');

       
            const val = tag.getAttribute('data-val') || tag.textContent.toLowerCase();
            currentCategory = val;

            // Sincronizar select (opcional)
            filterSelect.value = currentCategory === 'todos' ? 'todos' : currentCategory;
            if(currentCategory === 'todos') filterSelect.value = 'todos';

            applyFilters();
        });
    });

    // C. Dropdown (Select)
    filterSelect.addEventListener('change', (e) => {
        currentCategory = e.target.value;

        // Sincronizar tags
        filterTags.forEach(tag => {
            const tagVal = tag.getAttribute('data-val');
            if (tagVal === currentCategory) {
                tag.classList.add('active');
            } else {
                tag.classList.remove('active');
            }
        });

        applyFilters();
    });

});
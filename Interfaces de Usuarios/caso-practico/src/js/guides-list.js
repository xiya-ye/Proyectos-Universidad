document.addEventListener('DOMContentLoaded', function() {

    // Buscar el enlace "Mi cuenta"
    const accountLink = document.querySelector('.user-link');
    
    if (accountLink) {
        // Comprobar si hay usuario
        const currentUser = localStorage.getItem('msf_currentUser');
        
        if (currentUser) {
            // Si hay usuario, cambiamos el enlace al dashboard
            accountLink.href = 'account-dashboard.html';
            accountLink.textContent = 'Mi cuenta';
        } else {
            // Si no, lo dejamos como estaba (login.html)
            accountLink.href = 'login.html';
        }
    }    

    if (typeof guias === 'undefined') return;
    const guiasData = guias;

    // --- FUNCIÓN PARA ELEGIR IDIOMA ---
    const currentLang = localStorage.getItem('selectedLang') || 'es';

    function getText(data) {
        if (typeof data === 'object' && data !== null && !Array.isArray(data)) {
            // Si es un objeto {es: "...", en: "..."}, devolvemos el idioma actual
            return data[currentLang] || data['es'];
        }
        // Si es texto plano (ej: "Egipto"), lo devolvemos tal cual
        return data;
    }
    // ------------------------------------------

    const container = document.querySelector('.guides-list-container');
    const inputSearch = document.querySelector('.filter-input-search');
    const selectDestino = document.querySelectorAll('.filter-select')[0];
    const selectTipo = document.querySelectorAll('.filter-select')[1];
    const selectDuracion = document.querySelectorAll('.filter-select')[2];
    const tags = document.querySelectorAll('.tag');
    const paginationContainer = document.querySelector('.pagination');

    let currentFilters = { search: "", destino: "Todos", tipo: "Todos", categoria: "Todos", duracion: "Todos" };
    let currentPage = 1;
    const itemsPerPage = 9;
    let filteredData = []; 

    function poblarSelects() {
        const destinos = [...new Set(guiasData.map(g => g.destino))].sort();
        // Para 'tipo' usamos getText porque puede venir traducido
        const tipos = [...new Set(guiasData.map(g => getText(g.tipo)))].sort();

        selectDestino.innerHTML = '<option value="Todos">Todos los destinos...</option>';
        destinos.forEach(d => selectDestino.innerHTML += `<option value="${d}">${d}</option>`);

        selectTipo.innerHTML = '<option value="Todos">Tipo de contenido...</option>';
        tipos.forEach(t => selectTipo.innerHTML += `<option value="${t}">${t}</option>`);

        selectDuracion.innerHTML = `<option value="Todos">Duración...</option><option value="corto">Corta (< 10 min)</option><option value="largo">Larga (> 10 min)</option>`;
    }

    function aplicarFiltros() {
        filteredData = guiasData.filter(guia => {
            // Usamos getText() para poder filtrar aunque esté en inglés/italiano
            const titulo = getText(guia.titulo).toLowerCase();
            const desc = getText(guia.desc).toLowerCase();
            const tipo = getText(guia.tipo);
            const categoria = getText(guia.categoria); 

            const searchText = currentFilters.search.toLowerCase();
            const matchSearch = titulo.includes(searchText) || desc.includes(searchText);
            
            const matchDestino = currentFilters.destino === "Todos" || guia.destino === currentFilters.destino;
            const matchTipo = currentFilters.tipo === "Todos" || tipo === currentFilters.tipo;
            const matchCategoria = currentFilters.categoria === "Todos" || categoria === currentFilters.categoria;
            
            let matchDuracion = true;
            const minutos = parseInt(guia.lectura);
            if (currentFilters.duracion === "corto") matchDuracion = minutos < 10;
            else if (currentFilters.duracion === "largo") matchDuracion = minutos >= 10;

            return matchSearch && matchDestino && matchTipo && matchCategoria && matchDuracion;
        });

        currentPage = 1;
        renderPagination();
        renderGuiasPage();
    }


    // 5. RENDERIZADO PRINCIPAL (ACTUALIZADO)

    function renderGuiasPage() {
        container.innerHTML = '';

        if (filteredData.length === 0) {
            container.innerHTML = `<div style="text-align:center; padding:3rem;"><h3>No se encontraron resultados 😔</h3></div>`;
            return;
        }

        const start = (currentPage - 1) * itemsPerPage;
        const end = start + itemsPerPage;
        const pageItems = filteredData.slice(start, end);

        pageItems.forEach(guia => {
            const html = `
                <article class="guide-card"> 
                    <div class="guide-image-container">
                        <img src="${guia.imagen}" alt="${guia.titulo}" class="guide-image">
                    </div>
                    <div class="guide-content">
                        <div class="guide-meta-top">${getText(guia.tipo)} · ${guia.destino}</div>
                        <h3>${getText(guia.titulo)}</h3>
                        <p class="guide-description">${getText(guia.desc)}</p>
                        <div class="guide-tags-bottom">
                            ${guia.tags.map(tag => `<span class="feature-tag">${tag}</span>`).join('')}
                        </div>
                    </div>
                    <div class="guide-sidebar">
                        <div class="sidebar-top">
                            <div class="guide-reading-time">Lectura: ${guia.lectura}</div>
                            <div class="rating-small">★ ${guia.valoracion}</div>
                        </div>
                        
                        <button class="btn-leer-guia" onclick="window.location.href='guide-article.html?id=${guia.id}'" data-i18n="btn_read_guide">Leer guía</button>
                    </div>
                </article>
            `;
            container.innerHTML += html;
        });
        
        const resultsText = document.querySelector('.results-count-row span');
        if(resultsText) resultsText.innerHTML = `${filteredData.length} resultados`;

        // --- FORZAR TRADUCCIÓN ---
        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }
    }

    function renderPagination() {

        const totalPages = Math.ceil(filteredData.length / itemsPerPage);
        paginationContainer.innerHTML = '';
        if (totalPages <= 1) return;
        for (let i = 1; i <= totalPages; i++) {
            const btn = document.createElement('button');
            btn.className = `page-btn ${i === currentPage ? 'active' : ''}`;
            btn.innerText = i;
            btn.addEventListener('click', () => { currentPage = i; renderGuiasPage(); renderPagination(); window.scrollTo(0,0); });
            paginationContainer.appendChild(btn);
        }
    }

 
    // 6. RENDERIZADO DE EXTRAS (FOOTER WIDGETS)

    function renderFooterWidgets() {
        
        // A. COLECCIONES DESTACADAS
        // Usamos IDs que SÍ existen en tu archivo info-guias.js (31, 35, 33, 32)
        const collectionIds = [31, 35, 33, 32]; 
        const collectionsList = document.querySelector('.collections-list');
        
        if (collectionsList) {
            collectionsList.innerHTML = '';
            collectionIds.forEach(id => {
                const guia = guiasData.find(g => g.id === id);
                // Usamos getText() para traducir el título
                if (guia) {
                    collectionsList.innerHTML += `<li><a href="guide-article.html?id=${guia.id}">➤ ${getText(guia.titulo)}</a></li>`;
                }
            });
        }

        // B. GUÍAS POPULARES
        // Usamos IDs que SÍ existen (1, 12, 21, 11)
        const popularIds = [1, 12, 21, 11];
        const popularList = document.querySelector('.popular-items-list');

        if (popularList) {
            popularList.innerHTML = ''; 
            popularIds.forEach(id => {
                const guia = guiasData.find(g => g.id === id);
                // Aquí aplicamos getText() para título, valoración y lectura
                if (guia) {
                    popularList.innerHTML += `
                        <div class="pop-item">
                            <a href="guide-article.html?id=${guia.id}" class="pop-title">${getText(guia.titulo)}</a>
                            <span class="pop-meta">${guia.valoracion} ★ · ${guia.lectura} lectura</span>
                        </div>
                    `;
                }
            });
        }
    }

    // EVENTOS
    inputSearch.addEventListener('input', (e) => { currentFilters.search = e.target.value; aplicarFiltros(); });
    selectDestino.addEventListener('change', (e) => { currentFilters.destino = e.target.value; aplicarFiltros(); });
    selectTipo.addEventListener('change', (e) => { currentFilters.tipo = e.target.value; aplicarFiltros(); });
    selectDuracion.addEventListener('change', (e) => { currentFilters.duracion = e.target.value; aplicarFiltros(); });

    tags.forEach(tag => {
        tag.addEventListener('click', function() {
            tags.forEach(t => t.classList.remove('active'));
            this.classList.add('active');
            const textoTag = this.textContent.trim();
            currentFilters.categoria = (textoTag === "Todos") ? "Todos" : textoTag;
            aplicarFiltros();
        });
    });

    // FUNCIONALIDAD SUSCRIPCIÓN 
    const btnSuscribir = document.querySelector('.btn-suscribir');
    const modal = document.getElementById('subscribeModal');
    const closeModal = document.querySelector('.close-modal');
    const formSubscribe = document.getElementById('subscribeForm');

    if (btnSuscribir && modal) btnSuscribir.addEventListener('click', () => modal.classList.add('active'));
    if (closeModal) closeModal.addEventListener('click', () => modal.classList.remove('active'));
    window.addEventListener('click', (e) => { if (e.target == modal) modal.classList.remove('active'); });
    if (formSubscribe) {
        formSubscribe.addEventListener('submit', (e) => {
            e.preventDefault();
            alert("¡Suscrito!");
            modal.classList.remove('active');
        });
    }

    // ESCUCHAR CAMBIO DE IDIOMA 
    // Esto asume que tienes los botones .lang-option en tu header
    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            // Esperamos un pelín a que language.js cambie el localStorage
            setTimeout(() => {
                location.reload(); // En este caso, recargar es lo más fácil para refrescar todo
            }, 100);
        });
    });

    // ARRANQUE
    poblarSelects();
    aplicarFiltros();
    renderFooterWidgets();
});
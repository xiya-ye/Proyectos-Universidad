document.addEventListener('DOMContentLoaded', () => {

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

    const params = new URLSearchParams(window.location.search);
    const guideId = parseInt(params.get('id'));

    if (typeof guias === 'undefined') return;
    const guia = guideId ? guias.find(g => g.id === guideId) : guias[0];

    if (!guia) {
        window.location.href = 'guides-list.html';
        return;
    }

    // --- FUNCIÓN HELPER IDIOMA ---
    const currentLang = localStorage.getItem('selectedLang') || 'es';
    function getText(data) {
        if (typeof data === 'object' && data !== null && !Array.isArray(data)) {
            return data[currentLang] || data['es'];
        }
        return data;
    }
    // ----------------------------

    // Breadcrumbs
    const breadcrumbDestino = document.querySelector('.breadcrumbs a:nth-child(3)');
    const breadcrumbTitulo = document.querySelector('.breadcrumbs span');
    if(breadcrumbDestino) breadcrumbDestino.textContent = guia.destino;
    if(breadcrumbTitulo) breadcrumbTitulo.textContent = getText(guia.titulo);

    // Header
    document.querySelector('.article-header h1').textContent = getText(guia.titulo);
    document.querySelector('.subtitle').textContent = getText(guia.detalle.subtitulo);
    
    // Meta tags
    const tagsRow = document.querySelector('.tags-row');
    tagsRow.innerHTML = `<span class="meta-tag">${getText(guia.tipo)}</span> <span class="meta-tag">${guia.destino}</span>`;
    
    document.querySelector('.reading-info').innerHTML = `<span>Lectura: ${guia.lectura}</span> <span>★ ${guia.valoracion}</span>`;
    document.querySelector('.hero-image').src = guia.imagen;
    document.querySelector('.intro-text').innerHTML = getText(guia.detalle.intro);

// --- ITINERARIO ---
    const timelineContainer = document.querySelector('.timeline');
    if (guia.detalle.itinerario && timelineContainer) {
        timelineContainer.innerHTML = '';
        guia.detalle.itinerario.forEach(item => {
            const html = `
                <div class="timeline-item">
                    <div class="day-badge">
                        <span data-i18n="article_day">Día</span> ${item.dia}
                    </div>
                    <div class="timeline-content">
                        <h4>${getText(item.titulo)}</h4>
                        <p>${getText(item.desc)}</p>
                    </div>
                </div>
            `;
            timelineContainer.insertAdjacentHTML('beforeend', html);
        });
        
        // ¡IMPORTANTE! Forzamos la traducción de lo que acabamos de pintar
        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }
    }

   
    // Si la guía está traducida, 'consejos' debería ser un objeto {es: [], en: []}
    // Si no, es un array directo []
    const tipsList = document.querySelector('.check-list');
    if (guia.detalle.consejos && tipsList) {
        tipsList.innerHTML = '';
        
        let consejosArray = [];
        if (Array.isArray(guia.detalle.consejos)) {
            consejosArray = guia.detalle.consejos; // Formato antiguo
        } else {
            consejosArray = guia.detalle.consejos[currentLang] || guia.detalle.consejos['es']; // Formato nuevo
        }

        consejosArray.forEach(tip => {
            tipsList.insertAdjacentHTML('beforeend', `<li>${tip}</li>`);
        });
    }

    // --- MALETA ---
    const climateText = document.querySelector('.climate-box p');
    if(climateText) climateText.textContent = getText(guia.detalle.maleta.clima);

    const backpackList = document.querySelector('.backpack-box .simple-list');
    if (guia.detalle.maleta.items && backpackList) {
        backpackList.innerHTML = '';
        
        let itemsArray = [];
        if (Array.isArray(guia.detalle.maleta.items)) {
            itemsArray = guia.detalle.maleta.items;
        } else {
            itemsArray = guia.detalle.maleta.items[currentLang] || guia.detalle.maleta.items['es'];
        }

        itemsArray.forEach(item => {
            backpackList.insertAdjacentHTML('beforeend', `<li>${item}</li>`);
        });
    }

    // --- SIDEBAR ---
    const infoList = document.querySelector('.info-list');
    if (infoList && guia.detalle.sidebar) {
        infoList.innerHTML = `
            <li>
                <span data-i18n="sidebar_duration">Duración:</span> 
                <strong>${getText(guia.detalle.sidebar.duracion)}</strong>
            </li>
            <li>
                <span data-i18n="sidebar_season">Mejor época:</span> 
                <strong>${getText(guia.detalle.sidebar.epoca)}</strong>
            </li>
            <li>
                <span data-i18n="sidebar_budget">Presupuesto:</span> 
                <strong>${guia.detalle.sidebar.presupuesto}</strong>
            </li>
            <li>
                <span data-i18n="sidebar_intensity">Intensidad:</span> 
                <strong>${getText(guia.detalle.sidebar.intensidad)}</strong>
            </li>
        `;
        
        // Forzamos la traducción de estas etiquetas nuevas
        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }   
    }

    // Guías relacionadas
    const relatedContainer = document.querySelector('.related-grid');
    
    if (relatedContainer) {
        // 1. Buscar guías del MISMO destino (excluyendo la actual)
        let related = guias.filter(g => g.id !== guia.id && g.destino === guia.destino);

        // 2. Si tenemos menos de 3, rellenamos con otras guías cualesquiera
        if (related.length < 3) {
            const others = guias.filter(g => g.id !== guia.id && g.destino !== guia.destino);
            // Concatenamos y cortamos para tener máximo 3
            related = related.concat(others);
        }
        
        // Nos quedamos solo con las 3 primeras
        const selection = related.slice(0, 3);

        // 3. Limpiar el HTML estático y pintar el dinámico
        relatedContainer.innerHTML = '';
        
        selection.forEach(item => {
            const html = `
                <a href="guide-article.html?id=${item.id}" class="related-card">
                    <img src="${item.imagen}" alt="Imagen guía">
                    <div class="related-content">
                        <h4>${getText(item.titulo)}</h4>
                        <span data-i18n="link_read_article">Leer artículo ›</span>
                    </div>
                </a>
            `;
            relatedContainer.insertAdjacentHTML('beforeend', html);
        });

        // 4. Traducir los textos fijos ("Leer artículo")
        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }
    }

    // Widget Twitter
    const tweetName = document.getElementById('tweetAuthor');
    const tweetUser = document.querySelector('.tweet-header div span');
    const tweetAvatar = document.querySelector('.tweet-avatar');

    if (guia.detalle.autor) {
        // 1. Nombre del autor (ej: Egypt Tourism Board)
        if(tweetName) tweetName.textContent = guia.detalle.autor;
        
        // 2. Usuario (ej: @EgyptTourismBoard)
        if(tweetUser) tweetUser.textContent = "@" + guia.detalle.autor.replace(/\s/g, '');
        
        // 3. Letra del avatar (ej: E)
        if(tweetAvatar) tweetAvatar.textContent = guia.detalle.autor.charAt(0);
    }

    // Funciones visuales 
    initArticleFeatures();
    
    // Escuchar cambio de idioma
    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => setTimeout(() => location.reload(), 100));
    });
});

function initArticleFeatures() {
    const progressBar = document.createElement('div');
    progressBar.className = 'reading-progress-bar';
    document.body.appendChild(progressBar);
    window.addEventListener('scroll', () => {
        const totalHeight = document.body.scrollHeight - window.innerHeight;
        const progress = (window.pageYOffset / totalHeight) * 100;
        progressBar.style.width = `${progress}%`;
    });
}
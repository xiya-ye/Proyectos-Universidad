// Funcionalidad para el formulario de búsqueda
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

    // Manejo del formulario de búsqueda
    const searchForm = document.querySelector('.search-form');
    if (searchForm) {
        searchForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // 1. Capturar los valores de los inputs
            // Usamos querySelector por el orden o atributos específicos
            const inputs = this.querySelectorAll('input[type="text"]');
            const selects = this.querySelectorAll('select');
            
            const destino = inputs[0].value || 'El Cairo'; // Valor o defecto
            const fechas = inputs[1].value || 'Fechas flexibles';
            
            // Para los select, cogemos el texto de la opción seleccionada, no el value (para que quede bonito)
            const personasSelect = selects[0];
            const personas = personasSelect.options[personasSelect.selectedIndex].text;
            
            // 2. Crear los parámetros de URL
            const params = new URLSearchParams();
            params.append('destination', destino);
            params.append('dates', fechas);
            params.append('people', personas);
            
            // 3. Redirigir a la página de resultados con los datos en la mochila
            window.location.href = `alojamiento.html?${params.toString()}`;
        });
    }
    
    // Funcionalidad para los tags de búsqueda rápida
    const tags = document.querySelectorAll('.tag');
    tags.forEach(tag => {
        tag.addEventListener('click', function() {
            console.log('Tag seleccionado:', this.textContent);
            // Aquí puedes agregar lógica para filtrar según el tag
        });
    });
    
    // Funcionalidad para los botones de las cards
    const cardButtons = document.querySelectorAll('.card-btn');
    cardButtons.forEach(btn => {
        btn.addEventListener('click', function() {
            const card = this.closest('.card');
            const title = card.querySelector('h3').textContent;
            console.log('Ver detalles de:', title);
            // Redirigir a página de detalles
        });
    });
    
    // Funcionalidad para los botones del foro
    const forumButtons = document.querySelectorAll('.btn-ver-hilo');
    forumButtons.forEach(btn => {
        btn.addEventListener('click', function() {
            const forumItem = this.closest('.forum-item');
            const title = forumItem.querySelector('h3').textContent;
            console.log('Ver hilo:', title);
            // Redirigir a página del hilo
        });
    });
    
    // Smooth scroll para los enlaces de navegación
    const navLinks = document.querySelectorAll('nav a[href^="#"]');
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href');
            const targetSection = document.querySelector(targetId);
            
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Animación de aparición para las cards
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };
    
    const observer = new IntersectionObserver(function(entries) {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, observerOptions);
    
    const cards = document.querySelectorAll('.card');
    cards.forEach(card => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        card.style.transition = 'opacity 0.5s ease, transform 0.5s ease';
        observer.observe(card);
    });

    // ==============================================
    // CARGAR HILOS DEL FORO EN LA HOME
    // ==============================================
    const forumContainer = document.getElementById('home-forum-container');

    if (forumContainer) {
        // Obtener hilos de la Base de Datos (info-foro.js)
        const dbThreads = (typeof forohilos !== 'undefined') ? forohilos : [];

        // Obtener hilos nuevos creados por el usuario (LocalStorage)
        let localThreads = [];
        try {
            localThreads = JSON.parse(localStorage.getItem('foro_hilos') || '[]');
        } catch (e) {
            console.warn('Error leyendo localStorage del foro', e);
        }

        const allThreads = [...localThreads, ...dbThreads];

        const threadsToShow = allThreads.slice(0, 3);

        // Generar el HTML
        forumContainer.innerHTML = '';

        threadsToShow.forEach(hilo => {
            const titulo = hilo.titulo || 'Sin título';
            const autor = hilo.autor ? (hilo.autor.nombre || hilo.autor) : 'Anónimo';
            const fecha = hilo.fechaCreacion || hilo.fecha || 'Reciente';
            
            const tempDiv = document.createElement("div");
            tempDiv.innerHTML = hilo.contenido || '';
            const textoPlano = tempDiv.textContent || tempDiv.innerText || '';
            const resumen = textoPlano.substring(0, 100) + '...';

            const html = `
                <div class="forum-item">
                    <div class="forum-content">
                        <h3>${titulo}</h3>
                        <p>${resumen}</p>
                        <div class="forum-meta">
                            Por <strong>${autor}</strong> · ${fecha}
                        </div>
                    </div>
                    <a href="foro-hilo.html?id=${hilo.id}" class="btn-ver-hilo" data-i18n="btn_view_thread">VER HILO</a>
                </div>
            `;
            forumContainer.insertAdjacentHTML('beforeend', html);
        });

        if (window.updateLanguage) {
            const currentLang = localStorage.getItem('selectedLang') || 'es';
            window.updateLanguage(currentLang);
        }
    }

    // ==============================================
    // CARGAR GUÍAS EN LA HOME
    // ==============================================
    
    function renderHomeGuides() {
        const guidesContainer = document.getElementById('home-guides-container');
        if (!guidesContainer) return;
        const currentLang = localStorage.getItem('selectedLang') || 'es';
        const dbGuides = (typeof guias !== 'undefined') ? guias : [];
        const guidesToShow = dbGuides.slice(10, 13);
        guidesContainer.innerHTML = '';
        guidesToShow.forEach(guia => {
            const titulo = guia.titulo[currentLang] || guia.titulo['es'];
            const descFull = guia.desc[currentLang] || guia.desc['es'];
            
            const descCorta = descFull.length > 100 ? descFull.substring(0, 100) + '...' : descFull;

            const html = `
                <div class="card">
                    <img src="${guia.imagen}" alt="${titulo}" class="card-image" loading="lazy">
                    <div class="card-content">
                        <h3>${titulo}</h3>
                        <p>${descCorta}</p>
                        <a href="guide-article.html?id=${guia.id}" class="card-btn" data-i18n="btn_read_guide">LEER GUÍA</a>
                    </div>
                </div>
            `;
            guidesContainer.insertAdjacentHTML('beforeend', html);
        });

        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }
    }

    // Ejecutar al cargar la página
    renderHomeGuides();

    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            setTimeout(renderHomeGuides, 50);
        });
    });

    // ==============================================
    // CARGAR ALOJAMIENTOS DESTACADOS EN LA HOME
    // ==============================================
    
    function renderHomeAccommodations() {
        const container = document.getElementById('home-accommodations-container');
        if (!container) return;

        const currentLang = localStorage.getItem('selectedLang') || 'es';
        
        const dbAlojamientos = (typeof alojamientos !== 'undefined') ? alojamientos : [];

        const destacados = dbAlojamientos.filter(a => a.rating >= 4.7).slice(0, 3);
        container.innerHTML = '';

        destacados.forEach(hotel => {
            const descFull = hotel.descripcion[currentLang] || hotel.descripcion['es'];
            const descCorta = descFull.length > 80 ? descFull.substring(0, 80) + '...' : descFull;

            const html = `
                <div class="card">
                    <img src="${hotel.imagenPrincipal}" alt="${hotel.nombre}" class="card-image" loading="lazy">
                    <div class="card-content">
                        <h3>${hotel.nombre}</h3>
                        <p>${descCorta}</p>
                        
                        <div class="rating" style="margin-bottom: 10px; color: #ffc107;">
                            <span class="stars">★</span>
                            <span style="color: #666; font-size: 0.9rem;">${hotel.rating} (${hotel.resenas})</span>
                        </div>

                        <a href="alojamiento-detalle.html?id=${hotel.id}" class="card-btn" data-i18n="btn_view">VER</a>
                    </div>
                </div>
            `;
            container.insertAdjacentHTML('beforeend', html);
        });

        if (window.updateLanguage) {
            window.updateLanguage(currentLang);
        }
    }

    // Ejecutar al cargar
    renderHomeAccommodations();
    const langBtnsUpdate = document.querySelectorAll('.lang-option');
    langBtnsUpdate.forEach(btn => {
        btn.addEventListener('click', () => {
            setTimeout(() => {
                renderHomeGuides();
                renderHomeAccommodations();
            }, 50);
        });
    });
});
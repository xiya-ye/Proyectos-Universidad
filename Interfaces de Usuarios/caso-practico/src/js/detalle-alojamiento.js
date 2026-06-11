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


    // 1. VARIABLES Y CARGA INICIAL

    const params = new URLSearchParams(window.location.search);
    const hotelId = parseInt(params.get('id'));
    
    // Elementos Reserva
    const checkinInput = document.getElementById('checkinInput');
    const checkoutInput = document.getElementById('checkoutInput');
    const peopleInput = document.getElementById('peopleInput');
    const totalPriceEl = document.querySelector('.total-price');
    const bookingDetailsEl = document.querySelector('.booking-details');

    // Elementos Modal Reseña
    const modal = document.getElementById('reviewModal');
    const btnOpenModal = document.querySelector('.reviews-buttons .btn-primary'); // Botón "Escribir reseña"
    const btnCloseModal = document.querySelector('.close-modal');
    const reviewForm = document.getElementById('reviewForm');

    // Verificar datos
    if (typeof alojamientos === 'undefined') {
        console.error("Error: No se ha cargado el archivo info-alojamientos.js");
        return;
    }

    const hotel = hotelId ? alojamientos.find(h => h.id === hotelId) : alojamientos[0];

    if (!hotel) {
        window.location.href = 'alojamiento.html';
        return;
    }


    // 2. RELLENAR LA PÁGINA
  
    const currentLang = localStorage.getItem('selectedLang') || 'es';

    document.querySelector('.accommodation-title').textContent = hotel.nombre;
    const breadcrumbTitle = document.querySelector('.breadcrumb span:last-child');
    if(breadcrumbTitle) breadcrumbTitle.textContent = hotel.nombre;

    document.querySelector('.location').textContent = hotel.ubicacion;
    document.querySelector('.price').textContent = hotel.precio + ' €';
    
    const descText = hotel.descripcion[currentLang] || hotel.descripcion['es'];
    document.querySelector('.description-text').textContent = descText;
    
    // Imágenes
    const mainImage = document.querySelector('.main-image');
    if(mainImage) mainImage.src = hotel.imagenPrincipal;
    
    const thumbnails = document.querySelectorAll('.thumbnail');
    if (hotel.imagenes && hotel.imagenes.length > 0) {
        thumbnails.forEach((thumb, index) => {
            if (hotel.imagenes[index]) {
                thumb.src = hotel.imagenes[index];
                thumb.onclick = function() {
                    thumbnails.forEach(t => t.classList.remove('active'));
                    this.classList.add('active');
                    mainImage.src = this.src.replace('w=150', 'w=800');
                };
            }
        });
    }

    // Servicios
    const servicesGrid = document.querySelector('.services-grid');
    if(servicesGrid && hotel.servicios) {
        servicesGrid.innerHTML = '';
        const listaServicios = hotel.servicios[currentLang] || hotel.servicios['es'];
        listaServicios.forEach(servicio => {
            const div = document.createElement('div');
            div.className = 'service-item';
            div.textContent = '· ' + servicio;
            servicesGrid.appendChild(div);
        });
    }

   
    // 3. GESTIÓN DE RESEÑAS (ESTÁTICAS + LOCALSTORAGE)

    
    // Función para leer reseñas guardadas en el navegador
    function getStoredReviews() {
        const stored = localStorage.getItem(`reviews_${hotel.id}`);
        return stored ? JSON.parse(stored) : [];
    }

    // Función para renderizar todas las reseñas (Base de datos + Nuevas)
    function renderReviews() {
        const reviewsSection = document.querySelector('.reviews-section');
        if (!reviewsSection) return;

        // 1. Combinar reseñas: Las del archivo JS + Las que guardó el usuario
        const storedReviews = getStoredReviews();

        const allReviews = [...(hotel.listaResenas || []), ...storedReviews];

        // 2. Actualizar cabecera
        const totalReviews = hotel.resenas + storedReviews.length;
        const reviewsHeaderInfo = reviewsSection.querySelector('.reviews-header p');
        if (reviewsHeaderInfo) {
            reviewsHeaderInfo.innerHTML = `${totalReviews} <span data-i18n="reviews_count">reseñas</span> · <span data-i18n="avg_note">Nota media</span> ${hotel.rating}/5`;
        }

        // Actualizar Badge de arriba también
        const ratingBadge = document.querySelector('.rating-badge');
        if(ratingBadge) {
            ratingBadge.innerHTML = `
                <span class="rating-number">${hotel.rating}</span> 
                <span><span data-i18n="rate_excellent">Excelente</span> · ${totalReviews} <span data-i18n="reviews_count">reseñas</span></span>
            `;
        }

        // 3. Limpiar y Repintar
        const header = reviewsSection.querySelector('.reviews-header');
        reviewsSection.innerHTML = ''; 
        if(header) reviewsSection.appendChild(header);

        allReviews.forEach(review => {
            
            // Así que comprobamos si es objeto o string.
            
            let comentario = "";
            let fecha = "";
            let tipo = "";

            if (typeof review.comentario === 'object') {
                // Es del archivo info-alojamientos.js (tiene idiomas)
                comentario = review.comentario[currentLang] || review.comentario['es'];
                fecha = review.fecha[currentLang] || review.fecha['es'];
                tipo = review.tipoViaje[currentLang] || review.tipoViaje['es'];
            } else {
                // Es una reseña nueva (Localstorage) - Texto plano
                comentario = review.comentario;
                fecha = review.fecha;
                tipo = review.tipoViaje;
            }

            const stars = '★'.repeat(Math.floor(review.rating));

            const html = `
                <div class="review-item">
                    <div class="review-header">
                        <div class="reviewer-info">
                            <div class="reviewer-avatar">${review.usuario.charAt(0)}</div>
                            <div>
                                <p class="reviewer-name">${review.usuario}</p>
                                <p class="review-date">${fecha}</p>
                            </div>
                        </div>
                        <div class="review-rating">
                            <span style="color:#ffc107; margin-right:5px;">${stars}</span>
                            <span class="rating-value">${review.rating}</span>
                        </div>
                    </div>
                    <p class="review-text">${comentario}</p>
                    <button class="btn-link" style="margin-top:10px; font-size:12px; color:#666; text-decoration:underline; border:none; background:none; cursor:pointer;">${tipo}</button>
                </div>
            `;
            reviewsSection.insertAdjacentHTML('beforeend', html);
        });
        
        // Traducir los elementos nuevos
        if (window.updateLanguage) window.updateLanguage(currentLang);
    }

    // Inicializar reseñas
    renderReviews();

 
    // 4. LÓGICA DEL MODAL (ESCRIBIR RESEÑA)
   
    
    // Abrir modal
    if(btnOpenModal) {
        btnOpenModal.addEventListener('click', () => {
            modal.classList.add('active');
        });
    }

    // Cerrar modal (X)
    if(btnCloseModal) {
        btnCloseModal.addEventListener('click', () => {
            modal.classList.remove('active');
        });
    }

    // Cerrar si clic fuera
    window.addEventListener('click', (e) => {
        if (e.target == modal) modal.classList.remove('active');
    });

    // ENVIAR FORMULARIO
    if(reviewForm) {
        reviewForm.addEventListener('submit', (e) => {
            e.preventDefault();

            const name = document.getElementById('reviewName').value;
            const type = document.getElementById('reviewType').value;
            const rating = parseInt(document.getElementById('reviewRating').value);
            const text = document.getElementById('reviewText').value;
            
            // Fecha actual formateada
            const today = new Date();
            const dateStr = today.toLocaleDateString('es-ES', { month: 'short', year: 'numeric' });

            // Crear objeto reseña (Simplificado, sin traducción múltiple)
            const newReview = {
                usuario: name,
                fecha: dateStr, 
                rating: rating,
                comentario: text, // Guardamos texto directo
                tipoViaje: type
            };

            // Guardar en LocalStorage
            const stored = getStoredReviews();
            stored.unshift(newReview); // Añadir al principio
            localStorage.setItem(`reviews_${hotel.id}`, JSON.stringify(stored));

            // Recargar reseñas y cerrar modal
            renderReviews();
            modal.classList.remove('active');
            reviewForm.reset();
            alert("¡Gracias! Tu reseña se ha publicado.");
        });
    }



    // 5. LÓGICA DE RESERVA Y PRECIOS


    function setDefaultDates() {
        const today = new Date();
        const tomorrow = new Date(today);
        tomorrow.setDate(tomorrow.getDate() + 7); 

        if (checkinInput) checkinInput.value = today.toISOString().split('T')[0];
        if (checkoutInput) checkoutInput.value = tomorrow.toISOString().split('T')[0];
        
        const peopleParam = params.get('people');
        if(peopleParam && peopleInput) {
            const num = parseInt(peopleParam); 
            peopleInput.value = isNaN(num) ? 2 : num;
        }
    }

    function calculateTotal() {
        if (!checkinInput || !checkoutInput || !peopleInput) return;

        const date1 = new Date(checkinInput.value);
        const date2 = new Date(checkoutInput.value);
        const personas = parseInt(peopleInput.value) || 1;

        const diffTime = date2 - date1;
        let diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24));

        if (diffDays < 1) diffDays = 1;

        const total = hotel.precio * diffDays * personas;

        if (totalPriceEl) totalPriceEl.textContent = total + ' €';
        
        if(bookingDetailsEl) {
            bookingDetailsEl.innerHTML = `
                ${diffDays} <span data-i18n="nights_text">noches</span> · 
                <span data-i18n="booking_taxes">Impuestos incluidos</span>
            `;
            if (window.updateLanguage) window.updateLanguage(currentLang);
        }
    }

    if(checkinInput && checkoutInput && peopleInput) {
        setDefaultDates();
        calculateTotal();

        checkinInput.addEventListener('change', calculateTotal);
        checkoutInput.addEventListener('change', calculateTotal);
        peopleInput.addEventListener('input', calculateTotal);
    }

 
    // 6. MAPA

    const mapCoords = hotel.coordenadas || [30.0444, 31.2357]; 

    if (document.getElementById('map')) {
        setTimeout(() => {
            if (window.detalleMap) window.detalleMap.remove();
            
            const map = L.map('map').setView(mapCoords, 15);
            window.detalleMap = map;

            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                attribution: '&copy; OpenStreetMap'
            }).addTo(map);
            
            L.marker(mapCoords).addTo(map)
                .bindPopup(`<b>${hotel.nombre}</b><br>${hotel.ubicacion}`)
                .openPopup();
        }, 100);
    }


    // 7. EXTRAS
  
    if (window.updateLanguage) {
         window.updateLanguage(currentLang); 
    }

    let lastScroll = 0;
    const header = document.querySelector('.header');
    if (header) {
        window.addEventListener('scroll', function() {
            const currentScroll = window.pageYOffset;
            if (currentScroll <= 0) {
                header.classList.remove('scroll-up'); return;
            }
            if (currentScroll > lastScroll && !header.classList.contains('scroll-down')) {
                header.classList.remove('scroll-up'); header.classList.add('scroll-down');
            } else if (currentScroll < lastScroll && header.classList.contains('scroll-down')) {
                header.classList.remove('scroll-down'); header.classList.add('scroll-up');
            }
            lastScroll = currentScroll;
        });
    }
});
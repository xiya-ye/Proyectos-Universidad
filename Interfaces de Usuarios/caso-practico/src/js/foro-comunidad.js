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

    // 1. VERIFICAR QUE TENEMOS DATOS BASE
    if (typeof forohilos === 'undefined') {
        console.error("Error: No se ha cargado info-foro.js");
        return;
    }

    // --- FUNCIÓN HELPER IDIOMA ---
    const currentLang = localStorage.getItem('selectedLang') || 'es';
    function getText(data) {
        if (typeof data === 'object' && data !== null) {
            return data[currentLang] || data['es'];
        }
        return data; // Si es texto plano lo devuelve tal cual
    }

    // --- CARGAR LOCALSTORAGE Y COMBINAR ---
    const hilosGuardados = JSON.parse(localStorage.getItem('mis_hilos_nuevos')) || [];
    let todosLosHilos = [...hilosGuardados, ...forohilos]; 

    const tableBody = document.querySelector('.forum-table');
    const tableHeaderHTML = document.querySelector('.table-header').outerHTML;

    
    // 2. FUNCIÓN PARA PINTAR LA LISTA
  
    function renderHilos(lista) {
        tableBody.innerHTML = tableHeaderHTML; 

        lista.forEach(hilo => {
            const respuestasLocales = JSON.parse(localStorage.getItem(`respuestas_hilo_${hilo.id}`)) || [];
            const totalRespuestas = (hilo.respuestasCount || 0) + respuestasLocales.length;
            
            // USAMOS getText() 
            const tituloMostrar = getText(hilo.titulo);
            const fechaMostrar = getText(hilo.fechaCreacion);

            const html = `
                <div class="thread-item" onclick="window.location.href='foro-hilo.html?id=${hilo.id}'">
                    <div class="thread-info">
                        <h3>${tituloMostrar}</h3>
                        <p class="thread-meta">Por ${hilo.autor.nombre} · ${fechaMostrar} · ${hilo.vistas} visitas</p>
                    </div>
                    <div class="thread-category">${hilo.categoria}</div>
                    <div class="thread-replies">${totalRespuestas}</div>
                    <div class="thread-activity">Reciente</div>
                </div>
            `;
            tableBody.insertAdjacentHTML('beforeend', html);
        });
    }


    // 3. FILTROS Y BÚSQUEDA

    const tagBtns = document.querySelectorAll('.tag-btn');
    tagBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            tagBtns.forEach(b => b.classList.remove('active'));
            this.classList.add('active');
            
            // Obtenemos la categoría original 
            // Para este prototipo, filtramos si el texto coincide parcialmente o usamos lógica simple
            const categoria = this.textContent; 
            
            
            if (this.getAttribute('data-i18n') === 'filter_all') {
                renderHilos(todosLosHilos);
            } else {
                // Filtro simple: Si la categoría del hilo coincide
                renderHilos(todosLosHilos); 
            }
        });
    });

    const searchInput = document.querySelector('.search-input');
    searchInput.addEventListener('input', function(e) {
        const texto = e.target.value.toLowerCase();
        const filtrados = todosLosHilos.filter(h => 
            getText(h.titulo).toLowerCase().includes(texto) || 
            getText(h.contenido).toLowerCase().includes(texto)
        );
        renderHilos(filtrados);
    });

    // Filtro "Solo mis hilos"
    const myThreadsCheck = document.querySelector('.checkbox-filter input');
    if(myThreadsCheck) {
        myThreadsCheck.addEventListener('change', function() {
            if(this.checked) renderHilos(hilosGuardados);
            else renderHilos(todosLosHilos);
        });
    }

    // 4. INICIALIZACIÓN
    renderHilos(todosLosHilos);

    // 5. FUNCIONALIDAD CREAR TEMA
    const btnNewThread = document.querySelector('.btn-new-thread');
    const modal = document.getElementById('newThreadModal');
    const closeModal = document.querySelector('.close-modal');
    const formNewThread = document.getElementById('newThreadForm');

    if (btnNewThread) btnNewThread.addEventListener('click', () => modal.classList.add('active'));
    if (closeModal) closeModal.addEventListener('click', () => modal.classList.remove('active'));
    window.addEventListener('click', (e) => { if (e.target == modal) modal.classList.remove('active'); });

if (formNewThread) {
        formNewThread.addEventListener('submit', function(e) {
            e.preventDefault();

            // --- VERIFICAR LOGIN ---
            const currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));

            if (!currentUser) {
                alert("Debes iniciar sesión para crear un nuevo tema.");
                window.location.href = 'login.html'; // Les mandamos a loguearse
                return;
            }
            // ------------------------------

            const titulo = document.getElementById('newTitle').value;
            const categoria = document.getElementById('newCategory').value;
            const contenido = document.getElementById('newContent').value;

            const nuevoHilo = {
                id: Date.now(),
                titulo: titulo, 
                categoria: categoria,
                // --- USAR DATOS DEL USUARIO ---
                autor: { 
                    nombre: currentUser.name, // Nombre real del usuario
                    avatar: currentUser.name.charAt(0).toUpperCase(), // Primera letra de su nombre
                    fecha: "Nuevo", 
                    mensajes: 1 
                },
                // -------------------------------------
                fechaCreacion: "Hace un momento", // O podrías usar new Date().toLocaleDateString()
                vistas: 0,
                respuestasCount: 0,
                contenido: `<p>${contenido.replace(/\n/g, '<br>')}</p>`,
                tags: [categoria],
                respuestas: []
            };

            todosLosHilos.unshift(nuevoHilo);
            
            // Guardamos en localStorage
            const actuales = JSON.parse(localStorage.getItem('mis_hilos_nuevos')) || [];
            actuales.unshift(nuevoHilo);
            localStorage.setItem('mis_hilos_nuevos', JSON.stringify(actuales));

            renderHilos(todosLosHilos);
            modal.classList.remove('active');
            formNewThread.reset();
            alert("¡Tema publicado con tu cuenta!");
        });
    }

    // 6. ESCUCHAR CAMBIO DE IDIOMA
    const langBtns = document.querySelectorAll('.lang-option');
    langBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            setTimeout(() => { location.reload(); }, 100);
        });
    });
});
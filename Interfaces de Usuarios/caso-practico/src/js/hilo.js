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

    // --- FUNCIÓN HELPER IDIOMA ---
    const currentLang = localStorage.getItem('selectedLang') || 'es';
    function getText(data) {
        if (typeof data === 'object' && data !== null) {
            return data[currentLang] || data['es'];
        }
        return data;
    }
    // ----------------------------
    
    // 1. CARGA DE DATOS
    if (typeof forohilos === 'undefined') {
        console.error("Error: info-foro.js no cargado");
        return;
    }

    const hilosGuardados = JSON.parse(localStorage.getItem('mis_hilos_nuevos')) || [];
    const todosLosHilos = [...hilosGuardados, ...forohilos];

    const params = new URLSearchParams(window.location.search);
    const hiloId = parseInt(params.get('id'));
    const hilo = hiloId ? todosLosHilos.find(h => h.id === hiloId) : todosLosHilos[0];

    if (!hilo) {
        document.querySelector('.thread-content').innerHTML = "<h1>Tema no encontrado</h1> <a href='foro-comunidad.html'>Volver al foro</a>";
        return;
    }

    const respuestasLocales = JSON.parse(localStorage.getItem(`respuestas_hilo_${hiloId}`)) || [];
    let totalRespuestas = (hilo.respuestasCount || 0) + respuestasLocales.length;


    // 2. RENDERIZADO (Usando getText)

    // --- HEADER Y META ---
    const tituloTraducido = getText(hilo.titulo);
    document.title = `${tituloTraducido} - Mochileros Sin Fronteras`;
    
    const breadcrumbSpan = document.querySelector('.breadcrumb span');
    if(breadcrumbSpan) breadcrumbSpan.textContent = tituloTraducido;

    document.querySelector('.thread-title-section h1').textContent = tituloTraducido;
    
    const fechaTraducida = getText(hilo.fechaCreacion);
    document.querySelector('.thread-meta').textContent = `Creado por ${hilo.autor.nombre} · ${fechaTraducida} · ${totalRespuestas} respuestas · ${hilo.vistas} visitas`;


    // --- MENSAJE ORIGINAL ---
    const originalPost = document.querySelector('.original-post');
    if (originalPost) {
        originalPost.querySelector('.user-avatar').textContent = hilo.autor.avatar;
        originalPost.querySelector('.user-info strong').textContent = hilo.autor.nombre;
        originalPost.querySelector('.user-date').textContent = hilo.autor.fecha;
        
        originalPost.querySelector('.post-time').textContent = `Publicado ${fechaTraducida}`;
        originalPost.querySelector('.post-body').innerHTML = getText(hilo.contenido);
    }

    // --- RESPUESTAS ---
    const repliesContainer = document.querySelector('.replies-header'); 
    if(repliesContainer) {
        repliesContainer.querySelector('.replies-count').textContent = `${totalRespuestas} respuestas`;
    }

    const examplePosts = document.querySelectorAll('.post:not(.original-post)');
    examplePosts.forEach(post => post.remove());

    // Respuestas Fijas (Traducibles)
    if (hilo.respuestas && hilo.respuestas.length > 0) {
        hilo.respuestas.forEach(resp => {
            renderRespuestaHTML(resp); // La función usará getText dentro
        });
    }

    // Respuestas Nuevas (Locales - Texto plano)
    if (respuestasLocales.length > 0) {
        respuestasLocales.forEach(resp => {
            renderRespuestaHTML(resp);
        });
    }

    function renderRespuestaHTML(resp) {
        const contenidoTraducido = getText(resp.contenido);
        const fechaRespTraducida = getText(resp.fecha);
        
        const htmlRespuesta = `
            <div class="post">
                <div class="post-sidebar">
                    <div class="user-avatar">${resp.autor.avatar}</div>
                    <div class="user-info">
                        <strong>${resp.autor.nombre}</strong>
                        <span class="user-date">${resp.autor.rango}</span>
                    </div>
                </div>
                <div class="post-content">
                    <div class="post-header">
                        <span class="post-time">${fechaRespTraducida}</span>
                        <span class="useful-badge" style="display: ${resp.votos > 0 ? 'inline-block' : 'none'}">${resp.votos} votos útiles</span>
                    </div>
                    <div class="post-body">
                        <p>${contenidoTraducido}</p>
                    </div>
                    <div class="post-actions">
                        <button class="btn-action" data-i18n="btn_reply">Responder</button>
                        <button class="btn-action" data-i18n="btn_quote">Citar</button>
                        <button class="btn-action delete" data-i18n="btn_report">Denunciar</button>
                    </div>
                </div>
            </div>
        `;
        const replyForm = document.querySelector('.reply-form');
        replyForm.insertAdjacentHTML('beforebegin', htmlRespuesta);
    }

    // --- SIDEBAR ---
    const authorCard = document.querySelector('.author-card');
    if(authorCard) {
        authorCard.querySelector('.author-avatar').textContent = hilo.autor.avatar;
        authorCard.querySelector('.author-header strong').textContent = hilo.autor.nombre;
        authorCard.querySelector('.author-header p').textContent = `${hilo.autor.fecha} · ${hilo.autor.mensajes} mensajes`;
    }
    
const threadInfoList = document.querySelector('.thread-info-list');
    if(threadInfoList) {
        // Usamos data-i18n dentro de las etiquetas <strong>
        threadInfoList.innerHTML = `
            <li><strong data-i18n="info_category">Categoría:</strong> ${hilo.categoria}</li>
            <li><strong data-i18n="info_views">Visitas:</strong> ${hilo.vistas}</li>
            <li><strong data-i18n="info_replies">Respuestas:</strong> ${totalRespuestas}</li>
            <li><strong data-i18n="info_last_activity">Última actividad:</strong> Hace un momento</li>
        `;
        if (window.updateLanguage) {
            window.updateLanguage(localStorage.getItem('selectedLang') || 'es');
        }
    }
    // 3. INTERACTIVIDAD
    
    // --- ACTUALIZAR "RESPONDIENDO COMO..." VISUALMENTE ---
    const currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));
    const replyAsContainer = document.querySelector('.reply-as');
    
    if (currentUser && replyAsContainer) {
        // Ponemos la inicial del usuario
        replyAsContainer.querySelector('.avatar-small').textContent = currentUser.name.charAt(0).toUpperCase();
        // Ponemos el nombre completo
        replyAsContainer.querySelector('span strong').textContent = currentUser.name;
    }

    const btnFollow = document.querySelector('.btn-follow');
    if(btnFollow) {
        btnFollow.addEventListener('click', function() {
            const isFollowing = this.style.background === 'rgb(74, 144, 226)' || this.style.background === '#4A90E2';
            if (!isFollowing) {
                this.textContent = 'Siguiendo'; 
                this.style.background = '#4A90E2';
                this.style.color = 'white';
            } else {
                this.textContent = 'Seguir hilo';
                this.style.background = '';
                this.style.color = '';
            }
        });
    }
    
    document.querySelector('.thread-content').addEventListener('click', function(e) {
        if (e.target.classList.contains('btn-action')) {
            const actionKey = e.target.getAttribute('data-i18n');
            const postContent = e.target.closest('.post-content');
            
            if (actionKey === 'btn_reply' || e.target.textContent.includes('Responder')) {
                const replyForm = document.querySelector('.reply-form');
                replyForm.scrollIntoView({ behavior: 'smooth', block: 'center' });
                document.querySelector('.reply-form textarea').focus();
            } 
            else if (actionKey === 'btn_quote' || e.target.textContent.includes('Citar')) {
                const text = postContent.querySelector('.post-body p').textContent;
                const textarea = document.querySelector('.reply-form textarea');
                textarea.value = `> "${text}"\n\n`;
                document.querySelector('.reply-form').scrollIntoView({ behavior: 'smooth', block: 'center' });
                textarea.focus();
            } 
            else if (actionKey === 'btn_report' || e.target.textContent.includes('Denunciar')) {
                if (confirm('¿Seguro?')) alert('Acción realizada.');
            }
        }
    });
    
    const replyTextarea = document.querySelector('.reply-form textarea');
    const btnSubmit = document.querySelector('.btn-submit');
    const btnCancel = document.querySelector('.btn-cancel');
    
if(btnSubmit) {
        btnSubmit.addEventListener('click', function() {
            // --- VERIFICAR LOGIN ---
            const currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));

            if (!currentUser) {
                alert("Debes iniciar sesión para responder.");
                window.location.href = 'login.html';
                return;
            }
            // ------------------------------

            const content = replyTextarea.value.trim();
            if (content === '') {
                alert('Escribe algo...');
                return;
            }
            
            const nuevaRespuesta = {
                // --- USAR DATOS DEL USUARIO ---
                autor: { 
                    nombre: currentUser.name, 
                    avatar: currentUser.name.charAt(0).toUpperCase(), 
                    rango: "Miembro" // Podrías personalizar esto según el usuario
                },
                // -------------------------------------
                fecha: "Ahora mismo", // O new Date().toLocaleDateString()
                contenido: content.replace(/\n/g, '<br>'),
                votos: 0
            };

            const key = `respuestas_hilo_${hiloId}`;
            const guardadas = JSON.parse(localStorage.getItem(key)) || [];
            guardadas.push(nuevaRespuesta);
            localStorage.setItem(key, JSON.stringify(guardadas));
            
            renderRespuestaHTML(nuevaRespuesta);
            
            // Actualizar contadores visuales
            const contadorHeader = document.querySelector('.replies-count');
            if (contadorHeader) {
                let numeroActual = parseInt(contadorHeader.textContent);
                let nuevoNumero = numeroActual + 1;
                contadorHeader.textContent = `${nuevoNumero} respuestas`;
                
                const meta = document.querySelector('.thread-meta');
                if (meta) meta.textContent = meta.textContent.replace(numeroActual, nuevoNumero); // Un poco frágil, pero sirve por ahora
                
                const sidebarInfo = document.querySelector('.thread-info-list li:nth-child(3)');
                if (sidebarInfo) sidebarInfo.innerHTML = `<strong>Respuestas:</strong> ${nuevoNumero}`;
            }

            replyTextarea.value = '';
            if (window.updateLanguage) window.updateLanguage(localStorage.getItem('selectedLang') || 'es');
        });
    }
    
    if(btnCancel) {
        btnCancel.addEventListener('click', function() { replyTextarea.value = ''; });
    }

    if (window.updateLanguage) window.updateLanguage(localStorage.getItem('selectedLang') || 'es');
});
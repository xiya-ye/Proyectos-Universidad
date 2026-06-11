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

    // 1. Verificar si hay usuario logueado
    const currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));

    if (!currentUser) {
        // Si no hay usuario, redirigir al login
        alert("Debes iniciar sesión para ver esta página.");
        window.location.href = 'login.html';
        return; 
    }

    // 2. Rellenar datos del usuario en el HTML



    // A. Inicial del avatar (círculo)
    const avatars = document.querySelectorAll('.avatar-circle');
    const inicial = currentUser.name.charAt(0).toUpperCase();
    avatars.forEach(avatar => avatar.textContent = inicial);

    // B. Nombre del usuario

    const nameElements = document.querySelectorAll('.user-mini-info strong, .profile-text h4');
    nameElements.forEach(el => el.textContent = currentUser.name);

    // C. Email del usuario
    const emailElement = document.querySelector('.profile-text span');
    if (emailElement) emailElement.textContent = currentUser.email;

    // D. Fecha de registro y datos extra (simulados por ahora o guardados)
    const joinedDate = currentUser.joinedDate || "Miembro reciente";
    const dateElement = document.querySelector('.user-mini-info span');
    if (dateElement) dateElement.textContent = `Miembro desde ${joinedDate}`;


    // 3. Funcionalidad CERRAR SESIÓN

    const logoutBtn = document.getElementById('btn-logout');
    
    if (logoutBtn) {
        logoutBtn.addEventListener('click', (e) => {
            e.preventDefault();
            
            // Borramos la sesión activa
            localStorage.removeItem('msf_currentUser');
            
            alert("Has cerrado sesión correctamente.");
            window.location.href = 'home.html';
        });
    }

    // 4. Actualizar preferencias de accesibilidad en el dashboard


    const prefsList = document.querySelector('.prefs-list');

    // Función auxiliar para traducir dentro del JS
    function translateKey(key) {
        // Obtenemos el idioma actual
        const lang = localStorage.getItem('selectedLang') || 'es';
        // Buscamos en el objeto global 'translations' si existe
        if (typeof translations !== 'undefined' && translations[lang] && translations[lang][key]) {
            return translations[lang][key];
        }
        return key; // Si falla, devuelve la clave original
    }

    if (prefsList) {
        // A. Recuperar Idioma
        const currentLang = localStorage.getItem('selectedLang') || 'es';
        const labelLang = translateKey('pref_label_lang');
        
        let htmlContent = `<li><strong>${labelLang}:</strong> ${currentLang.toUpperCase()}</li>`;

        // B. Recuperar Ajustes de Accesibilidad
        if (currentUser && currentUser.preferences) {
            const p = currentUser.preferences;
            
            // Construimos las claves de traducción dinámicamente
           
            const sizeKey = `val_size_${p.textSize}`; 
            const fontKey = `val_font_${p.fontType}`;
            const contrastKey = p.contrast ? 'val_contrast_on' : 'val_contrast_off';
            const motionKey = p.reduceMotion ? 'val_motion_on' : 'val_motion_off';

            // Traducimos las etiquetas y los valores
            htmlContent += `
                <li><strong>${translateKey('pref_label_text_size')}:</strong> ${translateKey(sizeKey)}</li>
                <li><strong>${translateKey('pref_label_contrast')}:</strong> ${translateKey(contrastKey)}</li>
                <li><strong>${translateKey('pref_label_font')}:</strong> ${translateKey(fontKey)}</li>
                <li><strong>${translateKey('pref_label_motion')}:</strong> ${translateKey(motionKey)}</li>
            `;
        } else {
            htmlContent += `<li>${translateKey('val_a11y_standard')}</li>`;
        }

        prefsList.innerHTML = htmlContent;
    }
    
    // (Opcional) Forzar actualización de idioma en toda la página al cargar
    if (window.updateLanguage) {
        window.updateLanguage(localStorage.getItem('selectedLang') || 'es');
    }
});
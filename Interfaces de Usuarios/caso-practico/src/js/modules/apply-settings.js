// js/apply-settings.js
(function() {
    // 1. Intentar leer usuario logueado
    const currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));
    let prefs = {};

    if (currentUser && currentUser.preferences) {
        // Usar preferencias de Carmen
        prefs = currentUser.preferences;
    } else {
        // Usar preferencias de invitado
        prefs = JSON.parse(localStorage.getItem('msf_guest_prefs')) || {};
    }

    // 2. Aplicar clases al <body> o <html>
    // Usamos document.documentElement (la etiqueta <html>) para que aplique lo antes posible
    const root = document.documentElement;

    // A. TAMAÑO DE TEXTO
    // Limpiamos clases previas por si acaso
    root.classList.remove('size-small', 'size-normal', 'size-large', 'size-xl');
    if (prefs.textSize && prefs.textSize !== 'normal') {
        root.classList.add(`size-${prefs.textSize}`);
    }

    // B. CONTRASTE
    if (prefs.contrast) {
        root.classList.add('mode-high-contrast');
    } else {
        root.classList.remove('mode-high-contrast');
    }

    // C. TIPOGRAFÍA (Dislexia)
    if (prefs.fontType === 'dyslexic') {
        root.classList.add('font-dyslexic');
    } else {
        root.classList.remove('font-dyslexic');
    }

    // D. ANIMACIONES
    if (prefs.reduceMotion) {
        root.classList.add('reduce-motion');
    }

})();
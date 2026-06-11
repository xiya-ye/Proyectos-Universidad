document.addEventListener('DOMContentLoaded', () => {

    // 1. Elementos del selector de idioma
    const langOptions = document.querySelectorAll('.lang-option');
    
    // 2. Comprobar si ya había un idioma guardado, si no, usar español
    let currentLang = localStorage.getItem('selectedLang') || 'es';
    
    // 3. Función para cambiar el idioma visualmente
    function updateLanguage(lang) {
        // Guardamos la preferencia para la próxima vez
        localStorage.setItem('selectedLang', lang);
        
        // --- A. CAMBIAR TEXTOS NORMALES (h1, p, span, button, option...) ---
        const elementsToTranslate = document.querySelectorAll('[data-i18n]');
        
        elementsToTranslate.forEach(element => {
            const key = element.getAttribute('data-i18n'); 
            
            if (translations[lang] && translations[lang][key]) {
                element.textContent = translations[lang][key];
            }
        });

        // --- B. CAMBIAR PLACEHOLDERS (Inputs, selects con placeholder) ---
        const placeholdersToTranslate = document.querySelectorAll('[data-i18n-placeholder]');
        
        placeholdersToTranslate.forEach(element => {
            const key = element.getAttribute('data-i18n-placeholder');
            
            if (translations[lang] && translations[lang][key]) {
                element.placeholder = translations[lang][key];
            }
        });

        // --- C. ACTUALIZAR EL SELECTOR (Negrita en el activo) ---
        langOptions.forEach(option => {
            if (option.getAttribute('data-lang') === lang) {
                option.classList.add('active'); 
                option.style.fontWeight = 'bold'; 
                option.style.color = '#333';
            } else {
                option.classList.remove('active');
                option.style.fontWeight = 'normal';
                option.style.color = '#999';
            }
        });

        // --- D. Recalcular textos dinámicos dependientes del idioma (si existen) ---
        // Ej: resumen de personas en buy-p1
        if (typeof window.actualizarResumen === 'function') {
            window.actualizarResumen();
        }
    }

    // Hacemos accesible la función por si alguna página la quiere invocar a mano
    window.updateLanguage = updateLanguage;

    // 4. Aplicar el idioma al cargar la página
    updateLanguage(currentLang);

    // 5. Escuchar los clics en ES | EN | IT
    langOptions.forEach(option => {
        option.addEventListener('click', () => {
            const selectedLang = option.getAttribute('data-lang'); 
            updateLanguage(selectedLang);
        });
    });

});

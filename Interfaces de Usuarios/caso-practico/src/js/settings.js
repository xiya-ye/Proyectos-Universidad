document.addEventListener('DOMContentLoaded', () => {

    // --- FUNCIÓN HELPER PARA TRADUCIR DENTRO DE JS ---
    function getText(key) {
        const lang = localStorage.getItem('selectedLang') || 'es';
        // Verificamos si existe el diccionario global 'translations'
        if (typeof translations !== 'undefined' && translations[lang] && translations[lang][key]) {
            return translations[lang][key];
        }
        return key; // Si no encuentra traducción, devuelve la clave
    }
    // -------------------------------------------------


    // 1. GESTIÓN DEL USUARIO Y HEADER

    const accountLink = document.querySelector('.user-link');
    let currentUser = JSON.parse(localStorage.getItem('msf_currentUser'));

    if (accountLink) {
        if (currentUser) {
            accountLink.href = 'account-dashboard.html';
            accountLink.textContent = 'Mi Panel'; 
        } else {
            accountLink.href = 'login.html';
        }
    }

  
    // 2. REFERENCIAS A ELEMENTOS DEL DOM

    const radioTextSize = document.querySelectorAll('input[name="textSize"]');
    const toggleContrast = document.getElementById('toggle-contrast');
    const radioFontType = document.querySelectorAll('input[name="fontType"]');
    const toggleAnimations = document.getElementById('toggle-animations');
    
    const btnSave = document.querySelector('.btn-save');
    const btnReset = document.querySelector('.btn-reset');
    
    const successPill = document.querySelector('.success-pill');
    const previewCard = document.querySelector('.preview-content-card');


    // 3. FUNCIONES DE VISTA PREVIA
    
    function updateTextSize(size) {
        previewCard.classList.remove('size-small', 'size-normal', 'size-large', 'size-xl');
        if (size) previewCard.classList.add(`size-${size}`);
    }

    function updateContrast(isHighContrast) {
        if (isHighContrast) previewCard.classList.add('mode-high-contrast');
        else previewCard.classList.remove('mode-high-contrast');
    }

    function updateFont(fontType) {
        if (fontType === 'dyslexic') previewCard.classList.add('font-dyslexic');
        else previewCard.classList.remove('font-dyslexic');
    }

    // 4. CARGAR PREFERENCIAS AL INICIAR

    function loadPreferences() {
        let prefs = {};

        // A. Determinar origen de datos
        if (currentUser && currentUser.preferences) {
            prefs = currentUser.preferences;
            // TRADUCCIÓN  
            if (successPill) successPill.textContent = getText('msg_prefs_synced');
        } else {
            prefs = JSON.parse(localStorage.getItem('msf_guest_prefs')) || {};
            // TRADUCCIÓN  
            if (successPill) successPill.textContent = getText('msg_prefs_device');
        }

        // B. Aplicar valores
        const savedSize = prefs.textSize || 'normal';
        const radioSize = document.querySelector(`input[name="textSize"][value="${savedSize}"]`);
        if (radioSize) radioSize.checked = true;
        updateTextSize(savedSize);

        const savedContrast = prefs.contrast || false;
        if (toggleContrast) {
            toggleContrast.checked = savedContrast;
            updateContrast(savedContrast);
        }

        const savedFont = prefs.fontType || 'standard';
        const radioFont = document.querySelector(`input[name="fontType"][value="${savedFont}"]`);
        if (radioFont) radioFont.checked = true;
        updateFont(savedFont);

        if (toggleAnimations) {
            toggleAnimations.checked = prefs.reduceMotion || false;
        }
    }

    loadPreferences();

 
    // 5. LISTENERS

    radioTextSize.forEach(r => r.addEventListener('change', (e) => updateTextSize(e.target.value)));
    if (toggleContrast) toggleContrast.addEventListener('change', (e) => updateContrast(e.target.checked));
    radioFontType.forEach(r => r.addEventListener('change', (e) => updateFont(e.target.value)));


    // 6. GUARDAR (PERSISTENCIA)

    if (btnSave) {
        btnSave.addEventListener('click', () => {
            const selectedSize = document.querySelector('input[name="textSize"]:checked')?.value || 'normal';
            const isContrast = toggleContrast ? toggleContrast.checked : false;
            const selectedFont = document.querySelector('input[name="fontType"]:checked')?.value || 'standard';
            const isReduceMotion = toggleAnimations ? toggleAnimations.checked : false;

            const newPreferences = {
                textSize: selectedSize,
                contrast: isContrast,
                fontType: selectedFont,
                reduceMotion: isReduceMotion
            };

            if (currentUser) {
                currentUser.preferences = newPreferences;
                localStorage.setItem('msf_currentUser', JSON.stringify(currentUser));

                const usersDB = JSON.parse(localStorage.getItem('msf_users')) || [];
                const userIndex = usersDB.findIndex(u => u.email === currentUser.email);
                if (userIndex !== -1) {
                    usersDB[userIndex].preferences = newPreferences;
                    localStorage.setItem('msf_users', JSON.stringify(usersDB));
                }
                
                // TRADUCCIÓN 
                successPill.textContent = getText('msg_saved_account');
            } else {
                localStorage.setItem('msf_guest_prefs', JSON.stringify(newPreferences));
                // TRADUCCIÓN 
                successPill.textContent = getText('msg_saved_device');
            }

            const originalText = btnSave.textContent;
            // TRADUCCIÓN 
            btnSave.textContent = getText('msg_saved_short'); 
            btnSave.style.backgroundColor = "#10b981"; 

            setTimeout(() => {
                btnSave.textContent = originalText; 
                btnSave.style.backgroundColor = ""; 
            }, 2000);
        });
    }

    // 7. Botón Restablecer
    if (btnReset) {
        btnReset.addEventListener('click', () => {
            // TRADUCCIÓN 
            if(confirm(getText('msg_confirm_reset'))) {
                const defaultSize = document.querySelector('input[name="textSize"][value="normal"]');
                const defaultFont = document.querySelector('input[name="fontType"][value="standard"]');
                
                if(defaultSize) defaultSize.checked = true;
                if(defaultFont) defaultFont.checked = true;
                if(toggleContrast) toggleContrast.checked = false;
                if(toggleAnimations) toggleAnimations.checked = false;

                updateTextSize('normal');
                updateContrast(false);
                updateFont('standard');
                
                // TRADUCCIÓN (Alert)
                alert(getText('msg_reset_alert'));
            }
        });
    }

});
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

    const loginForm = document.querySelector('.auth-form');

    if (loginForm) {
        loginForm.addEventListener('submit', function(e) {
            e.preventDefault(); // Evitamos que el formulario se envíe por defecto
            handleLogin();
        });
    }

    function handleLogin() {
        // 1. Capturar los datos del formulario
        const emailInput = document.getElementById('email');
        const passwordInput = document.getElementById('password');
        
        const email = emailInput.value.trim().toLowerCase();
        const password = passwordInput.value;

        // 2. Obtener nuestra "Base de Datos" de usuarios
        const usersDB = JSON.parse(localStorage.getItem('msf_users')) || [];

        // 3. Buscar al usuario
        // Buscamos alguien que tenga ese email Y esa contraseña
        const userFound = usersDB.find(user => user.email === email && user.password === password);

        if (userFound) {
            // 4. LOGIN EXITOSO
            
            // Creamos la sesión (quitando la contraseña por seguridad simulada)
            const sessionUser = { ...userFound };
            delete sessionUser.password; 
            
            localStorage.setItem('msf_currentUser', JSON.stringify(sessionUser));

            // Si marcó "Recordar sesión" (opcional: podrías usar otra lógica aquí, 
            // pero por ahora lo tratamos igual)
            // const rememberMe = document.querySelector('input[name="remember"]').checked;

            alert(`¡Hola de nuevo, ${sessionUser.name}!`);
            
            // 5. Redirigir al panel de usuario
            window.location.href = 'account-dashboard.html';

        } else {
            // 6. ERROR DE LOGIN
            alert("El correo electrónico o la contraseña son incorrectos.");
            
            // Limpiar la contraseña para que vuelva a intentar
            passwordInput.value = '';
            passwordInput.focus();
            
            // Feedback visual simple 
            emailInput.style.borderColor = '#ff6b6b';
            passwordInput.style.borderColor = '#ff6b6b';
            
            // Quitamos el rojo cuando empiece a escribir de nuevo
            setTimeout(() => {
                emailInput.style.borderColor = '';
                passwordInput.style.borderColor = '';
            }, 3000);
        }
    }
});
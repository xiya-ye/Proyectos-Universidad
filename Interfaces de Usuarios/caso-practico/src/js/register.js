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

    const registerForm = document.querySelector('.auth-form');

    if (registerForm) {
        registerForm.addEventListener('submit', function(e) {
            e.preventDefault(); // Paramos el envío normal para hacerlo nosotros
            handleRegister(this);
        });
    }

    function handleRegister(form) {
        // 1. Capturar valores
        const name = document.getElementById('name').value.trim();
        const email = document.getElementById('email').value.trim().toLowerCase();
        const password = document.getElementById('password').value;
        const passwordConfirm = document.getElementById('password_confirm').value;
        const tripStyle = document.getElementById('trip_style').value;
        const newsletter = form.querySelector('input[name="newsletter"]').checked;

        // 2. Validaciones básicas
        
        // A. Contraseñas coinciden
        if (password !== passwordConfirm) {
            alert("Las contraseñas no coinciden.");
            return;
        }

        // B. Longitud contraseña
        if (password.length < 6) {
            alert("La contraseña debe tener al menos 6 caracteres.");
            return;
        }

        // C. Comprobar si el usuario ya existe (Simulando consulta a BBDD)
        const usersDB = JSON.parse(localStorage.getItem('msf_users')) || [];
        
        const userExists = usersDB.find(user => user.email === email);
        
        if (userExists) {
            alert("Este correo electrónico ya está registrado. Por favor, inicia sesión.");
            return;
        }

        // 3. Crear el objeto Usuario
        // IMPORTANTE PARA EL FORO: Generamos un ID único (Date.now())
        const newUser = {
            id: Date.now(), // ID único basado en el tiempo (ej: 1715629384)
            name: name,
            email: email,
            password: password, 
            tripStyle: tripStyle,
            newsletter: newsletter,
            joinedDate: new Date().toLocaleDateString(),
            avatar: '/assets/images/default-avatar.png' // Preparamos avatar por defecto
        };

        // 4. Guardar en "Base de Datos" (LocalStorage)
        usersDB.push(newUser);
        localStorage.setItem('msf_users', JSON.stringify(usersDB));

        // 5. Iniciar sesión automáticamente (Guardar sesión activa)
        // Guardamos todo menos la contraseña en la sesión activa por seguridad "visual"
        const sessionUser = { ...newUser };
        delete sessionUser.password;
        
        localStorage.setItem('msf_currentUser', JSON.stringify(sessionUser));

        // 6. Feedback y Redirección
        alert("¡Cuenta creada con éxito! Bienvenido a la aventura.");
        window.location.href = 'account-dashboard.html'; // Redirigir al panel
    }
});
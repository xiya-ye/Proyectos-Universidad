document.addEventListener('DOMContentLoaded', () => {
    // Seleccionamos los elementos del DOM
    const menuToggle = document.querySelector('.menu-toggle');
    const mainNav = document.querySelector('.main-nav');

    // Escuchamos el clic en el botón hamburguesa
    if (menuToggle) {
        menuToggle.addEventListener('click', () => {
            // Alternamos la clase 'active' para mostrar/ocultar el menú
            mainNav.classList.toggle('active');
            
            // Accesibilidad: Cambiamos aria-expanded
            const isExpanded = mainNav.classList.contains('active');
            menuToggle.setAttribute('aria-expanded', isExpanded);
        });
    }
});
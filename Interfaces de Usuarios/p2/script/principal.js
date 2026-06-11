const packs = document.querySelectorAll('.imagen2, .imagen3, .imagen4'); 
let indice = 0;
const total = packs.length;

// Oculta todos los packs menos el primero
packs.forEach((pack, i) => {
  if (i === 0) {
    pack.style.display = 'block';
  } else {
    pack.style.display = 'none';
  }
});

// Función para mostrar un pack concreto
function mostrarPack() {
  packs.forEach((pack, i) => {
    if (i === indice) {
      pack.style.display = 'block';
    } else {
      pack.style.display = 'none';
    }
  });
}

// Función para avanzar
function siguientePack() {
  indice = (indice + 1) % total;
  mostrarPack();
}

// Función para retroceder
function anteriorPack() {
  indice = (indice - 1 + total) % total;
  mostrarPack();
}


document.querySelectorAll('.siguiente').forEach(button => {
  button.addEventListener('click', (e) => {
    e.preventDefault();
    siguientePack();
    reiniciarAutoSlide();
  });
});

document.querySelectorAll('.anterior').forEach(button => {
  button.addEventListener('click', (e) => {
    e.preventDefault();
    anteriorPack();
    reiniciarAutoSlide();
  });
});

// Cambio automático cada 2 segundos
let autoSlide = setInterval(siguientePack, 2000);

function reiniciarAutoSlide() {
  clearInterval(autoSlide);
  autoSlide = setInterval(siguientePack, 2000);
}

//Para cuando hacen click en comprar
document.addEventListener('click', (e) => {
  if (e.target.matches('.button.comprar')){
    e.preventDefault();
    console.log("Boton comprar clickeado");

    const pack = e.target.closest('.imagen2, .imagen3, .imagen4');

    if (!pack) {
      console.error("No se encontro el pack del boton");
      return;
    }

    const tituloEl = pack.querySelector('h3');
    const descripcionEl = pack.querySelectorAll('p')[0];
    const precioEl = pack.querySelectorAll('p')[1];

    if (!tituloEl || !descripcionEl || !precioEl) {
      console.error("No se encontro alguno de los elementos dentro del pack");
      return;
    }

    const titulo = tituloEl.innerText;
    const descripcion = descripcionEl.innerText;
    const precio = precioEl.innerText;

    let imagenUrl = '';
    if (pack.classList.contains('imagen2')) {
      imagenUrl = 'images/pack.jpg';
    } else if (pack.classList.contains('imagen3')) {
      imagenUrl = 'images/pack1.jpg';
    } else if (pack.classList.contains('imagen4')) {
      imagenUrl = 'images/pack2.jpg';
    }

    console.log("Pack guardado: ", {titulo, descripcion, precio, imagenUrl});

    //guardamos la info en el localStorage
    const datosPack = {titulo: titulo, descripcion: descripcion, 
      precio: precio, imagenUrl: imagenUrl};
    
      localStorage.setItem('packSeleccionado', JSON.stringify(datosPack));
    
    const verificar = localStorage.getItem('packSeleccionado');
    console.log("Verificacion localStorage:", verificar);
    console.log("Tipo de dato:", typeof verificar);

    window.location.href = 'version_c.html';

  };
});



// ----------- INICIAR SESION

import { iniciarSesion } from './modules/usuario.mjs';

document.addEventListener('DOMContentLoaded', () => {
  const formLogin = document.getElementById('form-login');

  formLogin.addEventListener('submit', (e) => {
    e.preventDefault();

    const login = document.getElementById('login').value.trim();
    const password = document.getElementById('password').value.trim();

    if (!login || !password) {
      alert("Por favor, completa ambos campos.");
      return;
    }

    const usuario = iniciarSesion(login, password);

    if (usuario) {
      localStorage.setItem('usuarioEnUso', JSON.stringify(usuario));
      window.location.href = 'version_b.html';
    } else {
      alert("Usuario o contraseña incorrectos.");
    }
  });
});

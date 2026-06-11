import {Usuario, guardarUsuario, cargarUsuarios, iniciarSesion} from './modules/usuario.mjs';
import {Consejo, guardarConsejo, obtenerUltimosConsejos, validarConsejo} from './modules/consejos.mjs';

document.addEventListener("DOMContentLoaded", () => {
  // ---------- CONTROL DEL USUARIO ----------
  
  // Primero nos aseguramos de que el usuario está registrado
  const usuarioEnUso = JSON.parse(localStorage.getItem('usuarioEnUso'));
  if (!usuarioEnUso) {
    alert("Debes registrate primero");
    window.location.href = "principal.html";
    return;
  }

  // le asignamos el nombre del usuario y su imagen correspondiente
  document.querySelector("#nombre-usuario").textContent = usuarioEnUso.nombre;
  document.querySelector("#imagen-usuario").src = usuarioEnUso.imagen;

  const btnCerrarSesion = document.querySelector("#btn-cerrar-sesion");
  btnCerrarSesion.addEventListener("click", () => {
    // Abrimos ventana emergente para la confirmación de cierre de sesión
    if (confirm("¿Desea cerrar sesión?")) {
      localStorage.removeItem('usuarioEnUso');
      window.location.href = "principal.html";
    }
  });


  // ---------- MANEJO DE CONSEJOS ----------
const formConsejo = document.getElementById('form-consejo');

if (formConsejo) {
  formConsejo.addEventListener('submit', (e) => {
    e.preventDefault();

    const titulo = document.getElementById("titulo-consejo").value.trim();
    const descripcion = document.getElementById("descripcion-consejo").value.trim();

    //Validar longitud minima
    const errores = validarConsejo(titulo, descripcion);

    if (errores.length > 0) {
      alert("Errores en el formulario:\n\n-" + errores.join("\n- "));
      return;
    }

    //Si pasa las validaciones, guardar el consejo
    const nuevoConsejo = new Consejo(titulo, descripcion, usuarioEnUso.login);
    guardarConsejo(nuevoConsejo);
    alert("Consejo enviado correctamente.");

    // Limpiar el formulario
    formConsejo.reset();

    //Actualizar la lista de ultimos consejos
    mostrarUltimosConsejos();
  });
} 

//Mostrar ultimos consejos
function mostrarUltimosConsejos() {
  const ultimosConsejos = obtenerUltimosConsejos(3);
  const listaConsejos = document.querySelector(".lista-consejos");

  if (!listaConsejos) return;

  listaConsejos.innerHTML = "";

  if (ultimosConsejos.length === 0) {
    const listItem = document.createElement("li");
    listItem.textContent = "No hay consejos aun. Sé el primero en compartir uno!";
    listaConsejos.appendChild(listItem);
    return;
  }

  ultimosConsejos.forEach((consejo) => {
    const listItem = document.createElement("li");
    const link = document.createElement("a");
    link.href = "#";
    link.textContent = consejo.titulo;
    link.addEventListener("click", (e) => {
      e.preventDefault();
      mostrarConsejoCompleto(consejo);
    });

    listItem.appendChild(link);
    listaConsejos.appendChild(listItem);
  });
}

//Funcion para mostrar el consejo completo
function mostrarConsejoCompleto(consejo) {
  //Crear ventan emergente o mostrar en una seccion especifica
  const mensaje = `
Título: ${consejo.titulo}
Descripción: ${consejo.descripcion}
Usuario: ${consejo.usuario}
Fecha: ${new Date(consejo.fecha).toLocaleDateString()}`;

  alert(mensaje);
}
//Inicializar consejos
mostrarUltimosConsejos();

  // ---------- CONTROL DEL CARRUSEL ----------
  const packs = document.querySelectorAll('.imagen2, .imagen3, .imagen4'); 
  let indice = 0;
  const total = packs.length;

  if (packs.length === 0) {
        console.error("No se encontraron packs con las clases .imagen2, .imagen3, .imagen4");
        return;
    }

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

  // Cambio automático cada 2 segundos
  let autoSlide = setInterval(siguientePack, 2000);
  console.log("Auto slide iniciado");

  function reiniciarAutoSlide() {
    clearInterval(autoSlide);
    autoSlide = setInterval(siguientePack, 2000);
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

  document.querySelectorAll('.comprar').forEach(button => {
    button.addEventListener('click', (e) => {
      e.preventDefault();
      window.location.href = 'version_c.html';
    });
  });
});

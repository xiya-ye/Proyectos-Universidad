
import {
  validarNombre,
  validarApellidos,
  validarCorreo,
  validarLogin,
  validarPassword,
  validarFecha,
  validarImagen,
} from "./modules/validaciones.mjs";

import { Usuario, guardarUsuario } from "./modules/usuario.mjs";

// Creamos una funcion que convierta una imagen a Base64 con FileReader para poder guardarlo en el local storage
function convertirImagenABase64(imagen) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => resolve(reader.result);
    reader.onerror = error => reject(error);
    reader.readAsDataURL(imagen);
  });
}


// Cargamos el formulario y sus elementos
document.addEventListener("DOMContentLoaded", () => {
  const form = document.querySelector("#form-registro");
  const checkbox = document.querySelector("#politica_de_privacidad");
  const btnGuardar = document.querySelector(".button");

  // Nos aseguramos que el botón esté desactivado inicialmente
  checkbox.addEventListener("change", () => {
    btnGuardar.disabled = !checkbox.checked;
  });

  form.addEventListener("submit", async(e) => {
    e.preventDefault();

    // leemos los valores del formulario
    const nombre = document.querySelector("#nombre").value;
    const apellidos = document.querySelector("#apellidos").value;
    const correo = document.querySelector("#email").value;
    const confirmarCorreo = document.querySelector("#email2").value;
    const fecha = document.querySelector("#fecha").value;
    const login = document.querySelector("#login").value;
    const password = document.querySelector("#password").value;
    const imagen = document.querySelector("#imagen").files[0];

    let errores = [];

    // validamos todos los datos introducidos
    if (!validarNombre(nombre)) {
      errores.push("El nombre debe tener al menos 3 caracteres.");
    }

    if (!validarApellidos(apellidos)) {
      errores.push("Los apellidos deben tener al menos dos palabras de 3 letras cada una.");
    }

    if (!validarCorreo(correo)) {
      errores.push("El correo no tiene un formato válido.");
  }
  
    if (correo !== confirmarCorreo) {
      errores.push("Los correos no coinciden.");
    }

    // Para la fecha tenemos en cuenta que no es necesario rellenarlo
    if (fecha.trim() !== "" && !validarFecha(fecha)) {
      errores.push("La fecha de nacimiento no es válida.");
    }

    if (!validarLogin(login)) {
      errores.push("El login debe tener al menos 5 caracteres.");
    }

    if (!validarPassword(password)) {
      errores.push("La contraseña debe tener 8 caracteres, 2 números, 1 mayúscula, 1 minúscula y 1 símbolo.");
    }

    if (!validarImagen(imagen)) {
      errores.push("La imagen debe ser .webp, .png o .jpg.");
    }

    if (!checkbox.checked) {
      errores.push("Debes aceptar la política de privacidad.");
    }

    if (errores.length > 0) {
      alert("Errores:\n- " + errores.join("\n- "));
      return;
    }

    try {
      // Convertimos el foto de perfil a Base64
      let imagenBase64 = null;
      if (imagen) {
        imagenBase64 = await convertirImagenABase64(imagen);
      }
      // Guardamos el usuario
      const usuario = new Usuario(nombre, apellidos, correo, login, password, fecha || null, imagenBase64);
      guardarUsuario(usuario);

      // Guardamos el usuario en uso actual en el Local Storage
      localStorage.setItem('usuarioEnUso', JSON.stringify(usuario));
      window.location.href = "version_b.html";
    }

    catch (error) {
      console.error("Error al procesar la imagen:", error);
      alert("Error al procesar la imagen. Inténtalo de nuevo.");
    }
  });
});
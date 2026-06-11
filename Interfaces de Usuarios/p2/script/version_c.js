
import {
  validarNombreCompleto,
  validarCorreo,
  validarTipoTarjeta,
  validarNumeroTarjeta,
  validarNombreTitular,
  validarFechaCaducidad,
  validarCVV
} from "./modules/validaciones.mjs";

document.addEventListener("DOMContentLoaded", () => {
  const form = document.getElementById("form-compra");

  form.addEventListener("submit", (e) => {
    e.preventDefault();

    // leemos los valores del formulario
    const nombreCompleto = document.getElementById("fname").value;
    const correo = document.getElementById("lname").value;
    const tipoTarjeta = document.getElementById("tarjeta").value;
    const numeroTarjeta = document.getElementById("numtarjeta").value;
    const titularTarjeta = document.getElementById("titulartarjeta").value;
    const fechaCaducidad = document.getElementById("fecha").value;
    const cvv = document.getElementById("cvv").value;

    const errores = [];

    // validamos los formatos de los datos introducidos
    if (!validarNombreCompleto(nombreCompleto)) {
      errores.push("El nombre completo debe tener al menos 3 caracteres.");
    }

    if (!validarCorreo(correo)) {
      errores.push("El correo electrónico no es válido.");
    }

    if (!validarTipoTarjeta(tipoTarjeta)) {
      errores.push("Debes seleccionar un tipo de tarjeta.");
    }

    if (!validarNumeroTarjeta(numeroTarjeta)) {
      errores.push("El número de tarjeta debe tener 13, 15, 16 o 19 dígitos.");
    }

    if (!validarNombreTitular(titularTarjeta)) {
      errores.push("El nombre del titular debe tener al menos 3 caracteres.");
    }

    if (!validarFechaCaducidad(fechaCaducidad)) {
      errores.push("La fecha de caducidad no puede estar vencida.");
    }

    if (!validarCVV(cvv)) {
      errores.push("El CVV debe tener exactamente 3 dígitos.");
    }

    // Mostrar errores o confirmar compra
    if (errores.length > 0) {
      alert("Errores:\n- " + errores.join("\n- "));
    } 
    else {
      alert("Compra realizada con éxito.");
      // reseteamos el formulario
      form.reset();
    }
  });

  form.addEventListener("reset", () => {
    // Opcional: si quieres limpiar también mensajes o estilos
    console.log("Formulario reiniciado");
  });
});

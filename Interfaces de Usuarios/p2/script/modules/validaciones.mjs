// script/validaciones.mjs

// nombre: mínimo 3 caracteres
function validarNombre(nombre) {
  return nombre.length >= 3;
} export {validarNombre};

// apellidos: mínimo dos cadenas de caracteres de 3 caracteres de longitud cada una
function validarApellidos(apellidos) {
  const partes = apellidos.trim().split(/\s+/);
  return partes.length >= 2 && partes.every(p => p.length >= 3);
} export {validarApellidos};

// correo: formato válido
function validarCorreo(correo) {
  const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  return regex.test(correo);
} export {validarCorreo};

// login:  mínimo 5 caracteres de longitud
function validarLogin(login) {
  return login.length >= 5;
} export {validarLogin};

// password: 8 caracteres de longitud, con mínimo 2 números, 1 carácter especial, 1 letra mayúscula y 1 letra minúscula
function validarPassword(password) {
  const regex = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d.*\d)(?=.*[!@#$%^&*()_+])[A-Za-z\d!@#$%^&*()_+]{8,}$/;
  return regex.test(password);
} export {validarPassword};

// fecha: fecha valida y anterior a la actual
function validarFecha(fecha) {
  if (!fecha) return false;
  const hoy = new Date();
  const nacimiento = new Date(fecha);
  return nacimiento < hoy && nacimiento.getFullYear() > 1900;
} export {validarFecha};

// imagen: formatos permitidos (.webp, .png, .jpg)
function validarImagen(archivo) {
  if (!archivo) return false;
  const permitidos = ["image/webp", "image/png", "image/jpeg"];
  return permitidos.includes(archivo.type);
} export {validarImagen};




// nombre completo: mínimo 3 caracteres
function validarNombreCompleto(nombre) {
  return nombre.trim().length >= 3;
} export {validarNombreCompleto};

// tipo de tarjeta: no puede estar vacío
function validarTipoTarjeta(tipo) {
  return tipo !== "";
} export {validarTipoTarjeta};

// número de tarjeta: 13, 15, 16 o 19 dígitos
function validarNumeroTarjeta(numero) {
  const regex = /^(?:\d{13}|\d{15}|\d{16}|\d{19})$/;
  return regex.test(numero.trim());
} export {validarNumeroTarjeta};

// nombre del titular: mínimo 3 caracteres
function validarNombreTitular(nombre) {
  return nombre.trim().length >= 3;
} export {validarNombreTitular};

// fecha de caducidad: que sea una fecha posterior al actual
function validarFechaCaducidad(fechaCaducidad) {
  if (!fechaCaducidad) return false;
  const hoy = new Date();
  const fecha = new Date(fechaCaducidad);
  return fecha.getFullYear() > hoy.getFullYear() ||
         (fecha.getFullYear() === hoy.getFullYear() && fecha.getMonth() >= hoy.getMonth());
} export {validarFechaCaducidad};

// CVV: 3 dígitos
function validarCVV(cvv) {
  const regex = /^\d{3}$/;
  return regex.test(cvv.trim());
} export {validarCVV};
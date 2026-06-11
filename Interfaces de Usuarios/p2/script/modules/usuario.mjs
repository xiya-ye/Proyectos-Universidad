
class Usuario {
  constructor(nombre, apellidos, correo, login, password, fecha, imagen) {
    this.nombre = nombre;
    this.apellidos = apellidos;
    this.correo = correo;
    this.login = login;
    this.password = password;
    this.fecha = fecha;
    this.imagen = imagen;
  }
} export {Usuario};



function guardarUsuario(usuario) {
  // Obtenemos los usuarios del Local Storage y si esta vacío, creamos un array vacío
  const usuarios = JSON.parse(localStorage.getItem('usuarios') || '[]');

  // Añadimos el nuevo usuario
  usuarios.push(usuario);

  // Guardamos de nuevo en el Local Storage
  localStorage.setItem('usuarios', JSON.stringify(usuarios));
} export {guardarUsuario};

function cargarUsuarios() {
  return JSON.parse(localStorage.getItem('usuarios') || '[]');
} export {cargarUsuarios};

function iniciarSesion(login, password) {
  // Cargamos todos los usuarios guardados en localStorage
  const usuarios = cargarUsuarios();

  // Buscamos en la lista de usuarios
  for (let i = 0; i < usuarios.length; i++) {
    const usuario = usuarios[i];

    // Comprobamos que tanto el nombre de usuario como la contraseña coinciden
    if (usuario.login === login && usuario.password === password) {
      return usuario;
    }
  }
  return null;
} export {iniciarSesion};



/*
function cerrarSesion() {
  localStorage.removeItem('usuario');
} export {cerrarSesion};

function usuarioRegistrado() {
  return localStorage.getItem('usuario') !== null;
} export {usuarioRegistrado};
*/
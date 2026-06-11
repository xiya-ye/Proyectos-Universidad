import streamlit as st
import pandas as pd
import joblib

# Configuramos la página
st.set_page_config(page_title="Banco UC3M", layout="wide")
st.title("Simulador de Contratación de Depósitos")
st.markdown("Introduce los datos del cliente para predecir si contratará el depósito a plazo fijo.")

# Carga el modelo
@st.cache_resource
def cargar_modelo():
    return joblib.load('modelo_final.joblib')
    
modelo = cargar_modelo()

# Formulario de datos
st.header("Datos del Cliente")

col1, col2, col3 = st.columns(3)

with col1:
    age = st.number_input("Edad", min_value=18, max_value=100, value=30)
    job = st.selectbox("Trabajo", ["admin", "technician", "services", 
                                   "management", "retired", "blue-collar", 
                                   "unemployed", "entrepreneur", "housemaid", 
                                   "unknown", "self-employed", "student"])
    marital = st.selectbox("Estado Marital", ["married", "single", "divorced"])
    education = st.selectbox("Educación", ["secondary", "tertiary", "primary", 
                                           "unknown"])
    default = st.selectbox("Créditos no devueltos", ["no", "yes"])

with col2:
    balance = st.number_input("Balance anual medio", value=1000)
    housing = st.selectbox("¿Tiene una hipoteca?", ["yes", "no"])
    loan = st.selectbox("¿Tiene un préstamo?", ["yes", "no"])
    contact = st.selectbox("Tipo de contacto", ["cellular", "unknown", "telephone"])
    day = st.number_input("Último día del mes de contacto", min_value=1, max_value=31, value=15)
    month = st.selectbox("Último mes de contacto", ["jan", "feb", "mar", "apr", "may", 
                                                    "jun", "jul", "aug", "sep", "oct", 
                                                    "nov", "dec"])

with col3:
    duration = st.number_input("Duración (segundos) del último contacto", min_value=0, value=250)
    campaign = st.number_input("Número de contactos en esta campaña", min_value=1, value=1)
    pdays = st.number_input("Días desde el último contacto (-1 si no hubo)", value=-1)
    previous = st.number_input("Número de contactos previos", min_value=0, value=0)
    poutcome = st.selectbox("Resultado de campaña anterior", ["unknown", "other", "failure", "success"])

# Predicción y preprocesamiento
if st.button("Predecir Contratación", use_container_width=True):
    # Recogemos los datos del formulario
    datos_cliente = pd.DataFrame([{
        'age': age, 
        'job': job,
        'marital': marital,
        'education': education,
        'default': default,
        'balance': balance,
        'housing': housing,
        'loan': loan,
        'contact': contact,
        'day': day,
        'month': month,
        'duration': duration,
        'campaign': campaign,
        'pdays': pdays,
        'previous': previous,
        'poutcome': poutcome
    }])

    datos_cliente['contactado_previamente'] = (datos_cliente['pdays'] != -1).astype(int)
    datos_cliente.loc[datos_cliente['pdays'] == -1, 'pdays'] = 0

    prediccion = modelo.predict(datos_cliente)

    # Mostramos el resultado
    st.divider()
    if prediccion[0] == "yes":
        st.success("Predicción positiva: Es muy probable que este cliente contrate el depósito")

    else: 
        st.error("Predicción negativa: Es probable que este cliente rechace el depósito")

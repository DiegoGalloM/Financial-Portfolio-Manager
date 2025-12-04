# Reflexión

## Luis Ernesto Colunga Lozano

### 1. Cómo usaron/no usaron IA (Copilot, ChatGPT)?

    Yo personalmente utilicé la inteligencia artificial como una herramienta que me permitió convertir mis ideas en realidad y hacer mi código más legible. Por ejemplo, una de las partes más difíciles del código de Dato.h era lograr que los datos fueran lo suficientemente flexibles para poder cambiar entre tipos como integer, float, string o incluso un enum Category. Aunque ya tenía conocimiento previo sobre C++, nunca antes había hecho un programa que resolviera un problema así. Por lo tanto, cuando Copilot me informó sobre el tipo std::variant, me ayudó bastante a estructurar el programa correctamente.

### 2. Qué partes fueron hechas por el equipo?

    Lo que hicimos todos en equipo fue el código del Interactive Menu. Durante las reuniones discutimos juntos la manera en la que debían organizarse las opciones y las funcionalidades que debían mostrarse. Al final, nos dimos cuenta de que lo más importante del código era desplegar la información estadística del CSV.

### 3. Cómo validaron los resultados?

    Validamos los datos logrando desplegar correctamente la información del CSV tanto de forma resumida en la terminal como en su propio formato TXT. Además, el código puede manejar errores como CSV vacíos o nombres incorrectos de archivos introducidos por el usuario.

### 4. Qué aprendieron del proceso?

    Aprendimos la importancia de utilizar clases para ir aumentando el nivel de abstracción en el programa. Por ejemplo, la clase Analisis utiliza frecuentemente la estructura de Dato.h, lo cual permite emplear su tipo de dato variante DataValue. Además, Dato.h incluye funciones que ayudan al reconocimiento de tipos de datos, como string e integer, lo cual finalmente es utilizado por el CSVAnalyzer.

### 5. Qué riesgos detectaron?

    Detectamos que, para CSV muy grandes (como aquellos con más de 400 registros), el algoritmo tarda bastante en cargarlos por completo para realizar la investigación estadística. Por lo tanto, añadimos una barra de carga para que el usuario pueda ver el avance y no se desespere al no observar ningún cambio.
    Por otro lado, para los huecos en la base de datos (celdas con valores nulos), utilizamos una función de reconocimiento que mantiene el valor nulo dentro del vector donde se guardan los datos. Además, cuando se calculan las estadísticas, dichos valores nulos no se toman en cuenta, por lo que no se produce ningún error que pudiera comprometer el funcionamiento del programa.

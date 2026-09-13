# Actividad 2.6: Problema práctico en OpenMP.

**Integrantes:**
* Álvaro Moisés Molina Alvarado
* Cesar Longoria Flores
* Ismael Pantoja Araiza

## Descripción de la solución
Este proyecto implementa un algoritmo de búsqueda exhaustiva (fuerza bruta) para encontrar una clave específica dentro de un espacio de búsqueda determinado. La solución está desarrollada en C++ y gestiona la memoria mediante arreglos dinámicos (sin utilizar `std::vector`), calculando las combinaciones al vuelo para evitar el desbordamiento de memoria. Se implementaron dos versiones: una secuencial que evalúa una combinación a la vez, y una versión paralela que distribuye el espacio de búsqueda entre múltiples hilos para reducir el tiempo de ejecución.

## Caracteres utilizados
El espacio de búsqueda está definido por un conjunto de **36 caracteres**:
* Letras mayúsculas: A-Z
* Números: 0-9

## Algoritmos implementados
1. **Fuerza bruta secuencial:** Recorrido lineal de todo el espacio de búsqueda desde el inicio hasta encontrar la clave.
2. **Fuerza bruta paralela (Distribución de trabajo):** División estática del número total de combinaciones entre los hilos disponibles para evaluar rangos simultáneamente.
3. **Conversión de base (Base-10 a Base-36):** Algoritmo para generar la representación en texto de cualquier combinación a partir de un índice numérico iterativo.

## Directivas OpenMP utilizadas
* `#pragma omp parallel`: Define y crea la región paralela donde operan los hilos.
* `num_threads(n)`: Especifica explícitamente la cantidad de hilos a lanzar.
* `shared(variable)`: Permite que todos los hilos monitoreen y accedan a la misma bandera de finalización y variables de resultados.
* `#pragma omp critical`: Establece un mecanismo de sincronización (exclusión mutua) que evita condiciones de carrera cuando un hilo encuentra la clave y actualiza el estado global, además de evitar que el texto en la consola se sobreescriba.

## Instrucciones básicas de compilación y ejecución

**1. Compilación (Se requiere soporte OpenMP, ej. GCC o MinGW):**
Desde la consola, ejecuta el siguiente comando:
g++ -fopenmp main.cpp -o busqueda

**2. Ejecución:**
En Windows:
busqueda.exe

En Linux o macOS:
./busqueda

*Nota: Al ejecutar el programa, se solicitará ingresar por consola una clave de prueba en MAYÚSCULAS sin espacios.*

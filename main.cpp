#include <iostream>
#include <cstring>
#include <cctype>
#include <omp.h>

using namespace std;

class FuerzaBruta {
private:
    const char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int base;
    char* clave_obj;
    int len;
    unsigned long long total;

public:
    FuerzaBruta(const char* clave) {
        base = strlen(charset);
        len = strlen(clave);

        // Uso de arreglo dinámico (requisito de la práctica)
        clave_obj = new char[len + 1];
        strcpy(clave_obj, clave);

        // Calcular combinaciones totales (base ^ longitud)
        total = 1;
        for (int i = 0; i < len; ++i) total *= base;
    }

    ~FuerzaBruta() {
        delete[] clave_obj; // Liberación de memoria dinámica
    }

    unsigned long long get_total() { return total; }

    // Convierte un índice numérico en su equivalente de texto base-36
    void generar_texto(unsigned long long indice, char* salida) {
        for (int i = len - 1; i >= 0; --i) {
            salida[i] = charset[indice % base];
            indice /= base;
        }
        salida[len] = '\0';
    }

    void correr_secuencial() {
        cout << "\n--- VERSION SECUENCIAL ---" << endl;
        char* intento = new char[len + 1];
        double inicio = omp_get_wtime();

        for (unsigned long long i = 0; i < total; ++i) {
            generar_texto(i, intento);
            if (strcmp(intento, clave_obj) == 0) {
                cout << "Clave encontrada: " << intento << " (Iteracion: " << i + 1 << ")\n";
                break;
            }
        }

        cout << "Tiempo secuencial: " << (omp_get_wtime() - inicio) << " seg.\n";
        delete[] intento;
    }

    void correr_paralelo(int hilos) {
        cout << "\n--- VERSION PARALELA (" << hilos << " hilos) ---" << endl;

        volatile bool encontrado = false; // Bandera compartida de sincronización
        int hilo_ganador = -1;
        char* clave_ganadora = new char[len + 1];

        double inicio = omp_get_wtime();

        // Crear región paralela y compartir variables
        #pragma omp parallel num_threads(hilos) shared(encontrado, hilo_ganador, clave_ganadora)
        {
            int id = omp_get_thread_num();
            int total_hilos = omp_get_num_threads();

            // Distribución equitativa del espacio de búsqueda
            unsigned long long rango = total / total_hilos;
            unsigned long long inicio_rango = id * rango;
            unsigned long long fin_rango = (id == total_hilos - 1) ? total : inicio_rango + rango;

            char* str_inicio = new char[len + 1];
            char* str_fin = new char[len + 1];
            generar_texto(inicio_rango, str_inicio);
            generar_texto(fin_rango - 1, str_fin);

            // Proteger salida a consola para evitar sobreescritura de texto
            #pragma omp critical
            {
                cout << "Hilo " << id << " | Inicio: " << str_inicio << " | Fin: " << str_fin << "\n";
            }

            char* intento = new char[len + 1];

            // Búsqueda iterativa (se detiene si otro hilo encuentra la clave)
            for (unsigned long long i = inicio_rango; i < fin_rango && !encontrado; ++i) {
                generar_texto(i, intento);

                if (strcmp(intento, clave_obj) == 0) {
                    #pragma omp critical // Proteger escritura en variables compartidas
                    {
                        if (!encontrado) {
                            encontrado = true;
                            hilo_ganador = id;
                            strcpy(clave_ganadora, intento);
                        }
                    }
                }
            }

            #pragma omp critical
            {
                if (encontrado && hilo_ganador == id) cout << "-> Hilo " << id << " encontro la clave.\n";
                else cout << "-> Hilo " << id << " detenido.\n";
            }

            delete[] str_inicio;
            delete[] str_fin;
            delete[] intento;
        }

        cout << "\nRESULTADO: Clave " << clave_ganadora << " encontrada por el HILO " << hilo_ganador << "\n";
        cout << "Tiempo paralelo: " << (omp_get_wtime() - inicio) << " seg.\n";
        delete[] clave_ganadora;
    }
};

// Validación de entrada
bool es_valida(const char* c) {
    if (strlen(c) == 0) return false;
    for (int i = 0; i < strlen(c); ++i) {
        if (!isalnum(c[i]) || islower(c[i])) return false;
    }
    return true;
}

int main() {
    char* buffer = new char[100];

    do {
        cout << "Ingresa clave de prueba (MAYUSCULAS y numeros): ";
        cin >> buffer;
    } while (!es_valida(buffer));

    FuerzaBruta buscador(buffer);
    cout << "Combinaciones totales: " << buscador.get_total() << "\n";

    buscador.correr_secuencial();
    buscador.correr_paralelo(4);

    delete[] buffer;
    return 0;
}

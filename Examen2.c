#include <stdio.h>
#include <stdlib.h>

// Empiezo con la estructura del nodo

typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;


// Son los prototipos
Nodo* crearNodo(int dato);
Nodo* insertar(Nodo* raiz, int dato);
void preOrden(Nodo* raiz);
void inOrden(Nodo* raiz);
void postOrden(Nodo* raiz);
Nodo* buscarNodo(Nodo* raiz, int clave, int *nivel, int *grado);
Nodo* minimoNodo(Nodo* raiz);
Nodo* eliminarNodo(Nodo* raiz, int clave, int *eliminado);
int nivelNodo(Nodo* raiz, int clave);
int gradoNodo(Nodo* raiz, int clave);
int contarHojas(Nodo* raiz);
void liberarArbol(Nodo* raiz);
int esLleno(Nodo* raiz);
int esPadre(Nodo* raiz, int char* valor);
int contarPadres(Nodo* raiz);


// Creo un nodo nuevo

Nodo* crearNodo(int dato) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        fprintf(stderr, "Error: memoria insuficiente\n");
        exit(EXIT_FAILURE);
    }
    nuevo->dato = dato;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}


// Inserto el siguiendo reglas de BST 
Nodo* insertar(Nodo* raiz, int dato) {
    if (raiz == NULL) {
        return crearNodo(dato);
    }

    if (dato < raiz->dato) {
        raiz->izq = insertar(raiz->izq, dato);
    } else if (dato > raiz->dato) {
        raiz->der = insertar(raiz->der, dato);
    } 
    return raiz;
}


// Estos son los recorridos

void preOrden(Nodo* raiz) {
    if (raiz == NULL) return;
    printf("%d ", raiz->dato);
    preOrden(raiz->izq);
    preOrden(raiz->der);
}

void inOrden(Nodo* raiz) {
    if (raiz == NULL) return;
    inOrden(raiz->izq);
    printf("%d ", raiz->dato);
    inOrden(raiz->der);
}

void postOrden(Nodo* raiz) {
    if (raiz == NULL) return;
    postOrden(raiz->izq);
    postOrden(raiz->der);
    printf("%d ", raiz->dato);
}


// Busco el nodo: lo devuelve puntero si lo encuentra.
// Devuelve por referencia el nivel (1 = raíz) y el grado (0/1/2).
// Si no existe, devuelve NULL y pone nivel = -1, grado = -1.

Nodo* buscarNodo(Nodo* raiz, int clave, int *nivel, int *grado) {
    int nivel_actual = 1;
    Nodo* actual = raiz;
    while (actual != NULL) {
        if (clave == actual->dato) {
            int g = 0;
            if (actual->izq) g++;
            if (actual->der) g++;
            if (nivel) *nivel = nivel_actual;
            if (grado) *grado = g;
            return actual;
        } else if (clave < actual->dato) {
            actual = actual->izq;
        } else {
            actual = actual->der;
        }
        nivel_actual++;
    }
    if (nivel) *nivel = -1;
    if (grado) *grado = -1;
    return NULL;
}


// Obtengo el menor nodo (mínimo) de un subárbol

Nodo* minimoNodo(Nodo* raiz) {
    Nodo* actual = raiz;
    while (actual != NULL && actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}


// Esto elimina el nodo en BST (considera 3 casos)
// Se usa *eliminado = 1 si se borró, 0 si no se encontró.

Nodo* eliminarNodo(Nodo* raiz, int clave, int *eliminado) {
    if (raiz == NULL) {
        if (eliminado) *eliminado = 0;
        return NULL;
    }

    if (clave < raiz->dato) {
        raiz->izq = eliminarNodo(raiz->izq, clave, eliminado);
    } else if (clave > raiz->dato) {
        raiz->der = eliminarNodo(raiz->der, clave, eliminado);
    } else {
        // Encontrado
        if (eliminado) *eliminado = 1;

        // Caso 1: hoja
        if (raiz->izq == NULL && raiz->der == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: un solo hijo (derecho)
        else if (raiz->izq == NULL) {
            Nodo* temp = raiz->der;
            free(raiz);
            return temp;
        }
        // Caso 2: un solo hijo (izquierdo)
        else if (raiz->der == NULL) {
            Nodo* temp = raiz->izq;
            free(raiz);
            return temp;
        }
        // Caso 3: dos hijos -> reemplazar por sucesor inorder (mínimo del subárbol derecho)
        else {
            Nodo* succ = minimoNodo(raiz->der);
            raiz->dato = succ->dato; // copiar el dato del sucesor
            // eliminar el sucesor en el subárbol derecho (ya existe)
            int dummy = 0;
            raiz->der = eliminarNodo(raiz->der, succ->dato, &dummy);
            // mantenemos *eliminado = 1 (ya lo pusimos arriba)
        }
    }
    return raiz;
}


// Determino el nivel (1 = raíz) o -1 si no existe

int nivelNodo(Nodo* raiz, int clave) {
    int nivel_actual = 1;
    Nodo* actual = raiz;
    while (actual != NULL) {
        if (clave == actual->dato) return nivel_actual;
        else if (clave < actual->dato) actual = actual->izq;
        else actual = actual->der;
        nivel_actual++;
    }
    return -1;
}


// Determino el grado (0/1/2) o -1 si no existe

int gradoNodo(Nodo* raiz, int clave) {
    int nivel, grado;
    Nodo* n = buscarNodo(raiz, clave, &nivel, &grado);
    if (!n) return -1;
    return grado;
}


// Cuenta los nodos hoja (recursivo)

int contarHojas(Nodo* raiz) {
    if (raiz == NULL) return 0;
    if (raiz->izq == NULL && raiz->der == NULL) return 1;
    return contarHojas(raiz->izq) + contarHojas(raiz->der);
}




// Libero todo el árbol

void liberarArbol(Nodo* raiz) {
    if (raiz == NULL) return;
    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);
    free(raiz);
}



    //Este es el nodo que detecta si el arbol esta vacio o lleno

int esLleno(Nodo *raiz) {
    if (raiz == NULL)
        return 1;

    if (raiz->izq == NULL && raiz->der == NULL)
        return 1;
    
    if (raiz->izq != NULL && raiz->der != NULL)
        return esLleno(raiz->izq) && esLleno(raiz->der);

    return 0;
}


    // Este es el nodo que dice si es psdre o no 

int esPadre(Nodo *raiz, cons char* valor) {
    if (raiz == NULL) {
        printf("El arbol esta vacio.\n");
        return 0;

     // Buscar el nodo con esos valores 
        
Nodo *actual = raiz;

    while (actual != NULL) {
        if (valor == actual->dato)
            break;
        else if (valor < actual->dato)
            actual = actual->izq;
        else
            actual = actual->der;
    }

    if (actual == NULL) {
        printf("El nodo no existe en el arbol.\n");
        return 0;
    }


        // Cuenta todos los padres del arbol
    
        int contarPadres(Nodo *raiz) {
        if (raiz == NULL)
        return 0;

        int contador = 0;


        if (raiz->izq != NULL || raiz->der != NULL)
        contador = 1;

    
        contador += contarPadres(raiz->izq);
        contador += contarPadres(raiz->der);

        return contador;
}
        



// Menú principal

int main(void) {
    Nodo *raiz = NULL;
    int opcion = 0;
    int valor;

    do {
        printf("\n MENU \n");
        printf("1. Insertar elemento\n");
        printf("2. Buscar nodo (muestra nivel y grado)\n");
        printf("3. Eliminar nodo\n");
        printf("4. Analizar nodo (nivel y grado)\n");
        printf("5. Mostrar recorrido PreOrden\n");
        printf("6. Mostrar recorrido InOrden\n");
        printf("7. Mostrar recorrido PostOrden\n");
        printf("8. Contar nodos hoja\n");
        printf("9. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            int c;
            while ((c = getchar()) != EOF && c != '\n');
            printf("Entrada invalida. Intenta otra vez.\n");
            opcion = 0;
            continue;
        }

        switch (opcion) {
            case 1:
                printf("Ingrese un valor entero a insertar: ");
                if (scanf("%d", &valor) != 1) {
                    int c; while ((c = getchar()) != EOF && c != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }
                raiz = insertar(raiz, valor);
                printf("-> %d insertado.\n", valor);
                break;

            case 2: {
                printf("Ingrese valor a buscar: ");
                if (scanf("%d", &valor) != 1) {
                    int c; while ((c = getchar()) != EOF && c != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }
                int nivel = -1, grado = -1;
                Nodo* res = buscarNodo(raiz, valor, &nivel, &grado);
                if (res) {
                    printf("Nodo %d encontrado en el nivel %d con grado %d\n", valor, nivel, grado);
                } else {
                    printf("Nodo %d NO encontrado en el arbol.\n", valor); 
                }
                break;
            }

            case 3: {
                printf("Ingrese valor a eliminar: ");
                if (scanf("%d", &valor) != 1) {
                    int c; while ((c = getchar()) != EOF && c != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }
                int eliminado = 0;
                raiz = eliminarNodo(raiz, valor, &eliminado);
                if (eliminado) {
                    printf("Nodo %d eliminado correctamente\n", valor);
                    printf("Arbol en InOrden: ");
                    inOrden(raiz);
                    printf("\n");
                } else {
                    printf("Nodo %d NO existe en el arbol.\n", valor);
                }
                break;
            }

            case 4: {
                printf("Ingrese valor a analizar (nivel y grado): ");
                if (scanf("%d", &valor) != 1) {
                    int c; while ((c = getchar()) != EOF && c != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }
                int n = nivelNodo(raiz, valor);
                int g = gradoNodo(raiz, valor);
                if (n != -1) {
                    printf("Nodo %d -> Nivel: %d, Grado: %d\n", valor, n, g);
                } else {
                    printf("Nodo %d no existe en el arbol.\n", valor);
                }
                break;
            }

            case 5:
                printf("Recorrido PreOrden: ");
                preOrden(raiz);
                printf("\n");
                break;

            case 6:
                printf("Recorrido InOrden: ");
                inOrden(raiz);
                printf("\n");
                break;

            case 7:
                printf("Recorrido PostOrden: ");
                postOrden(raiz);
                printf("\n");
                break;

            case 8: {
                int hojas = contarHojas(raiz);
                printf("Total de nodos hoja: %d\n", hojas);
                break;
            }

            case 9:
                printf("Saliendo...\n");
                break;
        
            
            // Esto solo imprime si esta lleno o no 
            
            case 10:
            if (esLleno(raiz))
            printf("El arbol esta lleno.\n");
            else
            printf("El arbol no esta lleno.\n");
            break;

            default:
            printf("Opcion invalida.\n");
        }
         
        // Cuenta los hijos
            case 11:
            int hijos = 0;
        if (actual->izq != NULL) hijos++;
        if (actual->der != NULL) hijos++;

        
         // A qui pone los datos que pide el ejercicio
        case 12:
        if (hijos == 0) {
        printf("Nodo con 0 hijos\n");
        return 0;   
        }
        else if (hijos == 1) {
        printf("Nodo con un hijo -> 1 hijo\n");
        return 1;
        }
        else {
        printf("Nodo con dos hijos -> 2 hijos\n");
        return 1;
    }
}
        
        } while (opcion != 12);

    liberarArbol(raiz);
    return 0;
}






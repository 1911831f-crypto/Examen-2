
#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
    int dato;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

// Crear nodo
Nodo* crearNodo(int valor) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato = valor;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

// Insertar en ABB
Nodo* insertar(Nodo *raiz, int valor) {
    if (raiz == NULL)
        return crearNodo(valor);

    if (valor < raiz->dato)
        raiz->izq = insertar(raiz->izq, valor);
    else if (valor > raiz->dato)
        raiz->der = insertar(raiz->der, valor);

    return raiz;
}

// ------------------------------
// 1) ¿Árbol lleno?
// ------------------------------
int esLleno(Nodo *raiz) {
    if (raiz == NULL)
        return 1; // Árbol vacío se considera lleno

    // Caso en que tiene ambos hijos
    if (raiz->izq != NULL && raiz->der != NULL)
        return esLleno(raiz->izq) && esLleno(raiz->der);

    // Caso en que no tiene hijos (nodo hoja)
    if (raiz->izq == NULL && raiz->der == NULL)
        return 1;

    // Si tiene solo un hijo -> NO es lleno
    return 0;
}

// ------------------------------
// 2) ¿Es padre? + Mostrar hijos
// ------------------------------
int esPadre(Nodo *raiz, int valor) {
    Nodo *actual = raiz;

    // Buscar nodo
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

    // Contar hijos
    int hijos = 0;
    if (actual->izq != NULL) hijos++;
    if (actual->der != NULL) hijos++;

    if (hijos == 0) {
        printf("Nodo con 0 hijos.\n");
        return 0;
    } else if (hijos == 1) {
        printf("Nodo con 1 hijo.\n");
        return 1;
    } else {
        printf("Nodo con 2 hijos.\n");
        return 1;
    }
}

// ------------------------------
// 3) Contar nodos padre
// ------------------------------
int contarPadres(Nodo *raiz) {
    if (raiz == NULL)
        return 0;

    int es_padre = 0;

    if (raiz->izq != NULL || raiz->der != NULL)
        es_padre = 1;

    return es_padre + contarPadres(raiz->izq) + contarPadres(raiz->der);
}

// ------------------------------
// Recorridos
// ------------------------------
void inOrden(Nodo *raiz) {
    if (raiz == NULL) return;
    inOrden(raiz->izq);
    printf("%d ", raiz->dato);
    inOrden(raiz->der);
}

void preOrden(Nodo *raiz) {
    if (raiz == NULL) return;
    printf("%d ", raiz->dato);
    preOrden(raiz->izq);
    preOrden(raiz->der);
}

void postOrden(Nodo *raiz) {
    if (raiz == NULL) return;
    postOrden(raiz->izq);
    postOrden(raiz->der);
    printf("%d ", raiz->dato);
}

// ------------------------------
//         MENÚ PRINCIPAL
// ------------------------------
int main() {
    Nodo *raiz = NULL;
    int opcion, valor;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Insertar nodo\n");
        printf("2. Mostrar recorridos\n");
        printf("3. Ver si el arbol es lleno\n");
        printf("4. Evaluar si un nodo es padre\n");
        printf("5. Contar nodos padre del arbol\n");
        printf("0. Salir\n");
        printf("Elige: ");
        scanf("%d", &opcion);

        switch (opcion) {

        case 1:
            printf("Ingresa valor: ");
            scanf("%d", &valor);
            raiz = insertar(raiz, valor);
            break;

        case 2:
            printf("InOrden: ");
            inOrden(raiz);
            printf("\nPreOrden: ");
            preOrden(raiz);
            printf("\nPostOrden: ");
            postOrden(raiz);
            printf("\n");
            break;

        case 3:
            if (esLleno(raiz))
                printf("El arbol ES lleno.\n");
            else
                printf("El arbol NO es lleno.\n");
            break;

        case 4:
            printf("Ingresa el valor del nodo: ");
            scanf("%d", &valor);
            esPadre(raiz, valor);
            break;

        case 5:
            printf("Cantidad de nodos padre: %d\n", contarPadres(raiz));
            break;

        }

    } while (opcion != 0);

    return 0;
}





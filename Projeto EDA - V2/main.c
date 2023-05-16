#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int main() {

    // call the menu function with the loaded or empty graph
    registo* headR = NULL;
    cliente* headC = NULL;
    gestor* headG = NULL;
    grafo* headV = NULL;
    meio* headM = NULL;
    aresta* headA = NULL;

    // Verifica se o arquivo existe
    FILE* fp = fopen("grafo.txt", "r");
    if (fp) {
        // Se o arquivo existir, exibe o menu
        fclose(fp);
        showMenu(&headC, &headG, &headM, &headR, &headV, &headA);
    }
    else {
        // Se o arquivo não existir, cria um novo grafo, salva-o no arquivo e exibe o menu
        headV = criarGrafo();
        salvarGrafo(headV);
        showMenu(&headC, &headG, &headM, &headR, &headV, &headA);
        // Libera a memória alocada para o grafo após o uso.
        free(headV->vertices);
        free(headV);
    }
    return 0;
}

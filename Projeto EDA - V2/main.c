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
    meio* headM = NULL;

    // Verifica se o arquivo existe
    FILE* fp = fopen("grafo.txt", "r");
    if (fp) {
        // Se o arquivo existir, carrega o grafo a partir do arquivo e exibe o menu
        fclose(fp);
        grafo* g = carregarGrafo("grafo.txt");
        showMenu(&headC, &headG, &headM, &headR);
    }
    else {
        // Se o arquivo não existir, cria um novo grafo, salva-o no arquivo e exibe o menu
        grafo* g = criarGrafo();
        salvarGrafo(g, "grafo.txt");
        showMenu(&headC, &headG, &headM, &headR);
        // Libera a memória alocada para o grafo após o uso.
        free(g->vertices);
        free(g);
    }
    return 0;
}

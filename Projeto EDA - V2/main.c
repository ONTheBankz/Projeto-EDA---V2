#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int fileExiste(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;  // Ficheiro existe
    }
    return 0;  // Ficheiro não existe
}

int main() {
    registo* headR = NULL;
    cliente* headC = NULL;
    gestor* headG = NULL;
    grafo* headV = NULL;
    meio* headM = NULL;
    aresta* headA = NULL;

    // Verifica se o arquivo "grafo.txt" existe
    if (fileExiste("grafo.txt")) {
        // Se o arquivo existir, carrega o grafo do arquivo
        headV = carregarGrafo();
    }
    else {
        // Se o arquivo não existir, cria um novo grafo e salva-o no arquivo
        headV = criarGrafo();
        salvarGrafo(headV);
    }

    // Verifica se o arquivo "arestas.txt" existe
    if (fileExiste("arestas.txt")) {
        // Se o arquivo existir, carrega as arestas do arquivo
        headA = carregarAresta();
    }
    else {
        // Se o arquivo não existir, cria novas arestas e salva-as no arquivo
        headA = criarConexao();
        salvarAresta(headA);
    }

    // Exibe o menu
    showMenu(&headC, &headG, &headM, &headR, &headV, &headA);
    return 0;
}

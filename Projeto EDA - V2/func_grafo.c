#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

grafo* criarGrafo() {
    grafo* g = (grafo*)malloc(sizeof(grafo));
    g->num_vertices = 8;
    g->num_arestas = 0;
    g->vertices = NULL;

    char nomes[8][50] = {
        "rua.das.flores",
        "rua.das.araras",
        "rua.dos.jasmins",
        "rua.dos.colibris",
        "rua.dos.ipes",
        "rua.dos.sabias",
        "rua.dos.lirios",
        "rua.dos.beija-flores"
    };

    for (int i = 0; i < g->num_vertices; i++) {
        vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
        novoVertice->id = i;
        strcpy(novoVertice->nome, nomes[i]);
        novoVertice->arestas = NULL;
        novoVertice->seguinte = NULL;

        if (g->vertices == NULL) {
            g->vertices = novoVertice;
        }
        else {
            vertice* atual = g->vertices;
            while (atual->seguinte != NULL) {
                atual = atual->seguinte;
            }
            atual->seguinte = novoVertice;
        }
    }

    return g;
}

grafo* carregarGrafo() {
    grafo* g = (grafo*)malloc(sizeof(grafo));
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->vertices = NULL;

    FILE* arquivo = fopen("grafo.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return g;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        int id;
        char nome[50];
        if (sscanf(linha, "ID: %d, Nome: %[^\n]", &id, nome) == 2) {
            vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
            novoVertice->id = id;
            strcpy(novoVertice->nome, nome);
            novoVertice->arestas = NULL;
            novoVertice->seguinte = NULL;

            if (g->vertices == NULL) {
                g->vertices = novoVertice;
            }
            else {
                vertice* atual = g->vertices;
                while (atual->seguinte != NULL) {
                    atual = atual->seguinte;
                }
                atual->seguinte = novoVertice;
            }

            g->num_vertices++;
        }
    }

    fclose(arquivo);

    return g;
}

vertice* buscarVertice(grafo* g, int id) {
    g = carregarGrafo();
    vertice* atual = g->vertices;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->seguinte;
    }
    return NULL; // Retorna NULL se o vértice não for encontrado
}

void salvarGrafo(grafo* g) {
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    vertice* atual = g->vertices;
    while (atual != NULL) {
        fprintf(arquivo, "ID: %d, Nome: %s\n", atual->id, atual->nome);
        atual = atual->seguinte;
    }

    fclose(arquivo);
}

void criarVertice(grafo* g) {
    char nome[50];
    g = carregarGrafo();
    // Solicitar o nome do vértice ao usuário
    printf("Digite o nome do vertice: ");
    scanf("%s", nome);

    // Criar um novo vértice
    vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
    novoVertice->id = g->num_vertices;
    strcpy(novoVertice->nome, nome);
    novoVertice->arestas = NULL;
    novoVertice->seguinte = NULL;

    // Adicionar o novo vértice ao grafo
    if (g->vertices == NULL) {
        g->vertices = novoVertice;
    }
    else {
        vertice* atual = g->vertices;
        while (atual->seguinte != NULL) {
            atual = atual->seguinte;
        }
        atual->seguinte = novoVertice;
    }

    g->num_vertices++;

    // Atualizar o arquivo "grafo.txt" com os novos valores
    FILE* arquivo = fopen("grafo.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "ID: %d, Nome: %s\n", novoVertice->id, novoVertice->nome);

    fclose(arquivo);

    printf("\nNovo vertice criado com sucesso!\n");
    getchar();
}

void editarVertice(grafo* g) {
    g = carregarGrafo();
    int id;
    char novoNome[50];

    imprimirVertices(g);

    // Solicitar o ID do vértice a ser editado
    printf("\nDigite o ID do vertice a ser editado:");
    scanf("%d", &id);

    // Procurar o vértice com o ID fornecido
    vertice* atual = g->vertices;
    while (atual != NULL) {
        if (atual->id == id) {
            break;
        }
        atual = atual->seguinte;
    }

    if (atual == NULL) {
        printf("\nVertice com o ID %d nao encontrado.", id);
        return;
    }

    // Solicitar o novo nome do vértice ao usuário
    printf("\nDigite o novo nome do vertice: ");
    scanf("%s", novoNome);

    // Atualizar o nome do vértice
    strcpy(atual->nome, novoNome);

    // Atualizar o arquivo "grafo.txt" com os novos valores
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escrever os vértices atualizados no arquivo
    vertice* temp = g->vertices;
    while (temp != NULL) {
        fprintf(arquivo, "ID: %d, Nome: %s\n", temp->id, temp->nome);
        temp = temp->seguinte;
    }

    fclose(arquivo);

    printf("\nVertice editado com sucesso!\n");
    getchar();
}

void removerVertice(grafo* g) {
    g = carregarGrafo();
    int id;

    imprimirVertices(g);

    // Solicitar o ID do vértice a ser removido
    printf("\nDigite o ID do vertice a ser removido:");
    scanf("%d", &id);

    // Verificar se o vértice existe
    vertice* atual = g->vertices;
    vertice* anterior = NULL;
    while (atual != NULL) {
        if (atual->id == id) {
            break;
        }
        anterior = atual;
        atual = atual->seguinte;
    }

    if (atual == NULL) {
        printf("\nVertice com o ID %d nao encontrado.", id);
        return;
    }

    // Remover o vértice da lista de vértices
    if (anterior == NULL) {
        // O vértice a ser removido é o primeiro da lista
        g->vertices = atual->seguinte;
    }
    else {
        // O vértice a ser removido está no meio ou no final da lista
        anterior->seguinte = atual->seguinte;
    }

    // Liberar a memória alocada pelo vértice removido
    free(atual);

    // Atualizar o arquivo "grafo.txt" com os novos valores
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escrever os vértices atualizados no arquivo
    vertice* temp = g->vertices;
    while (temp != NULL) {
        fprintf(arquivo, "ID: %d, Nome: %s\n", temp->id, temp->nome);
        temp = temp->seguinte;
    }

    fclose(arquivo);

    printf("\nVertice removido com sucesso!\n");
    getchar();
}

void imprimirVertices(grafo* g) {
    g = carregarGrafo();
    printf("Localizacoes:\n\n");
    // Mostra a lista de vértices
    vertice* v = (g)->vertices;
    while (v != NULL) {
        printf("ID: %d | Nome: %s\n", v->id, v->nome);
        v = v->seguinte;
    }
}

void criarAresta(grafo* g) {
    g = carregarGrafo();
    int id_origem, id_destino, peso;
    imprimirVertices(g);

    // Obter as entradas do usuário
    printf("\nDigite o ID do vertice de origem: ");
    scanf("%d", &id_origem);

    printf("Digite o ID do vertice de destino: ");
    scanf("%d", &id_destino);

    // Verificar se os IDs de origem e destino existem nos vértices do grafo
    vertice* vertice_origem = buscarVertice(g, id_origem);
    vertice* vertice_destino = buscarVertice(g, id_destino);
    if (vertice_origem == NULL || vertice_destino == NULL) {
        printf("\nErro: IDs de origem ou destino invalidos.\n");
        getchar();
        return;
    }

    printf("Digite o peso da aresta: ");
    scanf("%d", &peso);

    // Criar uma nova aresta
    aresta* nova_aresta = (aresta*)malloc(sizeof(aresta));
    if (nova_aresta == NULL) {
        printf("Erro: Falha ao alocar memoria para a nova aresta.\n");
        return;
    }
    nova_aresta->id_origem = id_origem;
    nova_aresta->id_destino = id_destino;
    nova_aresta->peso = peso;
    nova_aresta->proxima = NULL;

    // Abrir o arquivo em modo de apêndice
    FILE* arquivo = fopen("arestas.txt", "a");
    if (arquivo == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        free(nova_aresta);
        return;
    }

    // Escrever a aresta no arquivo
    fprintf(arquivo, "%d %d %d\n", nova_aresta->id_origem, nova_aresta->id_destino, nova_aresta->peso);

    // Fechar o arquivo
    fclose(arquivo);

    printf("\nConexao das localizacoes criada com sucesso!\n");
    getchar();
}













































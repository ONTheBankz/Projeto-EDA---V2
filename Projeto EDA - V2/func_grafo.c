#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

grafo* criarGrafo() {
    // Declara o número de vértices e os seus nomes
    int num_vertices = 8;
    char* nomes_vertices[8] = { "rua.das.flores", "rua.das.araras",
                                "av.marechal.deodoro", "rua.joao.pessoa",
                                "rua.do.ribeiro", "rua.alberto.silva",
                                "rua.da.praia", "av.dos.estados" };

    // Aloca memória para o grafo e os seus vértices
    grafo* g = (grafo*)malloc(sizeof(grafo));
    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->vertices = (vertice*)malloc(num_vertices * sizeof(vertice));

    // Cria os vértices com os nomes fornecidos
    for (int i = 0; i < num_vertices; i++) {
        strcpy(g->vertices[i].nome, nomes_vertices[i]);
        g->vertices[i].id = i;
        g->vertices[i].meios = NULL;
        g->vertices[i].arestas = NULL;
        g->vertices[i].seguinte = NULL;
    }

    // Conecta os vértices com as arestas
    conectarVertices(g, 0, 1, 10, 2.5);   // rua.das.flores -> rua.das.araras
    conectarVertices(g, 0, 2, 5, 1.2);    // rua.das.flores -> av.marechal.deodoro
    conectarVertices(g, 1, 3, 7, 1.9);    // rua.das.araras -> rua.joao.pessoa
    conectarVertices(g, 2, 3, 4, 1.0);    // av.marechal.deodoro -> rua.joao.pessoa
    conectarVertices(g, 2, 4, 2, 0.5);    // av.marechal.deodoro -> rua.do.ribeiro
    conectarVertices(g, 3, 4, 5, 1.3);    // rua.joao.pessoa -> rua.do.ribeiro
    conectarVertices(g, 4, 5, 3, 0.7);    // rua.do.ribeiro -> rua.alberto.silva
    conectarVertices(g, 4, 6, 9, 2.1);    // rua.do.ribeiro -> rua.da.praia
    conectarVertices(g, 5, 7, 8, 1.8);    // rua.alberto.silva -> av.dos.estados
    conectarVertices(g, 6, 7, 6, 1.5);    // rua.da.praia -> av.dos.estados

    return g;
}

void conectarVertices(grafo* g, int id_origem, int id_destino, int peso, float distancia) {
    // Cria uma nova aresta
    aresta* nova_aresta = (aresta*)malloc(sizeof(aresta));
    nova_aresta->id_origem = id_origem;
    nova_aresta->id_destino = id_destino;
    nova_aresta->peso = peso;
    nova_aresta->distancia = distancia;
    nova_aresta->proxima = NULL;

    // Adiciona a nova aresta ao vértice de origem
    vertice* origem = &g->vertices[id_origem];
    if (origem->arestas == NULL) {
        origem->arestas = nova_aresta;
    }
    else {
        aresta* aresta_atual = origem->arestas;
        while (aresta_atual->proxima != NULL) {
            aresta_atual = aresta_atual->proxima;
        }
        aresta_atual->proxima = nova_aresta;
    }

    // Incrementa o num de arestas no grafo
    g->num_arestas++;
}

void salvarGrafo(const grafo* g, const char* nome_arquivo) {
    FILE* fp = fopen(nome_arquivo, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nome_arquivo);
        return;
    }

    // Escreve o número de vértices e arestas
    fprintf(fp, "%d %d\n", g->num_vertices, g->num_arestas);

    // Escreve os vértices
    for (int i = 0; i < g->num_vertices; i++) {
        fprintf(fp, "%d %s\n", g->vertices[i].id, g->vertices[i].nome);
    }

    // Escreve as arestas
    for (int i = 0; i < g->num_vertices; i++) {
        aresta* a = g->vertices[i].arestas;
        while (a != NULL) {
            fprintf(fp, "%d %d %d %.2f\n", a->id_origem, a->id_destino, a->peso, a->distancia);
            a = a->proxima;
        }
    }

    fclose(fp);
}

grafo* carregarGrafo(const char* nome_arquivo) {
    FILE* fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo %s para leitura.\n", nome_arquivo);
        return NULL;
    }

    // Lê o número de vértices e arestas
    int num_vertices, num_arestas;
    fscanf(fp, "%d %d", &num_vertices, &num_arestas);

    // Aloca memória para o grafo e os vértices
    grafo* g = (grafo*)malloc(sizeof(grafo));
    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->vertices = (vertice*)malloc(num_vertices * sizeof(vertice));

    // Lê os vértices
    for (int i = 0; i < num_vertices; i++) {
        int id;
        char nome[100];
        fscanf(fp, "%d %s", &id, nome);
        strcpy(g->vertices[i].nome, nome);
        g->vertices[i].id = id;
        g->vertices[i].meios = NULL;
        g->vertices[i].arestas = NULL;
        g->vertices[i].seguinte = NULL;
    }

    // Lê as arestas
    for (int i = 0; i < num_vertices; i++) {
        int id_origem, id_destino, peso;
        float distancia;
        while (fscanf(fp, "%d:%d %d %f", &id_origem, &id_destino, &peso, &distancia) == 4) {
            conectarVertices(g, id_origem, id_destino, peso, distancia);
        }
    }

    fclose(fp);
    return g;
}





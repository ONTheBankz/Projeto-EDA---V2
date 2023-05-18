#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

aresta* carregarAresta() {
    // Open the file in read mode
    FILE* arquivo = fopen("arestas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        return NULL;
    }

    aresta* lista_arestas = NULL;  // Linked list to store the edges

    // Read the values from the file and create edges
    int id_origem, id_destino, peso;
    while (fscanf(arquivo, "%d %d %d", &id_origem, &id_destino, &peso) == 3) {
        // Create a new edge
        aresta* nova_aresta = (aresta*)malloc(sizeof(aresta));
        if (nova_aresta == NULL) {
            printf("Erro: Falha ao alocar memoria para a nova aresta.\n");
            fclose(arquivo);
            return lista_arestas;
        }
        nova_aresta->id_origem = id_origem;
        nova_aresta->id_destino = id_destino;
        nova_aresta->peso = peso;
        nova_aresta->proxima = NULL;

        // Add the edge to the linked list
        if (lista_arestas == NULL) {
            lista_arestas = nova_aresta;
        }
        else {
            aresta* atual = lista_arestas;
            while (atual->proxima != NULL) {
                atual = atual->proxima;
            }
            atual->proxima = nova_aresta;
        }
    }

    // Close the file
    fclose(arquivo);

    return lista_arestas;
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

char* obterNomeVertice(grafo* g, int id_vertice) {
    vertice* verticePtr = g->vertices;
    while (verticePtr != NULL && verticePtr->id != id_vertice) {
        verticePtr = verticePtr->seguinte;
    }

    if (verticePtr != NULL) {
        return _strdup(verticePtr->nome);
    }

    return NULL;
}

void compararNomesVertices(grafo* g, meio* m, char** nomesVertices, int numVertices) {
    // Abrir arquivo em modo de leitura
    FILE* txt_file = fopen("meios.txt", "r");
    if (txt_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os meios para uma lista ligada
    meio* head = NULL;
    meio* curr = NULL;
    lerMeios(txt_file, &head);
    fclose(txt_file);

    printf("\nMeios disponiveis nos locais:\n\n");

    // Percorrer os nomes dos vizinhos encontrados
    for (int i = 0; i < numVertices; i++) {
        char* nomeVizinho = nomesVertices[i];

        // Percorrer a lista de meios
        curr = head;
        while (curr != NULL) {
            // Comparar o nome do vizinho com o local_grafo do meio
            if (strcmp(nomeVizinho, curr->local_grafo) == 0) {
                // Meio encontrado no local vizinho, imprimir informações
                printf("ID: %d\n", curr->id);
                printf("Tipo: %s\n", curr->tipo);
                printf("Custo: %.2f\n", curr->custo);
                printf("Bateria: %.2f\n", curr->bateria);
                printf("Local: %s\n", curr->local_grafo);
                printf("Reserva: %d\n", curr->reserva);
                printf("\n");
            }
            curr = curr->seguinte;
        }
    }
}

void criarAresta(grafo* g) {
    g = carregarGrafo();
    int id_origem, id_destino, peso;
    imprimirVertices(g);

    // Obter as entradas do usuário
    printf("\nDigite o ID da localizacao de origem: ");
    scanf("%d", &id_origem);

    printf("Digite o ID da localizacao de destino: ");
    scanf("%d", &id_destino);

    // Verificar se os IDs de origem e destino existem nos vértices do grafo
    vertice* vertice_origem = buscarVertice(g, id_origem);
    vertice* vertice_destino = buscarVertice(g, id_destino);
    if (vertice_origem == NULL || vertice_destino == NULL) {
        printf("\nErro: IDs de origem e/ou destino invalidos.\n");
        getchar();
        return;
    }

    // Verificar se o ID de origem é igual ao ID de destino
    if (id_origem == id_destino) {
        printf("\nErro: O ID de origem nao pode ser igual ao ID de destino.\n");
        getchar();
        return;
    }

    printf("Digite a distancia: ");
    scanf("%d", &peso);

    // Criar uma nova aresta
    aresta* nova_aresta = (aresta*)malloc(sizeof(aresta));
    if (nova_aresta == NULL) {
        printf("Erro: Falha ao alocar memoria para a nova conexao.\n");
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

void removerAresta(aresta* a) {
    a = carregarAresta();
    if (a == NULL) {
        printf("Erro: Lista de conexoes vazia.\n");
        getchar();
        return;
    }

    imprimirAresta(a);

    int id_origem, id_destino;
    printf("\nDigite o ID da localizacao de origem da conexao a ser removida: ");
    scanf("%d", &id_origem);

    printf("\nDigite o ID da localizacao de destino da conexao a ser removida: ");
    scanf("%d", &id_destino);

    aresta* anterior = NULL;
    aresta* atual = a;

    // Traverse the linked list to find the edge to be removed
    while (atual != NULL) {
        if (atual->id_origem == id_origem && atual->id_destino == id_destino) {
            // Edge found, remove it from the linked list
            if (anterior == NULL) {
                a = atual->proxima;
            }
            else {
                anterior->proxima = atual->proxima;
            }

            // Free the memory occupied by the edge
            free(atual);
            printf("\Conexao removida com sucesso.\n");
            getchar();
            // Update the file
            atualizarAresta(a);

            return;
        }

        anterior = atual;
        atual = atual->proxima;
    }

    printf("\nErro: Conexao nao encontrada.\n");
    getchar();
}

void imprimirAresta(aresta* a) {
    a = carregarAresta();
    printf("\nLista de conexoes:\n\n");
    aresta* atual = a;
    while (atual != NULL) {
        printf("Origem: %d, Destino: %d, Peso: %d\n", atual->id_origem, atual->id_destino, atual->peso);
        atual = atual->proxima;
    }
}

void atualizarAresta(aresta* a) {
    // Open the file in write mode (this will clear its contents)
    FILE* arquivo = fopen("arestas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        return;
    }

    // Write the edges from the linked list to the file
    aresta* atual = a;
    while (atual != NULL) {
        fprintf(arquivo, "%d %d %d\n", atual->id_origem, atual->id_destino, atual->peso);
        atual = atual->proxima;
    }

    // Close the file
    fclose(arquivo);
}

void verConexoesRaio(grafo* g, aresta* a, meio* m) {
    g = carregarGrafo();
    a = carregarAresta();
    imprimirVertices(g);

    int id_origem;
    float raio;

    printf("\nDigite o ID onde se encontra: ");
    scanf("%d", &id_origem);

    printf("Digite o raio de distancia: ");
    scanf("%f", &raio);

    printf("\nConexoes dentro do raio de %.2f a partir do ID %d:\n\n", raio, id_origem);
    encontrarConexoes(g, a, m, id_origem, raio);
    getchar();
}

void encontrarConexoes(grafo* g, aresta* a, meio* m, int id_origem, float raio) {
    // Criar uma lista ligada para acompanhar os vértices visitados
    bool* visitados = calloc(g->num_vertices, sizeof(bool));
    // Criar uma lista ligada para armazenar as distâncias acumuladas
    int* distanciasAcumuladas = calloc(g->num_vertices, sizeof(int));
    // Criar uma lista ligada para armazenar os nomes dos vizinhos
    char** nomesVertices = calloc(g->num_vertices, sizeof(char*));
    int numVertices = 0;

    // Marcar o vértice de origem como visitado e inseri-lo na lista
    visitados[id_origem] = true;
    int* fila = malloc(g->num_vertices * sizeof(int));
    int inicio = 0, fim = 0;
    fila[fim++] = id_origem;

    while (inicio != fim) {
        int verticeAtual = fila[inicio++];
        vertice* verticeAtualPtr = buscarVertice(g, verticeAtual);

        if (verticeAtualPtr == NULL) {
            continue;  // Vértice não encontrado no grafo
        }

        for (aresta* arestaAtual = a; arestaAtual != NULL; arestaAtual = arestaAtual->proxima) {
            int idVertice = -1;

            if (arestaAtual->id_origem == verticeAtual && !visitados[arestaAtual->id_destino]) {
                idVertice = arestaAtual->id_destino;
            }
            else if (arestaAtual->id_destino == verticeAtual && !visitados[arestaAtual->id_origem]) {
                idVertice = arestaAtual->id_origem;
            }

            if (idVertice != -1) {
                vertice* vizinhoPtr = buscarVertice(g, idVertice);

                if (vizinhoPtr != NULL) {
                    int distanciaAcumulada = distanciasAcumuladas[verticeAtual] + arestaAtual->peso;

                    if (distanciaAcumulada <= raio) {
                        visitados[idVertice] = true;
                        fila[fim++] = idVertice;
                        distanciasAcumuladas[idVertice] = distanciaAcumulada;
                        nomesVertices[numVertices] = obterNomeVertice(g, idVertice);
                        numVertices++;
                        imprimirConexoes(verticeAtual, idVertice, distanciaAcumulada);
                    }
                }
            }
        }
    }
        compararNomesVertices(g, m, nomesVertices, numVertices);
}

void imprimirConexoes(int origem, int destino, int distancia) {
    printf("Origem: %d, Destino: %d, Distancia: %d\n", origem, destino, distancia);
}





































































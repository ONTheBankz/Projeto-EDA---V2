#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include "structs.h"

grafo* criarGrafo() {
    grafo* g = (grafo*)malloc(sizeof(grafo));
    g->num_vertices = 8;
    g->num_arestas = 0;
    g->vertices = NULL;

    char nomes[8][50] = {
        "///malas.fixado.facam",
        "///provou.evita.irei",
        "///supoe.suponha.lenha",
        "///trazem.meteoro.zerado",
        "///ionico.falou.cidada",
        "///panelao.cetim.taxas",
        "///galao.conterraneos.marte",
        "///tunica.faisca.calo"
    };

    char local_meios[8][50] = {
      "Jardim_St_Barbara",
      "Se_Braga",
      "Monte_Picoto",
      "Braga_Parque",
      "Uni_Minho",
      "Parque_Rodovia",
      "Hospital",
      "Estadio"
    };

    for (int i = 0; i < g->num_vertices; i++) {
        vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
        novoVertice->id = i;
        strcpy(novoVertice->nome, nomes[i]);
        strcpy(novoVertice->local_meio, local_meios[i]);
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
        char local_meio[50];
        char nome[50];
        if (sscanf(linha, "%d %s %s\n", &id, local_meio, nome) == 3) {
            vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
            novoVertice->id = id;
            strcpy(novoVertice->local_meio, local_meio);
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
    // Abre o ficheiro em modo leitura
    FILE* arquivo = fopen("arestas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        return NULL;
    }

    aresta* lista_arestas = NULL;  // Lista ligada para guardar as arestas

    // L� os valores do ficheiro e cria as arestas
    int id_origem, id_destino, peso;
    while (fscanf(arquivo, "%d %d %d", &id_origem, &id_destino, &peso) == 3) {
        // Cria uma nova aresta
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

        // Adiciona a aresta � lista ligada
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

    // Fecha o ficheiro
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
    return NULL; // Retorna NULL se o v�rtice n�o for encontrado
}

void salvarGrafo(grafo* g) {
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    vertice* atual = g->vertices;
    while (atual != NULL) {
        fprintf(arquivo, "%d %s %s\n", atual->id, atual->local_meio, atual->nome);
        atual = atual->seguinte;
    }

    fclose(arquivo);
}

void criarVertice(grafo* g) {
    char nome[50];
    char local_meio[50];
    g = carregarGrafo();
    // Solicitar o nome do v�rtice ao usu�rio
    printf("Digite o geocodigo do vertice: ");
    scanf("%s", nome);

    printf("\nDigite o local do vertice: ");
    scanf("%s", local_meio);

    // Criar um novo v�rtice
    vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
    novoVertice->id = g->num_vertices;
    strcpy(novoVertice->nome, nome);
    strcpy(novoVertice->local_meio, local_meio);
    novoVertice->arestas = NULL;
    novoVertice->seguinte = NULL;

    // Adicionar o novo v�rtice ao grafo
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

    fprintf(arquivo, "ID: %d, Local: %s, Nome: %s\n", novoVertice->id, novoVertice->local_meio, novoVertice->nome);

    fclose(arquivo);

    printf("\nNovo vertice criado com sucesso!\n");
    getchar();
}

void editarVertice(grafo* g) {
    g = carregarGrafo();
    int id;
    char novoNome[50];
    char novoLocal[50];

    imprimirVertices(g);

    // Solicitar o ID do v�rtice a ser editado
    printf("\nDigite o ID do vertice a ser editado:");
    scanf("%d", &id);

    // Procurar o v�rtice com o ID fornecido
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

    // Solicitar o novo geocodigo do v�rtice ao usu�rio
    printf("\nDigite o novo geocodigo do vertice: ");
    scanf("%s", novoNome);

    // Atualizar o geocodigo do v�rtice
    strcpy(atual->nome, novoNome);

    // Solicitar o novo local do v�rtice ao usu�rio
    printf("\nDigite o novo local do vertice: ");
    scanf("%s", novoLocal);

    // Atualizar o local do v�rtice
    strcpy(atual->local_meio, novoLocal);

    // Atualizar o arquivo "grafo.txt" com os novos valores
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escrever os v�rtices atualizados no arquivo
    vertice* novoVertice = g->vertices;
    while (novoVertice != NULL) {
        fprintf(arquivo, "%d %s %s\n", novoVertice->id, novoVertice->local_meio, novoVertice->nome);
        novoVertice = novoVertice->seguinte;
    }

    fclose(arquivo);

    system("clear || cls");
    printf("\nVertice editado com sucesso!\n");
    getchar();
}

void removerVertice(grafo* g) {
    g = carregarGrafo();
    int id;

    imprimirVertices(g);

    // Solicitar o ID do v�rtice a ser removido
    printf("\nDigite o ID do vertice a ser removido:");
    scanf("%d", &id);

    // Verificar se o v�rtice existe
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

    // Remover o v�rtice da lista de v�rtices
    if (anterior == NULL) {
        // O v�rtice a ser removido � o primeiro da lista
        g->vertices = atual->seguinte;
    }
    else {
        // O v�rtice a ser removido est� no meio ou no final da lista
        anterior->seguinte = atual->seguinte;
    }

    // Liberar a mem�ria alocada pelo v�rtice removido
    free(atual);

    // Atualizar o arquivo "grafo.txt" com os novos valores
    FILE* arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escrever os v�rtices atualizados no arquivo
    vertice* novoVertice = g->vertices;
    while (novoVertice != NULL) {
        fprintf(arquivo, "%d %s %s\n", novoVertice->id, novoVertice->local_meio, novoVertice->nome);
        novoVertice = novoVertice->seguinte;
    }

    fclose(arquivo);

    system("clear || cls");
    printf("\nVertice removido com sucesso!\n");
    getchar();
}

void imprimirVertices(grafo* g) {
    printf("Localizacoes:\n\n");
    printf("%-5s | %-17s | %s\n", "ID", "Local", "Nome");
    printf("----------------------------------\n");

    vertice* v = g->vertices;
    while (v != NULL) {
        printf("%-5d | %-17s | %s\n", v->id, v->local_meio, v->nome);
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

void compararNomesVertices(grafo* g, meio* m, char** nomesVertices, int numVertices, const char* tipo_meio) {
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

    // Verificar se o tipo_meio � "Bicicleta" ou "Trotinete"
    if (strcmp(tipo_meio, "Bicicleta") != 0 && strcmp(tipo_meio, "Trotinete") != 0) {
        printf("Tipo de meio inv�lido! Deve ser 'Bicicleta' ou 'Trotinete'.\n");
        return;
    }

    // Percorrer os nomes dos vizinhos encontrados
    for (int i = 0; i < numVertices; i++) {
        char* nomeVertice = nomesVertices[i];

        // Percorrer a lista de meios
        curr = head;
        while (curr != NULL) {
            // Comparar o nome do vizinho com o local_grafo do meio e o tipo
            if (strcmp(nomeVertice, curr->local_grafo) == 0 && strcmp(tipo_meio, curr->tipo) == 0) {
                // Meio encontrado no local vizinho e do tipo desejado, imprimir informa��es
                printf("ID: %d\n", curr->id);
                printf("Tipo: %s\n", curr->tipo);
                printf("Custo: %.2f\n", curr->custo);
                printf("Bateria: %.2f\n", curr->bateria);
                printf("Local: %s\n", curr->local);
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

    // Obter as entradas do usu�rio
    printf("\nDigite o ID da localizacao de origem: ");
    scanf("%d", &id_origem);

    printf("Digite o ID da localizacao de destino: ");
    scanf("%d", &id_destino);

    // Verificar se os IDs de origem e destino existem nos v�rtices do grafo
    vertice* vertice_origem = buscarVertice(g, id_origem);
    vertice* vertice_destino = buscarVertice(g, id_destino);
    if (vertice_origem == NULL || vertice_destino == NULL) {
        printf("\nErro: IDs de origem e/ou destino invalidos.\n");
        getchar();
        return;
    }

    // Verificar se o ID de origem � igual ao ID de destino
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

    // Abrir o arquivo em modo de ap�ndice
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

    // Atravessa a lista para encontrar a aresta a ser removida
    while (atual != NULL) {
        if (atual->id_origem == id_origem && atual->id_destino == id_destino) {
            // Aresta encontrada, remove-a da lista 
            if (anterior == NULL) {
                a = atual->proxima;
            }
            else {
                anterior->proxima = atual->proxima;
            }

            // Liberta a mem�ria ocupada pela aresta
            free(atual);
            printf("\Conexao removida com sucesso.\n");
            getchar();
            // Atualiza o ficheiro
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
    // Abre o ficheiro em modo escrita
    FILE* arquivo = fopen("arestas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        return;
    }

    // Escreve as arestas da lista no ficheiro
    aresta* atual = a;
    while (atual != NULL) {
        fprintf(arquivo, "%d %d %d\n", atual->id_origem, atual->id_destino, atual->peso);
        atual = atual->proxima;
    }

    // Fecha o ficheiro
    fclose(arquivo);
}

void verConexoesRaio(grafo* g, aresta* a, meio* m, int caller) {
    g = carregarGrafo();
    a = carregarAresta();
    imprimirVertices(g);

    int id_origem;
    float raio;
    int tipo_opcao;
    char tipo_meio[50];

    printf("\nDigite o ID onde se encontra: ");
    scanf("%d", &id_origem);

    printf("\nDigite o tipo de meio:\n\n");
    printf("1. Bicicleta\n");
    printf("2. Trotinete\n\n");
    scanf("%d", &tipo_opcao);

    if (tipo_opcao == 1) {
        strcpy(tipo_meio, "Bicicleta");
    }
    else if (tipo_opcao == 2) {
        strcpy(tipo_meio, "Trotinete");
    }
  
    printf("\nDigite o raio de distancia: ");
    scanf("%f", &raio);

    printf("\nConexoes dentro do raio de %.2f a partir do ID %d:\n\n", raio, id_origem);

    if (caller == 0) {
        encontrarConexoes(g, a, m, id_origem, raio, tipo_meio, 0);
    }
    else if (caller == 1) {
        encontrarConexoes(g, a, m, id_origem, raio, tipo_meio, 1);
    }

    getchar();
}

void encontrarConexoes(grafo* g, aresta* a, meio* m, int id_origem, float raio, const char* tipo_meio, int caller) {
    bool* visitados = calloc(g->num_vertices, sizeof(bool));
    float* distancias = malloc(g->num_vertices * sizeof(float));
    int* antecessores = malloc(g->num_vertices * sizeof(int));
    char** nomesVertices = malloc(g->num_vertices * sizeof(char*));
    int numVertices = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        distancias[i] = FLT_MAX;
        antecessores[i] = -1;
    }

    distancias[id_origem] = 0.0;

    for (int count = 0; count < g->num_vertices - 1; count++) {
        int verticeAtual = -1;
        float menorDistancia = FLT_MAX;

        for (int v = 0; v < g->num_vertices; v++) {
            if (!visitados[v] && distancias[v] < menorDistancia) {
                verticeAtual = v;
                menorDistancia = distancias[v];
            }
        }

        if (verticeAtual == -1) {
            break;
        }

        visitados[verticeAtual] = true;

        for (aresta* arestaAtual = a; arestaAtual != NULL; arestaAtual = arestaAtual->proxima) {
            if (arestaAtual->id_origem == verticeAtual) {
                int idVizinho = arestaAtual->id_destino;
                float distanciaAcumulada = distancias[verticeAtual] + arestaAtual->peso;

                if (distanciaAcumulada <= raio && distanciaAcumulada < distancias[idVizinho]) {
                    distancias[idVizinho] = distanciaAcumulada;
                    antecessores[idVizinho] = verticeAtual;
                }
            }
            else if (arestaAtual->id_destino == verticeAtual) {
                int idVizinho = arestaAtual->id_origem;
                float distanciaAcumulada = distancias[verticeAtual] + arestaAtual->peso;

                if (distanciaAcumulada <= raio && distanciaAcumulada < distancias[idVizinho]) {
                    distancias[idVizinho] = distanciaAcumulada;
                    antecessores[idVizinho] = verticeAtual;
                }
            }
        }
    }

    // Armazenar o nome do v�rtice de origem
    nomesVertices[numVertices] = obterNomeVertice(g, id_origem);
    numVertices++;

    printf("+----------+---------+-----------+-------------------+\n");
    printf("| Origem   | Destino | Distancia |       Caminho     |\n");
    printf("+----------+---------+-----------+-------------------+\n");

    for (int i = 0; i < g->num_vertices; i++) {
        if (i != id_origem && distancias[i] <= raio) {
            int destino = i;

            // Reconstruir o caminho percorrido
            int* caminho = malloc(g->num_vertices * sizeof(int));
            int index = 0;
            int atual = destino;

            while (atual != -1) {
                caminho[index++] = atual;
                atual = antecessores[atual];
            }

            // Imprimir a conex�o
            imprimirConexao(id_origem, destino, distancias[destino], caminho, index);

            // Armazenar o nome do v�rtice vizinho
            nomesVertices[numVertices] = obterNomeVertice(g, i);
            numVertices++;
        }
    }

    if (caller == 0) {
        compararNomesVertices(g, m, nomesVertices, numVertices, tipo_meio);
    }
    else if (caller == 1) {
        compararNomesVerticesCliente(g, m, nomesVertices, numVertices, tipo_meio);
    }
}

void imprimirConexao(int id_origem, int destino, float distancia, int* caminho, int tamanho) {

    printf("| %8d | %7d | %9.2f | ", id_origem, destino, distancia);
    
    for (int i = tamanho - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }
        printf("\n");
        printf("+----------+---------+-----------+-------------------+\n");
    }






































































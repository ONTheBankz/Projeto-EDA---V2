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

aresta* criarConexao() {
    aresta* listaArestas = NULL;
    int conexoes[12][3] = {
        {0, 1, 13},
        {0, 2, 15},
        {1, 2, 17},
        {1, 4, 23},
        {2, 3, 19},
        {2, 6, 28},
        {3, 4, 13},
        {3, 5, 15},
        {4, 5, 17},
        {4, 7, 28},
        {5, 6, 19},
        {6, 7, 15}
    };
        for (int i = 0; i < 12; i++) {
            int id_origem = conexoes[i][0];
            int id_destino = conexoes[i][1];
            int peso = conexoes[i][2];

            aresta* novaAresta = (aresta*)malloc(sizeof(aresta));
            novaAresta->id_origem = id_origem;
            novaAresta->id_destino = id_destino;
            novaAresta->peso = peso;
            novaAresta->proxima = NULL;

        // Adiciona a nova aresta à lista de arestas
        if (listaArestas == NULL) {
            listaArestas = novaAresta;
        }
        else {
            aresta* atual = listaArestas;
            while (atual->proxima != NULL) {
                atual = atual->proxima;
            }
            atual->proxima = novaAresta;
        }
    }

    return listaArestas;
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
    // Lê os valores do ficheiro e cria as arestas
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
    // Adiciona a aresta à lista ligada
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
    return NULL; // Retorna NULL se o vértice não for encontrado
}

void salvarGrafo(grafo* g) {
    FILE* arquivo = fopen("grafo.txt", "w");
    FILE* arquivo_bin = fopen("grafo.bin", "wb");

    if (arquivo == NULL || arquivo_bin == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    vertice* atual = g->vertices;
    while (atual != NULL) {
        fprintf(arquivo, "%d %s %s\n", atual->id, atual->local_meio, atual->nome);
        fwrite(atual, sizeof(vertice), 1, arquivo_bin);
        atual = atual->seguinte;
    }
        fclose(arquivo);
    fclose(arquivo_bin);
}

void criarVertice(grafo* g) {  
    char nome[50];
    char local_meio[50];
    g = carregarGrafo();
    
    // Solicitar o nome do vértice ao usuário
    printf("Digite o geocodigo do vertice: ");
    scanf("%s", nome);

    printf("\nDigite o local do vertice: ");
    scanf("%s", local_meio);

    // Criar um novo vértice
    vertice* novoVertice = (vertice*)malloc(sizeof(vertice));
    novoVertice->id = g->num_vertices;
    strcpy(novoVertice->nome, nome);
    strcpy(novoVertice->local_meio, local_meio);

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
    // Escrever os vértices atualizados no arquivo
    atualizarVertices(g->vertices);

    system("clear || cls");
    printf("\nNovo vertice criado com sucesso!\n");
    getchar();
}

void editarVertice(grafo* g) {
    g = carregarGrafo();
    
    int id;
    char novoNome[50];
    char novoLocal[50];

    imprimirVertices(g);

    // Solicitar o ID do vértice a ser editado
    printf("\nDigite o ID do vertice a ser editado: ");
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

    // Solicitar o novo geocodigo do vértice ao usuário
    printf("\nDigite o novo geocodigo do vertice: ");
    scanf("%s", novoNome);

    // Atualizar o geocodigo do vértice
    strcpy(atual->nome, novoNome);

    // Solicitar o novo local do vértice ao usuário
    printf("\nDigite o novo local do vertice: ");
    scanf("%s", novoLocal);

    // Atualizar o local do vértice
    strcpy(atual->local_meio, novoLocal);

    // Escrever os vértices atualizados no arquivo
    atualizarVertices(g->vertices);

    system("clear || cls");
    printf("\nVertice editado com sucesso!\n");
    getchar();
}

void removerVertice(grafo* g) {
    g = carregarGrafo();
    imprimirVertices(g);
    int id;

    // Solicitar o ID do vértice a ser removido
    printf("\nDigite o ID do vertice a ser removido: ");
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

    // Escrever os vértices atualizados no arquivo
    atualizarVertices(g->vertices);

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

void atualizarVertices(vertice* vertices) {
    FILE* arquivo = fopen("grafo.txt", "w");
    FILE* arquivo_bin = fopen("grafo.bin", "wb");

    if (arquivo == NULL || arquivo_bin == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
        vertice* novoVertice = vertices;
        while (novoVertice != NULL) {
            fprintf(arquivo, "%d %s %s\n", novoVertice->id, novoVertice->local_meio, novoVertice->nome);
            fwrite(novoVertice, sizeof(vertice), 1, arquivo_bin);
            novoVertice = novoVertice->seguinte;
    }
        fclose(arquivo);
        fclose(arquivo_bin);
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

void mostrarMeios(grafo* g, meio* m, char** nomesVertices, int numVertices, const char* tipo_meio) {
    // Abrir arquivo em modo de leitura
    FILE* txt_file = fopen("meios.txt", "r");

    // Ler todos os meios para uma lista ligada
    meio* head = NULL;
    meio* curr = NULL;
    lerMeios(txt_file, &head);
    fclose(txt_file);

    printf("\nMeios disponiveis nos locais:\n\n");
    printf("ID | Nome      | Tipo      | Custo  | Bateria  | Local             | Reserva\n");

    // Verificar se o tipo_meio é "Bicicleta" ou "Trotinete"
    if (strcmp(tipo_meio, "Bicicleta") != 0 && strcmp(tipo_meio, "Trotinete") != 0) {
        printf("Tipo de meio inválido! Deve ser 'Bicicleta' ou 'Trotinete'.\n");
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
                // Meio encontrado no local vizinho e do tipo desejado, imprimir informações
                printf("---+-----------+-----------+--------+----------+-------------------+---------+\n");
                printf("%-3d| %-10s| %-10s| %-7.2f| %-9.2f| %-18s| %-6d\n",
                    curr->id, curr->nome, curr->tipo, curr->custo, curr->bateria, curr->local, curr->reserva);
            }
                curr = curr->seguinte;  // Move to the next meio
        }
    }
}

void criarAresta(grafo* g) {
    int id_origem, id_destino, peso;
    g = carregarGrafo();
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

    // Abrir o arquivo em modo de append
    FILE* arquivo = fopen("arestas.txt", "a");
    FILE* arquivo_bin = fopen("arestas.bin", "ab");

    if (arquivo == NULL || arquivo_bin == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        free(nova_aresta);
        return;
    }

    // Escrever a aresta no arquivo
    fprintf(arquivo, "%d %d %d\n", nova_aresta->id_origem, nova_aresta->id_destino, nova_aresta->peso);
    fwrite(nova_aresta, sizeof(aresta), 1, arquivo_bin);

    // Fechar o arquivo
    fclose(arquivo);
    fclose(arquivo_bin);

    printf("\nConexao das localizacoes criada com sucesso!\n");
    getchar();
}

void salvarAresta(aresta* a) {
    // Salvar em arquivo de texto (.txt)
    FILE* arquivoTxt = fopen("arestas.txt", "w");
    FILE* arquivoBin = fopen("arestas.bin", "wb");
    if (arquivoTxt == NULL || arquivoBin == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    aresta* atualTxt = a;
    while (atualTxt != NULL) {
        fprintf(arquivoTxt, "%d %d %d\n", atualTxt->id_origem, atualTxt->id_destino, atualTxt->peso);
        atualTxt = atualTxt->proxima;
    }

    aresta* atualBin = a;
    while (atualBin != NULL) {
        fwrite(&atualBin->id_origem, sizeof(int), 1, arquivoBin);
        fwrite(&atualBin->id_destino, sizeof(int), 1, arquivoBin);
        fwrite(&atualBin->peso, sizeof(int), 1, arquivoBin);
        atualBin = atualBin->proxima;
    }
    fclose(arquivoTxt);
    fclose(arquivoBin);
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
    printf("\nDigite o ID da localizacao de origem: ");
    scanf("%d", &id_origem);

    printf("\nDigite o ID da localizacao de destino: ");
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

            // Liberta a memória ocupada pela aresta
            free(atual);
            printf("\nConexao removida com sucesso.\n");
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
    // Abrir o arquivo em modo de escrita
    FILE* arquivo = fopen("arestas.txt", "w");
    FILE* arquivo_bin = fopen("arestas.bin", "wb");

    if (arquivo == NULL || arquivo_bin == NULL) {
        printf("Erro: Falha ao abrir o arquivo.\n");
        return;
    }

    // Escreve as arestas da lista no ficheiro
    aresta* atual = a;
    while (atual != NULL) {
        fprintf(arquivo, "%d %d %d\n", atual->id_origem, atual->id_destino, atual->peso);
        fwrite(atual, sizeof(aresta), 1, arquivo_bin);
        atual = atual->proxima;
    }

    // Fecha o ficheiro
    fclose(arquivo);
    fclose(arquivo_bin);
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
            int idVizinho = -1;

            if (arestaAtual->id_origem == verticeAtual) {
                idVizinho = arestaAtual->id_destino;
            }
            else if (arestaAtual->id_destino == verticeAtual) {
                idVizinho = arestaAtual->id_origem;
            }

            if (idVizinho != -1) {
                float distanciaAcumulada = distancias[verticeAtual] + arestaAtual->peso;

                if (distanciaAcumulada <= raio && distanciaAcumulada < distancias[idVizinho]) {
                    distancias[idVizinho] = distanciaAcumulada;
                    antecessores[idVizinho] = verticeAtual;
                }
            }
        }
    }

    // Armazenar o nome do vértice de origem
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

            // Imprimir a conexão
            imprimirConexao(id_origem, destino, distancias[destino], caminho, index);

            // Armazenar o nome do vértice vizinho
            nomesVertices[numVertices] = obterNomeVertice(g, i);
            numVertices++;
        }
    }

    if (caller == 0) {
        mostrarMeios(g, m, nomesVertices, numVertices, tipo_meio);
    }
    else if (caller == 1) {
        mostrarMeiosCliente(g, m, nomesVertices, numVertices, tipo_meio);
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






































































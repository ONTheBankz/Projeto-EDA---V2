#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Função para registar um meio
void registarMeio(meio** head) {
    grafo* lista_vertices = NULL;
    grafo* g = carregarGrafo("grafo.txt");
    lista_vertices = criarListaVertices(g);
    // Alocar memória para um novo meio
    meio* novo_meio = (meio*)malloc(sizeof(meio));

    // Abrir o ficheiro de meios para leitura
    FILE* f = fopen("meios.txt", "a+");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro meios.txt\n");
        return;
    }
    int id = 0;
    int id_vert;
    int reserva;
    char tipo[50], distancia[50], local[50], local_grafo[50];
    float custo, bateria;

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    while (fscanf(f, "%d %s %f %f %s %s %d\n", &id, tipo, &custo, &bateria, local, 
        local_grafo, &reserva) != EOF) {

    }

    id++;  // Incrementar o último ID encontrado
    fclose(f); // Fechar o ficheiro

    // Preencher os campos do novo meio
    novo_meio->id = id;
    novo_meio->reserva = 0;
    printf("Digite o tipo do meio: ");
    scanf("%s", novo_meio->tipo);

    printf("Digite o custo do meio: ");
    scanf("%f", &novo_meio->custo);

    printf("Digite a bateria do meio: ");
    scanf("%f", &novo_meio->bateria);

    printf("Digite o local do meio: ");
    scanf("%s", novo_meio->local);

    // Print the list of vertices and ask for input
    printVertice(lista_vertices);
    printf("\nDigite o ID correspondente ao vertice escolhido: ");
    scanf("%d", &id_vert);

    // Encontra o vértice com o ID escolhido e copia o respetivo nome
    vertice_node* current = lista_vertices;
    while (current != NULL) {
        if (current->id == id_vert) {
            strcpy(novo_meio->local_grafo, current->nome);
            break;
        }
        current = current->proximo;
    }

    // Colocar o próximo pointer no topo da lista
    novo_meio->seguinte = *head;
    *head = novo_meio;

    // Escrever os valores do gestor no ficheiro de texto
        f = fopen("meios.txt", "a");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    fprintf(f, "%d %s %.2f %.2f %s %s %d\n", novo_meio->id, novo_meio->tipo, novo_meio->custo,
        novo_meio->bateria, novo_meio->local, novo_meio->local_grafo, novo_meio->reserva);

    fclose(f);

    // Escrever os valores do gestor no ficheiro binário
    FILE* bin_meio = fopen("meios.bin", "ab");
    if (bin_meio == NULL) {
        printf("Erro ao abrir arquivo binário!\n");
        exit(1);
    }

    fwrite(novo_meio, sizeof(meio), 1, bin_meio);
    fclose(bin_meio);

    system("clear || cls");
    printf("Meio registrado com sucesso!\n");
    getchar();
}

// Função para passar os meios para uma lista ligada
void lerMeios(FILE* f, meio** head) {
    meio* current = NULL;
    while (!feof(f)) {
        meio* new_meio = (meio*)malloc(sizeof(meio));
        fscanf(f, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), 
                &(new_meio->bateria), new_meio->local, new_meio->local_grafo, 
                &(new_meio->reserva));
        new_meio->seguinte = NULL;
        if (*head == NULL) {
            *head = new_meio;
            current = new_meio;
        }
        else {
            current->seguinte = new_meio;
            current = new_meio;
        }
    }
}

// Função para atualizar os meios no ficheiro
void atualizarMeio(FILE** f, meio* head_meio) {
    // Abre o ficheiro para atualizar os valores
    *f = fopen("meios.txt", "wb");
    meio* curr_meio = head_meio;

    // Escreve os conteúdos de cada categoria
    while (curr_meio != NULL) {
        fprintf(*f, "%d %s %.2f %.2f %s %s %d\n", curr_meio->id, curr_meio->tipo, curr_meio->custo,
            curr_meio->bateria, curr_meio->local, curr_meio->local_grafo, curr_meio->reserva);
        curr_meio = curr_meio->seguinte;
    }

    fclose(*f);
}

// Função para atualizar os meios no binário
void atualizarBinMeio(FILE** f, meio* head_meio) {
    f = fopen("meios.bin", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    meio* curr_meio = head_meio;
    while (curr_meio != NULL) {
        fwrite(curr_meio, sizeof(meio), 1, f);
        curr_meio = curr_meio->seguinte;
    }

    fclose(f);
}

// Função para listar um meio
void listarMeio(char order_by) {
    // Abrir ficheiro em modo leitura
    FILE* txt_meio = fopen("meios.txt", "r");
    if (txt_meio == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }
    // Criar array de meios
    meio m[100];
    int count = 0;
    while (fscanf(txt_meio, "%d %s %f %f %s %s %d\n", &m[count].id, m[count].tipo, &m[count].custo, &m[count].bateria,
        m[count].local, m[count].local_grafo, &m[count].reserva) != EOF) {
        count++;
    }

    // Lista de ordenação
    if (order_by == 'b') {
        // Ordenar por bateria
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (m[j].bateria < m[j + 1].bateria) {
                    meio temp = m[j];
                    m[j] = m[j + 1];
                    m[j + 1] = temp;
                }
            }
        }
    }
    
    // Mostrar lista de meios
    printf("Lista de meios:\n\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nLocal: %s\nGeocodigo: %s\nReserva: %d\n\n", 
            m[i].id, m[i].tipo, m[i].custo, m[i].bateria, m[i].local, m[i].local_grafo, m[i].reserva);
    }

    fclose(txt_meio);
    getchar();
}

// Função para remover um meio
void removerMeio() {
        int id;

        // Abrir ficheiros em modo de escrita e leitura
        FILE* txt_file = fopen("meios.txt", "r");
        FILE* bin_file = fopen("meios.bin", "ab+");
        if (txt_file == NULL || bin_file == NULL) {
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

        // Escrever lista de meios
        printf("Lista de meios:\n\n");
        curr = head;
        while (curr != NULL) {
        printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nLocal: %s\nGeocodigo: %s\nReserva: %d\n\n", 
                curr->id, curr->tipo, curr->custo, curr->bateria, curr->local, curr->local_grafo, 
                curr->reserva);
                curr = curr->seguinte;
        }

        // Pedir ao user o ID do meio a remover
        printf("Digite o ID do meio a remover: ");
        scanf("%d", &id);

        // Procurar o meio com o ID escolhido
        curr = head;
        meio* prev = NULL;
        while (curr != NULL && curr->id != id) {
            prev = curr;
            curr = curr->seguinte;
        }

        if (curr == NULL) {
            system("clear || cls");
            printf("Meio com ID %d nao encontrado!\n", id);
            getchar();
        }
        else {
            // Remover o meio da lista ligada
            if (prev == NULL) {
                head = curr->seguinte;
            }
            else {
                prev->seguinte = curr->seguinte;
            }
            free(curr);

            // atualizar os meios no ficheiro
            atualizarMeio(&txt_file, head);

            // atualizar os meios no binário
            atualizarBinMeio(&txt_file, head);

            system("clear || cls");
            printf("Meio com ID %d removido com sucesso!\n", id);
            getchar();
        }
    }

// Função para alterar um meio
void alterarMeio() {
    int id;

    // Abrir ficheiro em modo leitura
    FILE* file = fopen("meios.txt", "r");
    FILE* bin_file = fopen("meios.bin", "ab+");
    if (file == NULL || bin_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os meios para uma lista ligada
    meio* head = NULL;
    meio* curr = NULL;
    lerMeios(file, &head);
    fclose(file);

    // Escrever lista de meios
    printf("Lista de meios:\n\n");
    curr = head;
    while (curr != NULL) {
    printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nLocal: %s\nGeocodigo: %s\nReserva: %d\n\n", 
            curr->id, curr->tipo, curr->custo, curr->bateria, curr->local, curr->local_grafo, 
            curr->reserva);
            curr = curr->seguinte;
    }

    // Pedir ao user o ID do meio a alterar
    printf("Digite o ID do meio a alterar: ");
    scanf("%d", &id);

    // Procurar o meio com o ID escolhido
    curr = head;
    meio* prev = NULL;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("Meio com ID %d nao encontrado!\n", id);
        getchar();
    }
    else {
        // Alterar o meio com o ID escolhido
        char campo[20], novo_valor[50];
        do {
            printf("Digite o campo a alterar (tipo, custo, bateria, local, geocodigo ou reserva), ou 'fim' para terminar: ");
            scanf("%s", campo);
            if (strcmp(campo, "fim") == 0) {
                break;
            }
            printf("Digite o novo valor: ");
            scanf("%s", novo_valor);
            if (strcmp(campo, "tipo") == 0) {
                strcpy(curr->tipo, novo_valor);
            }
            else if (strcmp(campo, "custo") == 0) {
                curr->custo = atof(novo_valor);
            }
            else if (strcmp(campo, "bateria") == 0) {
                curr->bateria = atof(novo_valor);
            }           
            else if (strcmp(campo, "local") == 0) {
                strcpy(curr->local, novo_valor);
            }
            else if (strcmp(campo, "geocodigo") == 0) {
                strcpy(curr->local_grafo, novo_valor);
            }
            else if (strcmp(campo, "reserva") == 0) {
                curr->reserva = atoi(novo_valor);
            }

        } while (strcmp(campo, "fim") != 0);

        // atualizar os meios no ficheiro
        atualizarMeio(&file, head);

        // atualizar os meios no ficheiro
        atualizarBinMeio(&file, head);

        system("clear || cls");
        printf("Meio com ID %d alterado com sucesso!\n", id);
        getchar();

        // Libertar a memória alocada para a lista ligada
        curr = head;
        while (curr != NULL) {
            meio* temp = curr;
            curr = curr->seguinte;
            free(temp);
        }
    }
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Fun��o para registar um meio
void registarMeio(meio** head) {
    // Alocar mem�ria para um novo meio
    meio* novo_meio = (meio*)malloc(sizeof(meio));

    // Abrir o ficheiro de meios para leitura
    FILE* f = fopen("meios.txt", "a+");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro meios.txt\n");
        return;
    }
    int id = 0;
    int reserva;
    char tipo[50], distancia[50], local[50];
    float custo, bateria;

    // Encontrar o �ltimo ID presente no ficheiro e increment�-lo
    while (fscanf(f, "%d %s %f %f %s %s %d\n", &id, tipo, &custo, &bateria, distancia, local, &reserva) != EOF) {

    }
    id++;  // Incrementar o �ltimo ID encontrado
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

    printf("Digite a distancia do meio: ");
    scanf("%s", novo_meio->distancia);

    printf("Digite o local do meio: ");
    scanf("%s", novo_meio->local);

    // Colocar o pr�ximo pointer no topo da lista
    novo_meio->seguinte = *head;
    *head = novo_meio;

    // Escrever os valores do gestor no ficheiro de texto
    FILE* file = fopen("meios.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    fprintf(file, "%d %s %.2f %.2f %s %s %d\n", novo_meio->id, novo_meio->tipo, novo_meio->custo,
        novo_meio->bateria, novo_meio->distancia, novo_meio->local, novo_meio->reserva);

    fclose(file);

    // Escrever os valores do gestor no ficheiro bin�rio
    FILE* binary_file = fopen("meios.bin", "ab");
    if (binary_file == NULL) {
        printf("Erro ao abrir arquivo bin�rio!\n");
        exit(1);
    }

    fwrite(novo_meio, sizeof(meio), 1, binary_file);

    fclose(binary_file);

    system("clear || cls");
    printf("Meio registrado com sucesso!\n");
    getchar();
}

// Fun��o para listar um meio
void listar_meio(char order_by) {
    // Open file in read mode
    FILE* file = fopen("meios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    // Create array of meios
    meio m[100];
    int count = 0;
    while (fscanf(file, "%d %s %f %f %s %s %d\n", &m[count].id, m[count].tipo, &m[count].custo, &m[count].bateria,
        m[count].distancia, m[count].local, &m[count].reserva) != EOF) {
        count++;
    }

    // Order list
    if (order_by == 'b') {
        // Order list by bateria
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
    else if (order_by == 'd') {
        // Order list by distancia
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                float dist1 = atof(m[j].distancia);
                float dist2 = atof(m[j + 1].distancia);
                if (dist1 > dist2) {
                    meio temp = m[j];
                    m[j] = m[j + 1];
                    m[j + 1] = temp;
                }
            }
        }
    }

    // Print list of meios
    printf("Lista de meios:\n\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nDistancia: %s\nLocal: %s\n\n", m[i].id, m[i].tipo,
            m[i].custo, m[i].bateria, m[i].distancia, m[i].local);
    }

    fclose(file);
    getchar();
}

// Fun��o para remover um meio
void remover_meio() {

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
        while (!feof(txt_file)) {
            meio* new_meio = (meio*)malloc(sizeof(meio));
            fscanf(txt_file, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), 
                &(new_meio->bateria), new_meio->distancia, new_meio->local, &(new_meio->reserva));
                new_meio->seguinte = NULL;

            if (head == NULL) {
                head = new_meio;
            }
            else {
                curr->seguinte = new_meio;
            }
            curr = new_meio;
        }

        fclose(txt_file);

        // Escrever lista de meios
        printf("Lista de meios:\n\n");
        curr = head;
        while (curr != NULL) {
            printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nDistancia: %s\nLocal: %s\nReserva: %d\n\n", curr->id,
                curr->tipo, curr->custo, curr->bateria, curr->distancia, curr->local, curr->reserva);
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

            // Escrever a lista atualizada de meios de volta para o arquivo de texto
            txt_file = fopen("meios.txt", "w");
            if (txt_file == NULL) {
                system("clear || cls");
                printf("Erro ao abrir arquivo!\n");
                getchar();
                exit(1);
            }

            curr = head;
            while (curr != NULL) {
                fprintf(txt_file, "%d %s %.2f %.2f %s %s %d\n", curr->id, curr->tipo, curr->custo, curr->bateria, 
                    curr->distancia, curr->local, curr->reserva);
                    curr = curr->seguinte;
            }

            fclose(txt_file);

            // Escrever a lista atualizada de meios de volta para o arquivo bin�rio
            bin_file = fopen("meios.bin", "wb");
            if (bin_file == NULL) {
                system("clear || cls");
                printf("Erro ao abrir arquivo!\n");
                getchar();
                exit(1);
            }
            curr = head;
            while (curr != NULL) {
                fwrite(curr, sizeof(meio), 1, bin_file);
                curr = curr->seguinte;
            }

            fclose(bin_file);
            system("clear || cls");
            printf("Meio com ID %d removido com sucesso!\n", id);
            getchar();
        }
    }

// Fun��o para alterar um meio
void alterar_meio() {
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
    while (!feof(file)) {

        meio* new_meio = (meio*)malloc(sizeof(meio));
        fscanf(file, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), &(new_meio->bateria), 
            new_meio->distancia, new_meio->local, &(new_meio->reserva));
            new_meio->seguinte = NULL;

        if (head == NULL) {
            head = new_meio;
        }
        else {
            curr->seguinte = new_meio;
        }
        curr = new_meio;
    }

    fclose(file);

    // Escrever lista de meios
    printf("Lista de meios:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nDistancia: %s\nLocal: %s\nReserva: %d\n\n", curr->id,
            curr->tipo, curr->custo, curr->bateria, curr->distancia, curr->local, curr->reserva);
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
            printf("Digite o campo a alterar (tipo, custo, bateria, distancia, local, reserva), ou 'fim' para terminar: ");
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
            else if (strcmp(campo, "distancia") == 0) {
                strcpy(curr->distancia, novo_valor);
            }
            else if (strcmp(campo, "local") == 0) {
                strcpy(curr->local, novo_valor);
            }
            else if (strcmp(campo, "reserva") == 0) {
                curr->reserva = atoi(novo_valor);
            }

        } while (strcmp(campo, "fim") != 0);

        // Escrever a lista atualizada de meios no arquivo
        file = fopen("meios.txt", "w");
        if (file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fprintf(file, "%d %s %.2f %.2f %s %s %d\n", curr->id, curr->tipo, curr->custo, curr->bateria, curr->distancia,
                curr->local, curr->reserva);
                curr = curr->seguinte;
        }

        fclose(file);

        // Escrever a lista atualizada de meios de volta para o arquivo bin�rio
        bin_file = fopen("meios.bin", "wb");
        if (bin_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }
        curr = head;
        while (curr != NULL) {
            fwrite(curr, sizeof(meio), 1, bin_file);
            curr = curr->seguinte;
        }

        fclose(bin_file);

        system("clear || cls");
        printf("Meio com ID %d alterado com sucesso!\n", id);
        getchar();

        // Libertar a mem�ria alocada para a lista ligada
        curr = head;
        while (curr != NULL) {
            meio* temp = curr;
            curr = curr->seguinte;
            free(temp);
        }
    }
}
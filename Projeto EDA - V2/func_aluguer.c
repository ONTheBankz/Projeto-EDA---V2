#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

// Função para alugar um meio
void registar_aluguer(int id_cliente, registo** headR) {
    FILE* txt_meios, * bin_meios, * txt_clientes, * bin_clientes, * txt_registos, * bin_registos, * txt_historico, * bin_historico;
    registo r;
    cliente c;
    meio m;
    int id_registo = 0;
    int id, meio_id, cliente_id, registo_id;
    int clientes, meios;
    int custo;
    char data_temp[50];
    char data[50];

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(data, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

    // Abrir ficheiros em modo de escrita e leitura
    txt_meios = fopen("meios.txt", "r");
    bin_meios = fopen("meios.bin", "ab+");
    if (txt_meios == NULL || bin_meios == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    while (!feof(txt_meios)) {
        meio* new_meio = (meio*)malloc(sizeof(meio));
        fscanf(txt_meios, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), &(new_meio->bateria),
            new_meio->distancia, new_meio->local, &(new_meio->reserva));
        new_meio->seguinte = NULL;
    
            if (head_meio == NULL) {
                head_meio = new_meio;
                curr_meio = new_meio;
            }
            else {
                curr_meio->seguinte = new_meio;
                curr_meio = new_meio;
            }
        }

    fclose(txt_meios);

    // Escrever lista de meios
    printf("Lista de meios:\n\n");
    curr_meio = head_meio;
    while (curr_meio != NULL) {
        if (curr_meio->reserva != 1) {
            printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nDistancia: %s\nLocal: %s\n\n", curr_meio->id,
                curr_meio->tipo, curr_meio->custo, curr_meio->bateria, curr_meio->distancia, curr_meio->local);
        }
        curr_meio = curr_meio->seguinte;
    }

    // Pedir ao user o ID do meio que quer alugar
    printf("Introduza o ID do meio que pretende alugar: ");
    scanf("%d", &meio_id);

    // Procurar o meio com o ID escolhido
    curr_meio = head_meio;
    while (curr_meio != NULL && curr_meio->id != meio_id) {
        curr_meio = curr_meio->seguinte;
    }

    // Verificar se o meio foi encontrado
    if (curr_meio == NULL) {
        system("clear || cls");
        printf("Meio com ID %d nao encontrado!\n", meio_id);
        getchar();
        return;
    }

    // Adicionar 1 ao campo "reserva" do meio
    curr_meio->reserva = 1;
    custo = curr_meio->custo;

    // Escrever a lista atualizada de meios de volta para o arquivo de texto
    txt_meios = fopen("meios.txt", "w");
    if (txt_meios == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    curr_meio = head_meio;
    while (curr_meio != NULL) {
        fprintf(txt_meios, "%d %s %.2f %.2f %s %s %d\n", curr_meio->id, curr_meio->tipo, curr_meio->custo, curr_meio->bateria,
            curr_meio->distancia, curr_meio->local, curr_meio->reserva);
            curr_meio = curr_meio->seguinte;
    }

    fclose(txt_meios);

    // Escrever a lista atualizada de meios de volta para o arquivo binário
    bin_meios = fopen("meios.bin", "wb");
    if (bin_meios == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    curr_meio = head_meio;
    while (curr_meio != NULL) {
        fwrite(curr_meio, sizeof(meio), 1, bin_meios);
        curr_meio = curr_meio->seguinte;
    }

    fclose(bin_meios);

    // Abrir ficheiros em modo de escrita e leitura
    txt_clientes = fopen("clientes.txt", "r");
    bin_clientes = fopen("clientes.bin", "ab+");
    if (txt_clientes == NULL || bin_clientes == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    while (!feof(txt_clientes)) {
        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(txt_clientes, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome, &(new_cliente->nif),
            new_cliente->morada, &(new_cliente->saldo), new_cliente->utilizador, new_cliente->password);
        new_cliente->seguinte = NULL;

        if (head == NULL) {
            head = new_cliente;
        }
        else {
            curr->seguinte = new_cliente;
        }
        curr = new_cliente;
    }

    fclose(txt_clientes);

    // Procurar o cliente com o ID escolhido
    curr = head;
    while (curr != NULL && curr->id != id_cliente) {
        curr = curr->seguinte;
    }

    // Subtrair o custo do saldo do cliente
    curr->saldo -= custo;

    // Escrever a lista atualizada de clientes de volta para o arquivo de texto
    txt_clientes = fopen("clientes.txt", "w");
    if (txt_clientes == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    curr = head;
    while (curr != NULL) {
        fprintf(txt_clientes, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada,
            curr->saldo, curr->utilizador, curr->password);
            curr = curr->seguinte;
    }

    fclose(txt_clientes);

    // Escrever a lista atualizada de clientes de volta para o arquivo binário
    bin_clientes = fopen("clientes.bin", "wb");
    if (bin_clientes == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    curr = head;
    while (curr != NULL) {
        fwrite(curr, sizeof(cliente), 1, bin_clientes);
        curr = curr->seguinte;
    }

    fclose(bin_clientes);

    // Alocar memória para um novo registo
    registo* new_registo = (registo*)malloc(sizeof(registo));

    // Abrir o ficheiro de registos para leitura em modo append
        txt_registos = fopen("registos.txt", "a+");
    if (txt_registos == NULL) {
        printf("Erro ao abrir o ficheiro registos.txt\n");
        return;
    }
        bin_registos = fopen("registos.bin", "ab");
    if (bin_registos == NULL) {
        printf("Erro ao abrir o ficheiro registos.bin\n");
        return;
    }

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    while (fscanf(txt_registos, "%d %d %d %s\n", &id_registo, &clientes, &meios, data_temp) != EOF) {
    }
    id_registo++; // Incrementar o último ID encontrado
    fclose(txt_registos); // Fechar o ficheiro

    // Preencher os campos do novo cliente
    new_registo->id = id_registo;
    new_registo->cliente_id = id_cliente;
    new_registo->meio_id = meio_id;
    strcpy(new_registo->data, data);

    // Colocar o próximo pointer no topo da lista
    new_registo->seguinte = *headR;
    *headR = new_registo;

    // Escrever os valores do cliente no ficheiro de texto
    txt_registos = fopen("registos.txt", "a");
    fprintf(txt_registos, "%d %d %d %s\n", new_registo->id, new_registo->cliente_id, new_registo->meio_id,
        new_registo->data);
    fclose(txt_registos);

    // Escrever os valores do cliente no ficheiro binário
    fwrite(new_registo, sizeof(registo), 1, bin_registos);
    fclose(bin_registos);

    // Abrir o ficheiro de histórico para escrita em modo append
        txt_historico = fopen("historico.txt", "a");
    if (txt_historico == NULL) {
        printf("Erro ao abrir o ficheiro historico.txt\n");
        return;
    }

        bin_historico = fopen("historico.bin", "ab");
    if (bin_historico == NULL) {
        printf("Erro ao abrir o ficheiro historico.bin\n");
        return;
    }

    // Escrever os valores do cliente no ficheiro de texto
    fprintf(txt_historico, "%d %d %d %s\n", new_registo->id, new_registo->cliente_id, new_registo->meio_id,
        new_registo->data);
    fclose(txt_historico);

    // Escrever os valores do cliente no ficheiro binário
    fwrite(new_registo, sizeof(registo), 1, bin_historico);
    fclose(bin_historico);

    system("clear || cls");
    printf("Aluguer registado com sucesso!\n");
    getchar();
}

// Função para listar alugueres de um determinado cliente
void listar_aluguer() {
    FILE* txt_meios, * txt_clientes, * txt_registos;
    registo r;
    cliente c;
    meio m;
    int meio_id, cliente_id, registo_id;
    char data[50];

    // Abrir ficheiros em modo de escrita e leitura
    txt_meios = fopen("meios.txt", "r");

    if (txt_meios == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    while (!feof(txt_meios)) {
        meio* new_meio = (meio*)malloc(sizeof(meio));
        fscanf(txt_meios, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), &(new_meio->bateria),
            new_meio->distancia, new_meio->local, &(new_meio->reserva));
        new_meio->seguinte = NULL;

        if (head_meio == NULL) {
            head_meio = new_meio;
            curr_meio = new_meio;
        }
        else {
            curr_meio->seguinte = new_meio;
            curr_meio = new_meio;
        }
    }

    fclose(txt_meios);

    // Abrir ficheiros em modo de escrita e leitura
    txt_clientes = fopen("clientes.txt", "r");
    if (txt_clientes == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    while (!feof(txt_clientes)) {
        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(txt_clientes, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome, &(new_cliente->nif),
            new_cliente->morada, &(new_cliente->saldo), new_cliente->utilizador, new_cliente->password);
        new_cliente->seguinte = NULL;

        if (head == NULL) {
            head = new_cliente;
        }
        else {
            curr->seguinte = new_cliente;
        }
        curr = new_cliente;
    }

    fclose(txt_clientes);

    // Abrir ficheiros em modo de escrita e leitura
    txt_registos = fopen("registos.txt", "ab+");
    if (txt_registos == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os registos para uma lista ligada
    registo* head_registo = NULL;
    registo* curr_registo = NULL;
    while (!feof(txt_registos)) {
        registo* new_registo = (registo*)malloc(sizeof(registo));
        fscanf(txt_registos, "%d %d %d %s\n", &(new_registo->id), &(new_registo->cliente_id), &(new_registo->meio_id),
            new_registo->data);
        new_registo->seguinte = NULL;

        if (head_registo == NULL) {
            head_registo = new_registo;
        }
        else {
            curr_registo->seguinte = new_registo;
        }
        curr_registo = new_registo;
    }

    fclose(txt_registos);

    // Percorrer a lista de registos
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        // Procurar o nome do cliente na lista de clientes
        cliente_id = curr_registo->cliente_id;
        curr = head;
        while (curr != NULL) {
            if (curr->id == cliente_id) {
                break;
            }
            curr = curr->seguinte;
        }

        // Procurar o nome do meio na lista de meios
        meio_id = curr_registo->meio_id;
        curr_meio = head_meio;
        while (curr_meio != NULL) {
            if (curr_meio->id == meio_id) {
                break;
            }
            curr_meio = curr_meio->seguinte;
        }

        // Imprimir os dados do registo
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %s\n\n", curr_registo->id, curr->nome, curr_meio->tipo, curr_registo->data);
        curr_registo = curr_registo->seguinte;
    }

    // Libertar memória alocada para as listas ligadas
    curr = head;
    while (curr != NULL) {
        cliente* temp = curr;
        curr = curr->seguinte;
        free(temp);
    }

    curr_meio = head_meio;
    while (curr_meio != NULL) {
        meio* temp = curr_meio;
        curr_meio = curr_meio->seguinte;
        free(temp);
    }

    curr_registo = head_registo;
    while (curr_registo != NULL) {
        registo* temp = curr_registo;
        curr_registo = curr_registo->seguinte;
        free(temp);
    }
    getchar();
}

// Função para cancelar um aluguer de um determinado cliente
void cancelar_aluguer() {
    FILE* txt_meios, * txt_clientes, * txt_registos, * bin_meios, * bin_clientes, * bin_registos;
    registo r;
    cliente c;
    meio m;
    int meio_id, cliente_id, registo_id;
    char data[50];

    // Abrir ficheiros em modo de leitura
    txt_meios = fopen("meios.txt", "r");

    if (txt_meios == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    while (!feof(txt_meios)) {
        meio* new_meio = (meio*)malloc(sizeof(meio));
        fscanf(txt_meios, "%d %s %f %f %s %s %d\n", &(new_meio->id), new_meio->tipo, &(new_meio->custo), &(new_meio->bateria),
            new_meio->distancia, new_meio->local, &(new_meio->reserva));
        new_meio->seguinte = NULL;

        if (head_meio == NULL) {
            head_meio = new_meio;
            curr_meio = new_meio;
        }
        else {
            curr_meio->seguinte = new_meio;
            curr_meio = new_meio;
        }
    }

    fclose(txt_meios);

    // Abrir ficheiros em modo de leitura
    txt_clientes = fopen("clientes.txt", "r");
    if (txt_clientes == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    while (!feof(txt_clientes)) {
        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(txt_clientes, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome, &(new_cliente->nif),
            new_cliente->morada, &(new_cliente->saldo), new_cliente->utilizador, new_cliente->password);
        new_cliente->seguinte = NULL;

        if (head == NULL) {
            head = new_cliente;
        }
        else {
            curr->seguinte = new_cliente;
        }
        curr = new_cliente;
    }

    fclose(txt_clientes);

    // Abrir ficheiros em modo de escrita e leitura
    txt_registos = fopen("registos.txt", "ab+");
    if (txt_registos == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os registos para uma lista ligada
    registo* head_registo = NULL;
    registo* curr_registo = NULL;
    while (!feof(txt_registos)) {
        registo* new_registo = (registo*)malloc(sizeof(registo));
        fscanf(txt_registos, "%d %d %d %s\n", &(new_registo->id), &(new_registo->cliente_id), &(new_registo->meio_id),
            new_registo->data);
        new_registo->seguinte = NULL;

        if (head_registo == NULL) {
            head_registo = new_registo;
        }
        else {
            curr_registo->seguinte = new_registo;
        }
        curr_registo = new_registo;
    }

    fclose(txt_registos);

    // Percorrer a lista de registos
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        // Procurar o nome do cliente na lista de clientes
        cliente_id = curr_registo->cliente_id;
        curr = head;
        while (curr != NULL) {
            if (curr->id == cliente_id) {
                break;
            }
            curr = curr->seguinte;
        }

        // Procurar o nome do meio na lista de meios
        meio_id = curr_registo->meio_id;
        curr_meio = head_meio;
        while (curr_meio != NULL) {
            if (curr_meio->id == meio_id) {
                break;
            }
            curr_meio = curr_meio->seguinte;
        }

        // Imprimir os dados do registo
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %s\n\n", curr_registo->id, curr->nome, curr_meio->tipo, curr_registo->data);
        curr_registo = curr_registo->seguinte;
    }

    // Pedir ao utilizador o ID do registo a ser removido
    printf("Insira o ID do aluguer que deseja cancelar: ");
    scanf("%d", &registo_id);

    // Percorrer a lista de registos para encontrar o registo a ser removido
    curr_registo = head_registo;
    registo* prev_registo = NULL;
    while (curr_registo != NULL) {
        if (curr_registo->id == registo_id) {
            // Encontrar o meio com o ID associado ao registo
            meio_id = curr_registo->meio_id;
            cliente_id = curr_registo->cliente_id;
            curr_meio = head_meio;
            while (curr_meio != NULL) {
                if (curr_meio->id == meio_id) {
                    break;
                }
                curr_meio = curr_meio->seguinte;
            }

            // Encontrar o cliente com o ID associado ao registo
            curr = head;
            while (curr != NULL) {
                if (curr->id == cliente_id) {
                    // Incrementar o saldo do cliente pelo custo do meio
                    curr->saldo += curr_meio->custo;
                    break;
                }
                curr = curr->seguinte;
            }

            // Atualizar o campo "reserva" para 0
            curr_meio->reserva = 0;

            // Remover o registo da lista
            if (prev_registo == NULL) {
                head_registo = curr_registo->seguinte;
            }
            else {
                prev_registo->seguinte = curr_registo->seguinte;
            }

            // Atualizar o ficheiro de registos com a nova lista de registos
            txt_registos = freopen("registos.txt", "wb", txt_registos);
            curr_registo = head_registo;
            while (curr_registo != NULL) {
                fprintf(txt_registos, "%d %d %d %s\n", curr_registo->id, curr_registo->cliente_id,
                    curr_registo->meio_id, curr_registo->data);
                curr_registo = curr_registo->seguinte;
            }

            fclose(txt_registos);

            // Atualizar o ficheiro de meios com a nova lista de meios
            txt_meios = freopen("meios.txt", "wb", txt_meios);
            curr_meio = head_meio;
            while (curr_meio != NULL) {
                fprintf(txt_registos, "%d %s %.2f %.2f %s %s %d\n", curr_meio->id, curr_meio->tipo, curr_meio->custo,
                    curr_meio->bateria, curr_meio->distancia, curr_meio->local, curr_meio->reserva);
                curr_meio = curr_meio->seguinte;
            }

            fclose(txt_meios);

            // Atualizar o ficheiro de clientes com a nova lista de clientes
            txt_clientes = freopen("clientes.txt", "wb", txt_clientes);
            curr = head;
            while (curr != NULL) {
                fprintf(txt_clientes, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada,
                    curr->saldo, curr->utilizador, curr->password);
                curr = curr->seguinte;
            }

            fclose(txt_clientes);

            system("clear || cls");
            printf("Aluguer cancelado com sucesso!\n");
            getchar();
            return;
        }

        prev_registo = curr_registo;
        curr_registo = curr_registo->seguinte;
    }

    // Atualizar o ficheiro de registos com a nova lista de registos
    bin_registos = fopen("registos.bin", "wb");
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        fwrite(curr_registo, sizeof(registo), 1, bin_registos);
        curr_registo = curr_registo->seguinte;
    }

    fclose(bin_registos);

    // Atualizar o ficheiro de meios com a nova lista de meios
    bin_meios = fopen("meios.bin", "wb");
    curr_meio = head_meio;
    while (curr_meio != NULL) {
        fwrite(curr_meio, sizeof(meio), 1, bin_meios);
        curr_meio = curr_meio->seguinte;
    }

    fclose(bin_meios);

    // Atualizar o ficheiro de clientes com a nova lista de clientes
    bin_clientes = fopen("clientes.bin", "wb");
    curr = head;
    while (curr != NULL) {
        fwrite(curr, sizeof(cliente), 1, bin_clientes);
        curr = curr->seguinte;
    }

    fclose(bin_clientes);

    system("clear || cls");
    printf("Aluguer nao encontrado.\n");
    getchar();

    // Libertar memória alocada para as listas ligadas
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        registo* temp = curr_registo;
        curr_registo = curr_registo->seguinte;
        free(temp);
    }

    curr_meio = head_meio;
    while (curr_meio != NULL) {
        meio* temp = curr_meio;
        curr_meio = curr_meio->seguinte;
        free(temp);
    }

    curr = head;
    while (curr != NULL) {
        cliente* temp = curr;
        curr = curr->seguinte;
        free(temp);
    }
}



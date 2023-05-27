#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

// Função para alugar um meio
void registarAluguer(int id_cliente, registo** headR) {
    FILE* txt_meios, * bin_meios, * txt_clientes, * bin_clientes, * txt_registos, 
        * bin_registos, * txt_historico, * bin_historico;
    registo r;
    cliente c;
    meio m;
    int id_registo = 0;
    int id, meio_id, cliente_id, registo_id;
    int clientes, meios;
    int custo;
    int dia_temp, mes_temp, ano_temp, hora_temp, min_temp;

    // Recebe o tempo atual
    time_t current_time;
    struct tm* local_time;
    char datetime[20];
    char hourtime[20];
    current_time = time(NULL);
    local_time = localtime(&current_time);

    int dia = local_time->tm_mday;
    int mes = local_time->tm_mon + 1;   
    int ano = local_time->tm_year + 1900;  
    int hora = local_time->tm_hour;
    int minuto = local_time->tm_min;

    // Abrir ficheiros em modo de escrita e leitura
    txt_meios = fopen("meios.txt", "r");
    bin_meios = fopen("meios.bin", "ab+");

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    lerMeios(txt_meios, &head_meio);
    fclose(txt_meios);

    // Mostra a lista de meios
    printf("ID | Nome      | Tipo      | Custo  | Bateria  | Local              \n");
    printf("---+-----------+-----------+--------+----------+-------------------+\n");
    curr_meio = head_meio;
    while (curr_meio != NULL) {
        if (curr_meio->reserva != 1) {
            printf("%-3d| %-10s| %-10s| %-7.2f| %-9.2f| %-18s\n",
                curr_meio->id, curr_meio->nome, curr_meio->tipo, curr_meio->custo, curr_meio->bateria,
                curr_meio->local, curr_meio->local_grafo, curr_meio->reserva);
        }
        curr_meio = curr_meio->seguinte;
    }

    // Pedir ao user o ID do meio que quer alugar
    printf("\nIntroduza o ID do meio que pretende alugar: ");
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
  
    // atualizar os meios no ficheiro
    atualizarMeio(&txt_meios, head_meio);

    // atualizar os meios no binário
    atualizarBinMeio(&bin_meios, head_meio);

    // Abrir ficheiros em modo de escrita e leitura
    txt_clientes = fopen("clientes.txt", "r");
    bin_clientes = fopen("clientes.bin", "ab+");

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    lerClientes(txt_clientes, &head);
    fclose(txt_clientes);

    // Procurar o cliente com o ID de login
    curr = head;
    while (curr != NULL && curr->id != id_cliente) {
        curr = curr->seguinte;
    }

    // atualizar os clientes no ficheiro
   atualizarCliente(&txt_clientes, head);

   // atualizar os clientes no binário
   atualizarBinCliente(&bin_clientes, head);

    // Alocar memória para um novo registo
    registo* new_registo = (registo*)malloc(sizeof(registo));

    // Abrir ficheiros em modo de escrita e leitura
    txt_registos = fopen("registos.txt", "r");
    bin_registos = fopen("registos.bin", "ab+");
    if (txt_registos == NULL || bin_registos == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    txt_registos = fopen("registos.txt", "a+");
    if (txt_registos == NULL) {
        printf("Erro ao abrir o ficheiro registos.txt\n");
        return;
    }
    if (fscanf(txt_registos, "%d %*d %*d %*d/%*d/%*d %*d:%*d\n", &id_registo) == EOF) {
        // Ficheiro está vazio, id registo fica a 1
        id_registo = 1;
    }
    else {
            id_registo++;
        // Ficheiro não está vazio, incrementa ao valor anterior
        while (fscanf(txt_registos, "%*d %*d %*d %*d/%*d/%*d %*d:%*d\n") != EOF) {
            id_registo++;
        }
    }

    fclose(txt_registos); // Fechar o ficheiro

    // Preencher os campos do novo registo
    new_registo->id = id_registo;
    new_registo->cliente_id = id_cliente;
    new_registo->meio_id = meio_id;
    new_registo->dia = dia;
    new_registo->mes = mes;
    new_registo->ano = ano;
    new_registo->horas = hora;
    new_registo->minutos = minuto;

    // Colocar o próximo pointer no topo da lista
    new_registo->seguinte = *headR;
    *headR = new_registo;

    // Escrever os valores do registo no ficheiro de texto
    txt_registos = fopen("registos.txt", "a");
    fprintf(txt_registos, "%d %d %d %d/%d/%d %d:%d\n", new_registo->id, new_registo->cliente_id, 
        new_registo->meio_id, new_registo->dia, new_registo->mes, new_registo->ano, new_registo->horas, 
        new_registo->minutos);
    fclose(txt_registos);

    // Escrever os valores do registo no ficheiro binário
    fwrite(new_registo, sizeof(registo), 1, bin_registos);
    fclose(bin_registos);

    // Abrir ficheiros em modo de escrita e leitura
    txt_historico = fopen("historico.txt", "r");
    bin_historico = fopen("historico.bin", "ab+");
    if (txt_historico == NULL || bin_historico == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }
    // Escrever os valores do registo no ficheiro de texto
    fprintf(txt_historico, "%d %d %d %d/%d/%d %d:%d\n", new_registo->id, new_registo->cliente_id, 
        new_registo->meio_id, new_registo->dia, new_registo->mes, new_registo->ano, new_registo->horas, 
        new_registo->minutos);
    fclose(txt_historico);

    // Escrever os valores do registo no ficheiro binário
    fwrite(new_registo, sizeof(registo), 1, bin_historico);
    fclose(bin_historico);

    system("clear || cls");
    printf("Aluguer registado com sucesso!\n");
    getchar();
}

// Função para passar os registos para uma lista ligada
void lerAluguer(FILE* f, registo** head) {
    f = fopen("registos.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }
    registo* current = NULL;
    while (!feof(f)) {
        registo* new_registo = (registo*)malloc(sizeof(registo));
        int result = fscanf(f, "%d %d %d %d/%d/%d %d:%d\n", &(new_registo->id), &(new_registo->cliente_id), 
            &(new_registo->meio_id), &(new_registo->dia), &(new_registo->mes), &(new_registo->ano), 
            &(new_registo->horas), &(new_registo->minutos));
              new_registo->seguinte = NULL;
        
        if (result != 8) {
            free(new_registo);
            break;
        }
        if (*head == NULL) {
            *head = new_registo;
            current = new_registo;
        }
        else {
            current->seguinte = new_registo;
            current = new_registo;
        }
    }
        if (*head == NULL) {
            system("clear || cls");
            printf("Nao existem registos de aluguer disponiveis.\n");
        }
}

// Função para atualizar os registos no ficheiro
void atualizarAluguer(FILE** f, registo* head_registo) {
    // Abre o ficheiro para atualizar os valores
    *f = fopen("registos.txt", "w");
    registo* curr_registo = head_registo;

    // Escreve os conteúdos de cada categoria
    while (curr_registo != NULL) {
        fprintf(*f, "%d %d %d %d/%d/%d %d:%d\n", curr_registo->id, curr_registo->cliente_id, curr_registo->meio_id,
            curr_registo->dia, curr_registo->mes, curr_registo->ano, curr_registo->horas, curr_registo->minutos);
        curr_registo = curr_registo->seguinte;
    }

    fclose(*f);
}

// Função para atualizar os registos no binário
void atualizarBinAluguer(FILE** f, registo* head_registo) {
    f = fopen("registos.bin", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }
        registo* curr_registo = head_registo;
        while (curr_registo != NULL) {
            fwrite(curr_registo, sizeof(registo), 1, f);
            curr_registo = curr_registo->seguinte;
        }

    fclose(f);
}

// Função para listar alugueres de um determinado cliente
void listarAluguer() {
    FILE* txt_meios, * txt_clientes, * txt_registos;
    registo r;
    cliente c;
    meio m;
    int meio_id, cliente_id, registo_id;
    char data[50];

    // Abrir ficheiros em modo de leitura
    txt_meios = fopen("meios.txt", "r");

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    lerMeios(txt_meios, &head_meio);
    fclose(txt_meios);

    // Abrir ficheiros em modo de leitura
    txt_clientes = fopen("clientes.txt", "r");

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    lerClientes(txt_clientes, &head);
    fclose(txt_clientes);

    // Abrir ficheiros em modo de leitura
    txt_registos = fopen("registos.txt", "r");

    // Ler todos os registos para uma lista ligada
    registo* head_registo = NULL;
    registo* curr_registo = NULL;
    lerAluguer(txt_registos, &head_registo);
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
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %d/%d/%d %d:%d\n\n", curr_registo->id, curr->nome, curr_meio->nome,
            curr_registo->dia, curr_registo->mes, curr_registo->ano, curr_registo->horas, curr_registo->minutos);
        curr_registo = curr_registo->seguinte;
    }
        getchar();
}

// Função para cancelar um aluguer de um determinado cliente
void cancelarAluguer() {
    FILE* txt_meios, * txt_clientes, * txt_registos, * bin_meios, * bin_clientes, * bin_registos;
    registo r;
    cliente c;
    meio m;
    int meio_id, cliente_id, registo_id;
    char data[50];

    // Abrir ficheiros em modo de leitura
    txt_meios = fopen("meios.txt", "r");

    // Ler todos os meios para uma lista ligada
    meio* head_meio = NULL;
    meio* curr_meio = NULL;
    lerMeios(txt_meios, &head_meio);
    fclose(txt_meios);

    // Abrir ficheiros em modo de leitura
    txt_clientes = fopen("clientes.txt", "r");

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    lerClientes(txt_clientes, &head);
    fclose(txt_clientes);

    // Abrir ficheiros em modo de leitura
    txt_registos = fopen("registos.txt", "r");

    // Ler todos os registos para uma lista ligada
    registo* head_registo = NULL;
    registo* curr_registo = NULL;
    lerAluguer(txt_registos, &head_registo);
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
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %d/%d/%d %d:%d\n\n", curr_registo->id, curr->nome, curr_meio->nome,
            curr_registo->dia, curr_registo->mes, curr_registo->ano, curr_registo->horas, curr_registo->minutos);
        curr_registo = curr_registo->seguinte;
    }

    // Pedir ao utilizador o ID do registo a ser removido
    printf("\nInsira o ID do aluguer que deseja cancelar: ");
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

            // atualizar os registos no ficheiro
            atualizarAluguer(&txt_registos, head_registo);

            // atualizar os meios no ficheiro
            atualizarMeio(&txt_meios, head_meio);

            // atualizar os clientes no ficheiro
            atualizarCliente(&txt_clientes, head);

            system("clear || cls");
            printf("Aluguer cancelado com sucesso!\n");
            getchar();
            return;
        }

        prev_registo = curr_registo;
        curr_registo = curr_registo->seguinte;
    }

    // atualizar os registos no binário
    atualizarBinAluguer(&bin_registos, head_registo);

    // atualizar os meios no binário
    atualizarBinMeio(&bin_meios, head_meio);

    // atualizar os clientes no binário
    atualizarBinCliente(&bin_clientes, head);

    system("clear || cls");
    printf("Aluguer nao encontrado.\n");
    getchar();
}



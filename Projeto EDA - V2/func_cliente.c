#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

int id_cliente = 0;

// Função para registar um cliente
void registarCliente(cliente** head) {

    // Alocar memória para um novo cliente
    cliente* new_cliente = (cliente*)malloc(sizeof(cliente));

    // Abrir o ficheiro de clientes para leitura em modo append
    FILE* f_txt = fopen("clientes.txt", "a+");
    if (f_txt == NULL) {
        printf("Erro ao abrir o ficheiro clientes.txt\n");
        return;
    }
    FILE* f_bin = fopen("clientes.bin", "ab");
    if (f_bin == NULL) {
        printf("Erro ao abrir o ficheiro clientes.bin\n");
        return;
    }

    int id = 0;
    char nome[50], morada[50], local_grafo[50], utilizador[50], password[50];
    int nif;
    float saldo;

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    while (fscanf(f_txt, "%d %s %d %s %f %s %s\n", &id, nome, &nif, morada, &saldo,
        utilizador, password) != EOF) {
    }
    id++; // Incrementar o último ID encontrado
    fclose(f_txt); // Fechar o ficheiro

    // Preencher os campos do novo cliente
    new_cliente->id = id;
    printf("Nome: ");
    scanf("%s", new_cliente->nome);
    printf("NIF: ");
    scanf("%d", &(new_cliente->nif));
    printf("Morada: ");
    scanf("%s", new_cliente->morada);
    printf("Saldo: ");
    scanf("%f", &(new_cliente->saldo));
    printf("Utilizador: ");
    scanf("%s", new_cliente->utilizador);
    printf("Password: ");
    scanf("%s", new_cliente->password);

    // Colocar o próximo pointer no topo da lista
    new_cliente->seguinte = *head;
    *head = new_cliente;

    // Escrever os valores do cliente no ficheiro de texto
        f_txt = fopen("clientes.txt", "a");
    fprintf(f_txt, "%d %s %d %s %.2f %s %s\n", new_cliente->id, new_cliente->nome, new_cliente->nif,
        new_cliente->morada, new_cliente->saldo, new_cliente->utilizador,
        new_cliente->password);
    fclose(f_txt);

    // Escrever os valores do cliente no ficheiro binário
        f_bin = fopen("clientes.bin", "ab");
    fwrite(new_cliente, sizeof(cliente), 1, f_bin);
    fclose(f_bin);

    system("clear || cls");
    printf("Cliente registado com sucesso!\n");
    getchar();
}

// Função para ler os clientes no ficheiro
cliente* lerCliente(FILE* f) {
    cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
    fscanf(f, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome,
        &(new_cliente->nif), new_cliente->morada, &(new_cliente->saldo), 
        new_cliente->utilizador, new_cliente->password);
    new_cliente->seguinte = NULL;
    return new_cliente;
}

// Função para passar os clientes para uma lista ligada
void lerClientes(FILE* f, cliente** head) {
    cliente* current = NULL;
    while (!feof(f)) {
        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(f, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome,
            &(new_cliente->nif), new_cliente->morada, &(new_cliente->saldo), 
            new_cliente->utilizador, new_cliente->password);
        new_cliente->seguinte = NULL;
        if (*head == NULL) {
            *head = new_cliente;
            current = new_cliente;
        }
        else {
            current->seguinte = new_cliente;
            current = new_cliente;
        }
    }
}

// Função para atualizar os clientes no ficheiro
void atualizarCliente(FILE** f, cliente* head) {
    // Abre o ficheiro para atualizar os valores
    *f = fopen("clientes.txt", "wb");
    cliente* curr = head;

    // Escreve os conteúdos de cada categoria
    while (curr != NULL) {
        fprintf(*f, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada,
            curr->saldo, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    fclose(*f);
}

// Função para atualizar os clientes no binário
void atualizarBinCliente(FILE** f, cliente* head) {
    f = fopen("clientes.bin", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    cliente* curr= head;
    while (curr != NULL) {
        fwrite(curr, sizeof(cliente), 1, f);
        curr = curr->seguinte;
    }

    fclose(f);
}

// Função para fazer login (Cliente)
void loginCliente(cliente** head, registo** headR) {
    char username[50];
    char password[50];

    printf("Utilizador: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE* f = fopen("clientes.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }
    cliente* curr = NULL;
    cliente* prev = NULL;

    while (!feof(f)) {
        curr = lerCliente(f);
        if (strcmp(curr->utilizador, username) == 0 && strcmp(curr->password, password) == 0) {
            id_cliente = curr->id;
            system("clear || cls");
            printf("Bem-vindo, %s!\n", curr->nome);
            getchar();
            showMenuCliente(headR);
            if (prev != NULL) {
                prev->seguinte = curr->seguinte;
                curr->seguinte = *head;
                *head = curr;
            }
            fclose(f);
            return;
        }
        prev = curr;
    }

    system("clear || cls");
    printf("Utilizador ou password incorretos!\n");
    getchar();
    fclose(f);
}

// Função para listar um cliente
void listarCliente() {
    int id;
    // Abre o ficheiro em modo read
    FILE* file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    // Mostra a lista de clientes
    printf("Lista de clientes:\n\n");
    cliente c;
    while (fscanf(file, "%d %s %d %s %f %s %s\n", &c.id, c.nome, &c.nif, c.morada, &c.saldo, 
        c.utilizador, c.password) != EOF) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", 
            c.id, c.nome, c.nif, c.morada, c.saldo, c.utilizador, c.password);
    }

    fclose(file);
    getchar();
}

// Função para remover um cliente
void removerCliente() {
    int id;

    // Abrir ficheiros em modo de escrita e leitura
    FILE* txt_file = fopen("clientes.txt", "r");
    FILE* bin_file = fopen("clientes.bin", "ab+");
    if (txt_file == NULL || bin_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    lerClientes(txt_file, &head);
    fclose(txt_file);

    // Escrever lista de clientes
    printf("Lista de clientes:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", 
            curr->id, curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, 
            curr->password);
            curr = curr->seguinte;
    }

    // Pedir ao user o ID do cliente a remover
    printf("Digite o ID do cliente a remover: ");
    scanf("%d", &id);

    // Procurar o cliente com o ID escolhido
    curr = head;
    cliente* prev = NULL;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("Cliente com ID %d nao encontrado!\n", id);
        getchar();
    }
    else {
        // Remover o cliente da lista ligada
        if (prev == NULL) {
            head = curr->seguinte;
        }
        else {
            prev->seguinte = curr->seguinte;
        }
        free(curr);

        // atualizar os clientes no ficheiro
        atualizarCliente(&txt_file, head);

        // atualizar os clientes no binário
        atualizarBinCliente(&txt_file, head);

        system("clear || cls");
        printf("Gestor com ID %d removido com sucesso!\n", id);
        getchar();
    }
}

// Função para alterar um cliente
void alterarCliente() {
    int id;

    // Abrir ficheiro em modo leitura
    FILE* file = fopen("clientes.txt", "r");
    FILE* bin_file = fopen("clientes.bin", "ab+");
    if (file == NULL || bin_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os clientes para uma lista ligada
    cliente* head = NULL;
    cliente* curr = NULL;
    lerClientes(file, &head);
    fclose(file);

    // Escrever lista de clientes
    printf("Lista de clientes:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", 
            curr->id, curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, 
            curr->password);
            curr = curr->seguinte;
    }

    // Pedir ao user o ID do cliente a alterar
    printf("Digite o ID do cliente a alterar: ");
    scanf("%d", &id);

    // Procurar o cliente com o ID escolhido
    curr = head;
    cliente* prev = NULL;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("Cliente com ID %d nao encontrado!\n", id);
        getchar();
    }
    else {
        // Alterar o cliente com o ID escolhido
        char campo[20], novo_valor[50];
        do {
            printf("Digite o campo a alterar (nome, nif, morada, saldo, utilizador, password), ou 'fim' para terminar: ");
            scanf("%s", campo);
            if (strcmp(campo, "fim") == 0) {
                break;
            }
            printf("Digite o novo valor: ");
            scanf("%s", novo_valor);
            if (strcmp(campo, "nome") == 0) {
                strcpy(curr->nome, novo_valor);
            }
            else if (strcmp(campo, "nif") == 0) {
                curr->nif = atoi(novo_valor);
            }
            else if (strcmp(campo, "morada") == 0) {
                strcpy(curr->morada, novo_valor);
            }
            else if (strcmp(campo, "saldo") == 0) {
                curr->saldo = atof(novo_valor);
            }
            else if (strcmp(campo, "utilizador") == 0) {
                strcpy(curr->utilizador, novo_valor);
            }
            else if (strcmp(campo, "password") == 0) {
                strcpy(curr->password, novo_valor);
            }

        } while (strcmp(campo, "fim") != 0);

        // atualizar os clientes no ficheiro
        atualizarCliente(&file, head);

        // atualizar os clientes no binário
        atualizarBinCliente(&file, head);

        system("clear || cls");
        printf("Cliente com ID %d alterado com sucesso!\n", id);
        getchar();

        // Libertar a memória alocada para a lista ligada
        curr = head;
        while (curr != NULL) {
            cliente* temp = curr;
            curr = curr->seguinte;
            free(temp);
        }
    }
}

// Função para listar alugueres de um determinado cliente
void listarAluguerCliente(int id_cliente) {
    FILE* txt_meios, * txt_clientes, * txt_registos;
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
    lerMeios(txt_meios, &head_meio);
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
    lerClientes(txt_clientes, &head);
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
    lerAluguer(txt_registos, &head_registo);
    fclose(txt_registos);

    // Percorrer a lista de registos
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        // Verificar se o registo pertence ao cliente logado
        if (curr_registo->cliente_id != id_cliente) {
            curr_registo = curr_registo->seguinte;
            continue;  // Saltar para a próxima iteração do ciclo while
        }

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
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %d/%d/%d %d:%d\n\n", curr_registo->id, curr->nome, curr_meio->tipo, 
        curr_registo->dia, curr_registo->mes, curr_registo->ano, curr_registo->horas, curr_registo->minutos);
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

// Função para listar um meio
void listarMeioCliente(char order_by) {
    // Abrir ficheiro em modo leitura
    FILE* file = fopen("meios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    // Criar array de meios
    meio m[100];
    int count = 0;
    while (fscanf(file, "%d %s %f %f %s %s %d\n", &m[count].id, m[count].tipo, &m[count].custo, &m[count].bateria,
        m[count].local, m[count].local_grafo, &m[count].reserva) != EOF) {
        if (m[count].reserva == 1) {
            continue; // saltar a adição do meio ao array caso reserva = 1
        }
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
    int printed_count = 0;
    printf("Lista de meios:\n\n");
    for (int i = 0; i < count; i++) {
        if (m[i].reserva == 1) {
            continue; // Saltar o meio
        }
        printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nLocal: %s\n\n", m[i].id, m[i].tipo,
            m[i].custo, m[i].bateria, m[i].local);
        printed_count++;
    }
    if (printed_count == 0) {
        printf("Nao existem meios disponiveis no momento.\n");
    }

    fclose(file);
    getchar();
}

// Função para cancelar um aluguer de um determinado cliente
void terminarAluguer(int id_cliente) {
    FILE* txt_meios, * txt_clientes, * txt_registos, * bin_meios, * bin_clientes, * bin_registos;
    registo r;
    cliente c;
    meio m;
    int meio_id, cliente_id, registo_id;
 
    // Recebe o tempo atual
    time_t now = time(NULL);
    struct tm* current_time = localtime(&now);

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
    lerMeios(txt_meios, &head_meio);
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
    lerClientes(txt_clientes, &head);
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
    lerAluguer(txt_registos, &head_registo);
    fclose(txt_registos);

        // Percorrer a lista de registos
    curr_registo = head_registo;
    while (curr_registo != NULL) {
        // Verificar se o registo pertence ao cliente logado
        if (curr_registo->cliente_id != id_cliente) {
            curr_registo = curr_registo->seguinte;
            continue; // Saltar para a próxima iteração do ciclo while
        }
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
        printf("ID: %d\nCliente: %s\nMeio: %s\nData: %d/%d/%d %d:%d\n\n", curr_registo->id, curr->nome, curr_meio->tipo,
            curr_registo->dia, curr_registo->mes, curr_registo->ano, curr_registo->horas, curr_registo->minutos);
        curr_registo = curr_registo->seguinte;
    }

    // Pedir ao utilizador o ID do registo a ser cancelado
    printf("\nInsira o ID do aluguer que deseja cancelar: ");
    scanf("%d", &registo_id);

    // Percorrer a lista de registos para encontrar o registo a ser cancelado
    curr_registo = head_registo;
    registo* prev_registo = NULL;
    while (curr_registo != NULL) {
        if (curr_registo->id == registo_id) {
    // Cálculo para encontrar a diferença em minutos da data de início e fim da viagem
    int minutes_difference = ((current_time->tm_hour * 60) + current_time->tm_min) - ((curr_registo->horas * 60) + curr_registo->minutos);
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
                    // Verificar se o saldo é suficiente para cobrir o custo do meio
                    if (curr->saldo >= (curr_meio->custo * minutes_difference)) {
                        // Decrementar o saldo do cliente pelo custo do meio
                        curr->saldo -= (curr_meio->custo * minutes_difference);
                    }
                    else {
                        system("clear || cls");
                        printf("Saldo insuficiente.\n");
                        getchar();
                        return;
                    }
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
    atualizarBinAluguer(&txt_registos, head_registo);

    // atualizar os meios no binário
    atualizarBinMeio(&txt_meios, head_meio);

    // atualizar os clientes no binário
    atualizarBinCliente(&txt_clientes, head);

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

// Função para carregar o saldo do cliente
void carregarSaldo(int id_cliente) {
    FILE * txt_clientes, * bin_clientes;
    float valor;

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
    lerClientes(txt_clientes, &head);
    fclose(txt_clientes);

    // Percorrer a lista ligada para encontrar o cliente com o id = id_cliente
    curr = head;
    while (curr != NULL && curr->id != id_cliente) {
        curr = curr->seguinte;
    }
    printf("Saldo atual: %.2f\n", curr->saldo);
    printf("\nDigite o valor a carregar: ");
    scanf("%f", &valor);
    curr->saldo += valor;
    system("clear || cls");
    printf("Saldo atual: %.2f\n", curr->saldo);
    getchar();

    // atualizar os clientes no ficheiro
    atualizarCliente(&txt_clientes, head);

    // atualizar os clientes no binário
    atualizarBinCliente(&txt_clientes, head);

    // Libertar memória alocada para as listas ligadas
    curr = head;
    while (curr != NULL) {
        cliente* temp = curr;
        curr = curr->seguinte;
        free(temp);
    }
}

// Menu para clientes
void showMenuCliente(registo** headR) {
   
    int opcao;
    char order_by;
    do {
        printf("=================================\n");
        printf("|| Bem-vindo ao Menu de Cliente! ||\n");
        printf("=================================\n\n");
        printf("CLIENTES\n\n");
        printf("Escolha uma opcao:\n");
        printf("1 - Alugar meios transporte\n");
        printf("2 - Consultar meios diponiveis\n");
        printf("3 - Consultar alugueres\n");
        printf("4 - Terminar alugueres\n");
        printf("5 - Carregar Saldo\n\n");
        printf("OUTROS\n\n");
        printf("Escolha uma opcao:\n");
        printf("0 - Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            system("clear || cls");
            printf("\nALUGAR MEIO(S) TRANSPORTE(S)\n\n");
            registarAluguer(id_cliente, headR);
            break;

        case 2:
            system("clear || cls");
            printf("\nCONSULTAR MEIO(S)\n\n");
            printf("Como deseja ordenar a lista de meios?\n\n");
            printf("B - Por bateria\n");
            printf("D - Por distancia\n\n");
            printf("Opcao: ");
            scanf(" %c", &order_by);
            printf("\n\n");
            listarMeioCliente(order_by);
            break;

        case 3:
            system("clear || cls");
            printf("\nCONSULTAR ALUGUER(ES)\n\n");
            listarAluguerCliente(id_cliente);
            break;

        case 4:
            system("clear || cls");
            printf("\nTERMINAR ALUGUER\n\n");
            terminarAluguer(id_cliente);
            break;

        case 5:
            system("clear || cls");
            printf("\nCARREGAR SALDO\n\n");
            carregarSaldo(id_cliente);
            break;

        case 0:
            system("clear || cls");
            printf("\nObrigado por utilizar o nosso sistema!\n");
            break;

        default:
            system("clear || cls");
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }
        // espera um pouco antes de limpar a tela
        printf("\nPressione qualquer tecla para continuar...");
        getchar(); // consome o char de quebra de linha deixado pelo scanf     
        // limpa o ecrã
        system("clear || cls");

    } while (opcao != 0);
}



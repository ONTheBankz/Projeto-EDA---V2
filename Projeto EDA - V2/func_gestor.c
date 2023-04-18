#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

// Função para registar um gestor
void registarGestor(gestor** head) {
    // Alocar memória para um novo gestor
    gestor* new_gestor = (gestor*)malloc(sizeof(gestor));
    // Abrir o ficheiro de gestores para leitura
    FILE* f = fopen("gestores.txt", "a+");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro gestores.txt\n");
        return;
    }

    int id = 0;
    char nome[50], utilizador[50], password[50];

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    while (fscanf(f, "%d %s %s %s\n", &id, nome, utilizador, password) != EOF) {

    }
    id++;  // Incrementar o último ID encontrado
    fclose(f); // Fechar o ficheiro

    // Preencher os campos do novo gestor
    new_gestor->id = id;
    printf("Nome: ");
    scanf("%s", new_gestor->nome);
    printf("Utilizador: ");
    scanf("%s", new_gestor->utilizador);
    printf("Password: ");
    scanf("%s", new_gestor->password);

    // Colocar o próximo pointer no topo da lista
    new_gestor->seguinte = *head;
    *head = new_gestor;

    // Escrever os valores do gestor no ficheiro de texto
    f = fopen("gestores.txt", "a");
    fprintf(f, "%d %s %s %s\n", new_gestor->id, new_gestor->nome, new_gestor->utilizador, new_gestor->password);
    fclose(f);

    // Escrever os valores do gestor no ficheiro binário
    f = fopen("gestores.bin", "ab");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }
    fwrite(new_gestor, sizeof(gestor), 1, f);
    fclose(f);

    system("clear || cls");
    printf("Gestor registado com sucesso!\n");
    getchar();
}

// Função para ler um gestor do ficheiro
gestor* lerGestor(FILE* f) {
    gestor* new_gestor = (gestor*)malloc(sizeof(gestor));
    fscanf(f, "%d %s %s %s\n", &(new_gestor->id), new_gestor->nome, new_gestor->utilizador, new_gestor->password);
    new_gestor->seguinte = NULL;
    return new_gestor;
}

// Função para fazer login (Gestor)
void loginGestor(gestor** head, meio** headM, registo** headR) {
    char username[50];
    char password[50];

    printf("Utilizador: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE* f = fopen("gestores.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }
    gestor* curr = NULL;
    gestor* prev = NULL;

    while (!feof(f)) {
        curr = lerGestor(f);
        if (strcmp(curr->utilizador, username) == 0 && strcmp(curr->password, password) == 0) {
            system("clear || cls");
            printf("Bem-vindo, %s!\n", curr->nome);
            getchar();
            showMenuGestor(headM, headR);
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

// Função para listar um gestor
void listar_gestor() {
    int id;
// Abre o ficheiro em modo read
    FILE* file = fopen("gestores.txt", "r");
    if (file == NULL) {
    printf("Erro ao abrir arquivo!\n");
    exit(1);
}

// Mostra a lista de gestores
    printf("Lista de gestores:\n\n");
    gestor g;
    while (fscanf(file, "%d %s %s %s\n", &g.id, g.nome, g.utilizador, g.password) != EOF) {
    printf("ID: %d\nNome: %s\nUtilizador: %s\nPassword: %s\n\n", g.id, g.nome, g.utilizador, g.password);
}

    fclose(file);
    getchar();
}

// Função para remover gestor por ID
void remover_gestor() {
    int id;

    // Abrir ficheiros em modo de escrita e leitura
    FILE* txt_file = fopen("gestores.txt", "r");
    FILE* bin_file = fopen("gestores.bin", "ab+");
    if (txt_file == NULL || bin_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os gestores para uma lista ligada
    gestor* head = NULL;
    gestor* curr = NULL;
    while (!feof(txt_file)) {
        gestor* new_gestor = (gestor*)malloc(sizeof(gestor));
        fscanf(txt_file, "%d %s %s %s\n", &(new_gestor->id), new_gestor->nome, new_gestor->utilizador, new_gestor->password);
        new_gestor->seguinte = NULL;

        if (head == NULL) {
            head = new_gestor;
        }
        else {
            curr->seguinte = new_gestor;
        }
        curr = new_gestor;
    }

    fclose(txt_file);

    // Escrever lista de gestores
    printf("Lista de gestores:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nUtilizador: %s\nPassword: %s\n\n", curr->id, curr->nome, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    // Pedir ao user o ID do gestor a remover
    printf("Digite o ID do gestor a remover: ");
    scanf("%d", &id);

    // Procurar o gestor com o ID escolhido
    curr = head;
    gestor* prev = NULL;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("Gestor com ID %d nao encontrado!\n", id);
        getchar();
    }
    else {
        // Remover o gestor da lista ligada
        if (prev == NULL) {
            head = curr->seguinte;
        }
        else {
            prev->seguinte = curr->seguinte;
        }
        free(curr);

        // Escrever a lista atualizada de gestores de volta para o arquivo de texto
        txt_file = fopen("gestores.txt", "w");
        if (txt_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fprintf(txt_file, "%d %s %s %s\n", curr->id, curr->nome, curr->utilizador, curr->password);
            curr = curr->seguinte;
        }

        fclose(txt_file);

        // Escrever a lista atualizada de gestores de volta para o arquivo binário
        bin_file = fopen("gestores.bin", "wb");
        if (bin_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fwrite(curr, sizeof(gestor), 1, bin_file);
            curr = curr->seguinte;
        }

        fclose(bin_file);
        system("clear || cls");
        printf("Gestor com ID %d removido com sucesso!\n", id);
        getchar();
    }
}

// Função para alterar um gestor
void alterar_gestor() {
    int id;

    // Abrir ficheiro em modo leitura
    FILE* file = fopen("gestores.txt", "r");
    FILE* bin_file = fopen("gestores.bin", "ab+");
    if (file == NULL || bin_file == NULL) {
        system("clear || cls");
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    // Ler todos os gestores para uma lista ligada
    gestor* head = NULL;
    gestor* curr = NULL;
    while (!feof(file)) {
        gestor* new_gestor = (gestor*)malloc(sizeof(gestor));
        fscanf(file, "%d %s %s %s\n", &(new_gestor->id), new_gestor->nome, new_gestor->utilizador, new_gestor->password);
        new_gestor->seguinte = NULL;

        if (head == NULL) {
            head = new_gestor;
        }
        else {
            curr->seguinte = new_gestor;
        }
        curr = new_gestor;
    }

    fclose(file);

    // Escrever lista de gestores
    printf("Lista de gestores:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nUtilizador: %s\nPassword: %s\n\n", curr->id, curr->nome, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    // Pedir ao user o ID do gestor a alterar
    printf("Digite o ID do gestor a alterar: ");
    scanf("%d", &id);

    // Procurar o gestor com o ID escolhido
    curr = head;
    gestor* prev = NULL;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("Gestor com ID %d nao encontrado!\n", id);
        getchar();
    }
    else {       
        // Alterar o gestor com o ID escolhido
        char campo[20], novo_valor[50];
        do {
            printf("Digite o campo a alterar (nome, utilizador, password), ou 'fim' para terminar: ");
            scanf("%s", campo);
            if (strcmp(campo, "fim") == 0) {
                break;
            }
            printf("Digite o novo valor: ");
            scanf("%s", novo_valor);
            if (strcmp(campo, "nome") == 0) {
                strcpy(curr->nome, novo_valor);
            }
            else if (strcmp(campo, "utilizador") == 0) {
                strcpy(curr->utilizador, novo_valor);
            }
            else if (strcmp(campo, "password") == 0) {
                strcpy(curr->password, novo_valor);
            }

        } while (strcmp(campo, "fim") != 0);

        // Escrever a lista atualizada de gestores no arquivo
        file = fopen("gestores.txt", "w");
        if (file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fprintf(file, "%d %s %s %s\n", curr->id, curr->nome, curr->utilizador, curr->password);
            curr = curr->seguinte;
        }

        fclose(file);

        // Escrever a lista atualizada de gestores de volta para o arquivo binário
        bin_file = fopen("gestores.bin", "wb");
        if (bin_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }
        curr = head;
        while (curr != NULL) {
            fwrite(curr, sizeof(gestor), 1, bin_file);
            curr = curr->seguinte;
        }

        fclose(bin_file);

        system("clear || cls");
        printf("Gestor com ID %d alterado com sucesso!\n", id);
        getchar();

        // Libertar a memória alocada para a lista ligada
        curr = head;
        while (curr != NULL) {
            gestor* temp = curr;
            curr = curr->seguinte;
            free(temp);
        }
    }
}

// Função para alugar um meio
void registar_aluguer_gestor(registo** headR) {
    FILE* txt_meios, * bin_meios, * txt_clientes, * bin_clientes, * txt_registos, * bin_registos, * txt_historico, * bin_historico;
    registo r;
    cliente c;
    meio m;
    int id_registo = 0;
    int id, id_cliente, meio_id, cliente_id, registo_id;
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
    
    // Abrir ficheiros em modo de escrita e leitura
    txt_clientes = fopen("clientes.txt", "r");
    bin_clientes = fopen("clientes.bin", "ab+");
    if (txt_clientes == NULL || bin_clientes == NULL) {
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
    system("clear || cls");

        // Escrever lista de clientes
        printf("Lista de clientes:\n\n");
        curr = head;
        while (curr != NULL) {
            printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", curr->id,
                    curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, curr->password);
                    curr = curr->seguinte;
        }

    printf("Introduza o ID do cliente que pretende efetuar o registo: ");
        scanf("%d", &id_cliente);

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

// Função para carregar o saldo de um cliente
void carregar_saldo_gestor() {
    FILE* txt_clientes, * bin_clientes;
    float valor;
    int id_cliente;

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

    // Escrever lista de clientes
    printf("Lista de clientes:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", curr->id,
            curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    // Pedir ao utilizador para introduzir o ID do cliente
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    // Percorrer a lista ligada para encontrar o cliente com o id = id_cliente
    curr = head;
    while (curr != NULL && curr->id != id_cliente) {
        curr = curr->seguinte;
    }

    if (curr == NULL) {
        system("clear || cls");
        printf("O cliente com o ID %d nao existe!\n", id_cliente);
        getchar();
        exit(1);
    }

    printf("\nSaldo atual: %.2f\n", curr->saldo);
    printf("\nDigite o valor a carregar: ");
    scanf("%f", &valor);
    curr->saldo += valor;
    system("clear || cls");
    printf("Saldo atual: %.2f\n", curr->saldo);
    getchar();

    // Atualizar o ficheiro de clientes com a nova lista de clientes
    txt_clientes = fopen("clientes.txt", "w");
    curr = head;
    while (curr != NULL) {
        fprintf(txt_clientes, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada,
            curr->saldo, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    fclose(txt_clientes);

    // Atualizar o ficheiro binario de clientes com a nova lista de clientes
    bin_clientes = fopen("clientes.bin", "wb");
    curr = head;
    while (curr != NULL) {
        fwrite(curr, sizeof(cliente), 1, bin_clientes);
        curr = curr->seguinte;
    }
    fclose(bin_clientes);

    // Libertar memória alocada para as listas ligadas
    curr = head;
    while (curr != NULL) {
        cliente* temp = curr;
        curr = curr->seguinte;
        free(temp);
    }
}

// Menu Gestor
void showMenuGestor(meio** headM, registo** headR) {
    int opcao;
    char order_by;
    do {
        printf("=================================\n");
        printf("|| Bem-vindo ao Menu de Gestor! ||\n");
        printf("=================================\n\n");
        printf("CLIENTES\n\n");
        printf("Escolha uma opcao:\n");
        printf("1 - Listar cliente\n");
        printf("2 - Remover cliente\n");
        printf("3 - Alterar dados cliente\n");
        printf("4 - Carregar saldo cliente\n\n");
        printf("GESTORES\n\n");
        printf("Escolha uma opcao:\n");
        printf("5 - Listar gestor\n");
        printf("6 - Remover gestor\n");
        printf("7 - Alterar dados gestor\n\n");
        printf("MEIOS TRANSPORTE\n\n");
        printf("Escolha uma opcao:\n");
        printf("8 - Registar meio\n");
        printf("9 - Listar meio\n");
        printf("10 - Remover meio\n");
        printf("11 - Alterar dados meio\n\n");
        printf("ALUGUERES\n\n");
        printf("Escolha uma opcao:\n");
        printf("12 - Registar novo aluguer\n");
        printf("13 - Listar registos aluguer\n");
        printf("14 - Cancelar Aluguer\n\n");
        printf("OUTROS\n\n");
        printf("Escolha uma opcao:\n");
        printf("0 - Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            system("clear || cls");
            printf("\nLISTAR CLIENTE\n\n");
            listar_cliente();
            break;

        case 2:
            system("clear || cls");
            printf("\nREMOVER CLIENTE\n\n");
            remover_cliente();
            break;

        case 3:
            system("clear || cls");
            printf("\nALTERAR DADOS CLIENTE\n\n");
            alterar_cliente();
            break;

        case 4:
            system("clear || cls");
            printf("\nCARREGAR SALDO\n\n");
            carregar_saldo_gestor();
            break;

        case 5:
            system("clear || cls");
            printf("\nLISTAR GESTOR\n\n");
            listar_gestor();
            break;

        case 6:
            system("clear || cls");
            printf("\nREMOVER GESTOR\n\n");         
            remover_gestor();
            break;

        case 7:
            system("clear || cls");
            printf("\nALTERAR DADOS GESTOR\n\n");
            alterar_gestor();
            break;

        case 8:
            system("clear || cls");
            printf("\nREGISTAR MEIO\n\n");
            registarMeio(headM);
            break;

        case 9:
            system("clear || cls");
            printf("\nLISTAR MEIO\n\n");
            printf("\nComo deseja ordenar a lista de meios?\n");
            printf("B - Por bateria\n");
            printf("D - Por distancia\n\n");
            printf("Opcao: ");
            scanf(" %c", &order_by);
            printf("\n\n");
            listar_meio(order_by);
            break;
 
        case 10:
            system("clear || cls");
            printf("\nREMOVER MEIO\n\n");
            remover_meio();
            break;

        case 11:
            system("clear || cls");
            printf("\nALTERAR DADOS MEIO\n\n");
            alterar_meio();
            break;
        
        case 12:
            system("clear || cls");
            printf("\nREGISTAR ALUGUER\n\n");
            registar_aluguer_gestor(headR);
            break;

        case 13:
            system("clear || cls");
            printf("\nLISTAR ALUGUER\n\n");
            listar_aluguer();
            break;

        case 14:
            system("clear || cls");
            printf("\CANCELAR ALUGUER\n\n");
            cancelar_aluguer();
            break;

        case 0:
            printf("\nObrigado por utilizar o nosso sistema!\n");
            break;

        default:
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



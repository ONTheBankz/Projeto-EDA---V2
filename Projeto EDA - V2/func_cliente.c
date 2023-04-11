#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char nome[50], morada[50], utilizador[50], password[50];
    int nif;
    float saldo;

    // Encontrar o último ID presente no ficheiro e incrementá-lo
    while (fscanf(f_txt, "%d %s %d %s %f %s %s\n", &id, nome, &nif, morada, &saldo, utilizador, password) != EOF) {
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
        new_cliente->morada, new_cliente->saldo, new_cliente->utilizador, new_cliente->password);
    fclose(f_txt);

    // Escrever os valores do cliente no ficheiro binário
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
        &(new_cliente->nif), new_cliente->morada, &(new_cliente->saldo), new_cliente->utilizador, new_cliente->password);
    new_cliente->seguinte = NULL;
    return new_cliente;
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
void listar_cliente() {
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
    while (fscanf(file, "%d %s %d %s %f %s %s\n", &c.id, c.nome, &c.nif, c.morada, &c.saldo, c.utilizador, c.password) != EOF) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", c.id, c.nome, c.nif,
            c.morada, c.saldo, c.utilizador, c.password);
    }

    fclose(file);
    getchar();
}

// Função para remover um cliente
void remover_cliente() {
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
    while (!feof(txt_file)) {
        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(txt_file, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome, &(new_cliente->nif), 
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

    fclose(txt_file);

    // Escrever lista de clientes
    printf("Lista de clientes:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", curr->id, 
            curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, curr->password);
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

        // Escrever a lista atualizada de clientes de volta para o arquivo de texto
        txt_file = fopen("clientes.txt", "w");
        if (txt_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fprintf(txt_file, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada, curr->saldo, 
                curr->utilizador, curr->password);
                curr = curr->seguinte;
        }

        fclose(txt_file);

        // Escrever a lista atualizada de clientes de volta para o arquivo binário
        bin_file = fopen("clientes.bin", "wb");
        if (bin_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }
        curr = head;
        while (curr != NULL) {
            fwrite(curr, sizeof(cliente), 1, bin_file);
            curr = curr->seguinte;
        }

        fclose(bin_file);
        system("clear || cls");
        printf("Gestor com ID %d removido com sucesso!\n", id);
        getchar();
    }
}

// Função para alterar um cliente
void alterar_cliente() {
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
    while (!feof(file)) {

        cliente* new_cliente = (cliente*)malloc(sizeof(cliente));
        fscanf(file, "%d %s %d %s %f %s %s\n", &(new_cliente->id), new_cliente->nome, &(new_cliente->nif),
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

    fclose(file);

    // Escrever lista de clientes
    printf("Lista de clientes:\n\n");
    curr = head;
    while (curr != NULL) {
        printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", curr->id,
            curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, curr->password);
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

        // Escrever a lista atualizada de clientes no arquivo
        file = fopen("clientes.txt", "w");
        if (file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }

        curr = head;
        while (curr != NULL) {
            fprintf(file, "%d %s %d %s %.2f %s %s\n", curr->id, curr->nome, curr->nif, curr->morada, curr->saldo,
                curr->utilizador, curr->password);
                curr = curr->seguinte;
        }

        fclose(file);

        // Escrever a lista atualizada de clientes de volta para o arquivo binário
        bin_file = fopen("clientes.bin", "wb");
        if (bin_file == NULL) {
            system("clear || cls");
            printf("Erro ao abrir arquivo!\n");
            getchar();
            exit(1);
        }
        curr = head;
        while (curr != NULL) {
            fwrite(curr, sizeof(cliente), 1, bin_file);
            curr = curr->seguinte;
        }

        fclose(bin_file);

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
        printf("3 - Consultar aluguer(es)\n\n");
        printf("OUTROS\n\n");
        printf("Escolha uma opcao:\n");
        printf("0 - Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            printf("\nALUGAR MEIO(S) TRANSPORTE(S)\n\n");
            registar_aluguer(id_cliente, headR);
            break;

        case 2:
            printf("\CONSULTAR MEIO(S)\n\n");
            printf("\nComo deseja ordenar a lista de meios?\n");
            printf("B - Por bateria\n");
            printf("D - Por distancia\n\n");
            printf("Opcao: ");
            scanf(" %c", &order_by);
            printf("\n\n");
            listar_meio(order_by);
            break;

        case 3:
            printf("\nCONSULTAR ALUGUER(ES)\n\n");
            listar_aluguer_cliente();
            break;

        case 4:
            printf("\nLISTAR GESTOR\n\n");
          
            break;

        case 5:
            printf("\nREMOVER GESTOR\n\n");
          
            break;

        case 6:
            printf("\nALTERAR DADOS GESTOR\n\n");
        
            break;

        case 7:
            printf("\nREGISTAR MEIO\n\n");
           
            break;

        case 8:
          
            break;

        case 9:
            printf("\nREMOVER MEIO\n\n");
            remover_meio();
            break;

        case 10:
            printf("\nALTERAR DADOS MEIO\n\n");
            alterar_meio();
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



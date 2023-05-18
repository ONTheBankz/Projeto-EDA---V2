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

// Função para passar os gestores para uma lista ligada
void lerGestores(FILE* f, gestor** head) {
    gestor* current = NULL;
    while (!feof(f)) {
        gestor* new_gestor = (gestor*)malloc(sizeof(gestor));
        fscanf(f, "%d %s %s %s\n", &(new_gestor->id), new_gestor->nome, new_gestor->utilizador, new_gestor->password);
        new_gestor->seguinte = NULL;
        if (*head == NULL) {
            *head = new_gestor;
            current = new_gestor;
        }
        else {
            current->seguinte = new_gestor;
            current = new_gestor;
        }
    }
}

// Função para atualizar os gestores no ficheiro
void atualizarGestor(FILE** f, gestor* head) {
    // Abre o ficheiro para atualizar os valores
    *f = fopen("gestores.txt", "wb");
    gestor* curr = head;

    // Escreve os conteúdos de cada categoria
    while (curr != NULL) {
        fprintf(*f, "%d %s %s %s\n", curr->id, curr->nome, curr->utilizador, curr->password);
        curr = curr->seguinte;
    }

    fclose(*f);
}

// Função para atualizar os gestores no binário
void atualizarBinGestor(FILE** f, gestor* head) {
    f = fopen("gestores.bin", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        getchar();
        exit(1);
    }

    gestor* curr = head;
    while (curr != NULL) {
        fwrite(curr, sizeof(gestor), 1, f);
        curr = curr->seguinte;
    }

    fclose(f);
}

// Função para fazer login (Gestor)
void loginGestor(gestor** head, meio** headM, registo** headR, grafo** headV, aresta** headA) {
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
            showMenuGestor(headM, headR, headV, headA);
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
void listarGestor() {
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
void removerGestor() {
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
    lerGestores(txt_file, &head);
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

        // atualizar os gestores no ficheiro
        atualizarGestor(&txt_file, head);

        // atualizar os gestores no binário
        atualizarBinGestor(&txt_file, head);

        system("clear || cls");
        printf("Gestor com ID %d removido com sucesso!\n", id);
        getchar();
    }
}

// Função para alterar um gestor
void alterarGestor() {
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
    lerGestores(file, &head);
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

        // atualizar os gestores no ficheiro
        atualizarGestor(&file, head);

        // atualizar os gestores no binário
        atualizarBinGestor(&file, head);

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
void registarAluguerGestor(registo** headR) {
    FILE* txt_meios, * bin_meios, * txt_clientes, * bin_clientes, * txt_registos, 
        * bin_registos, * txt_historico, * bin_historico;
    registo r;
    cliente c;
    meio m;
    int id_registo = 0;
    int id, id_cliente, meio_id, cliente_id, registo_id;
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
    if (txt_meios == NULL || bin_meios == NULL) {
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
    lerClientes(txt_clientes, &head);
    fclose(txt_clientes);

    // Escrever lista de meios
    printf("Lista de meios:\n\n");
    curr_meio = head_meio;
    while (curr_meio != NULL) {
        if (curr_meio->reserva != 1) {
            printf("ID: %d\nTipo: %s\nCusto: %.2f\nBateria: %.2f\nGeocodigo: %s\nReserva: %d\n\n", 
                curr_meio->id, curr_meio->tipo, curr_meio->custo, curr_meio->bateria,
                curr_meio->local_grafo, curr_meio->reserva);
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
            printf("ID: %d\nNome: %s\nNIF: %d\nMorada: %s\nSaldo: %.2f\nUtilizador: %s\nPassword: %s\n\n", 
                curr->id, curr->nome, curr->nif, curr->morada, curr->saldo, curr->utilizador, 
                curr->password);
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
   
    // atualizar os meios no ficheiro
    atualizarMeio(&txt_meios, head_meio);

    // atualizar os meios no binário
    atualizarBinMeio(&txt_meios, head_meio);

    // Procurar o cliente com o ID escolhido
    curr = head;
    while (curr != NULL && curr->id != id_cliente) {
        curr = curr->seguinte;
    }

    // atualizar os clientes no ficheiro
    atualizarCliente(&txt_clientes, head);

    // atualizar os clientes no binário
    atualizarBinCliente(&txt_clientes, head);

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

    // Preencher os campos do novo cliente
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

// Função para carregar o saldo de um cliente
void carregarSaldoGestor() {
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
    lerClientes(txt_clientes, &head);
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

// Menu Gestor
void showMenuGestor(meio** headM, registo** headR, grafo** headV, aresta** headA) {
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
        printf("12 - Registar aluguer\n");
        printf("13 - Listar aluguer\n");
        printf("14 - Cancelar Aluguer\n\n");
        printf("LOCALIZACOES\n\n");
        printf("Escolha uma opcao:\n");
        printf("15 - Registar localizacao\n");
        printf("16 - Editar localizacao\n");
        printf("17 - Remover localizacao\n");
        printf("18 - Registar conexao (locais)\n");
        printf("19 - Remover conexao (locais)\n");
        printf("20 - Listar meios por raio (distancia)\n");
        printf("OUTROS\n\n");
        printf("Escolha uma opcao:\n");
        printf("0 - Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            system("clear || cls");
            printf("\nLISTAR CLIENTE\n\n");
            listarCliente();
            break;

        case 2:
            system("clear || cls");
            printf("\nREMOVER CLIENTE\n\n");
            removerCliente();
            break;

        case 3:
            system("clear || cls");
            printf("\nALTERAR DADOS CLIENTE\n\n");
            alterarCliente();
            break;

        case 4:
            system("clear || cls");
            printf("\nCARREGAR SALDO\n\n");
            carregarSaldoGestor();
            break;

        case 5:
            system("clear || cls");
            printf("\nLISTAR GESTOR\n\n");
            listarGestor();
            break;

        case 6:
            system("clear || cls");
            printf("\nREMOVER GESTOR\n\n");         
            removerGestor();
            break;

        case 7:
            system("clear || cls");
            printf("\nALTERAR DADOS GESTOR\n\n");
            alterarGestor();
            break;

        case 8:
            system("clear || cls");
            printf("\nREGISTAR MEIO\n\n");
            registarMeio(headM, headV);
            break;

        case 9:
            system("clear || cls");
            printf("\nLISTAR MEIO\n\n");
            printf("Como deseja ordenar a lista de meios?\n");
            printf("B - Por bateria\n");
            printf("C - por custo\n");
            printf("Opcao: ");
            scanf(" %c", &order_by);
            printf("\n\n");
            listarMeio(order_by);
            break;
 
        case 10:
            system("clear || cls");
            printf("\nREMOVER MEIO\n\n");
            removerMeio();
            break;

        case 11:
            system("clear || cls");
            printf("\nALTERAR DADOS MEIO\n\n");
            alterarMeio();
            break;
        
        case 12:
            system("clear || cls");
            printf("\nREGISTAR ALUGUER\n\n");
            registarAluguerGestor(headR);
            break;

        case 13:
            system("clear || cls");
            printf("\nLISTAR ALUGUER\n\n");
            listarAluguer();
            break;

        case 14:
            system("clear || cls");
            printf("\CANCELAR ALUGUER\n\n");
            cancelarAluguer();
            break;

        case 15:
            system("clear || cls");
            printf("\nREGISTAR LOCALIZACAO\n\n");
            criarVertice(headV);
            break;

        case 16:
            system("clear || cls");
            printf("\nEDITAR LOCALIZACAO\n\n");
            editarVertice(headV);
            break;

        case 17:
            system("clear || cls");
            printf("\REMOVER LOCALIZACAO\n\n");
            removerVertice(headV);
            break;

        case 18:
            system("clear || cls");
            printf("\REGISTAR CONEXAO (LOCAIS)\n\n");
            criarAresta(headV);
            break;

        case 19:
            system("clear || cls");
            printf("\REMOVER CONEXAO (LOCAIS)\n\n");
            removerAresta(headA);
            break;

        case 20:
            system("clear || cls");
            printf("\LISTAR MEIOS RAIO (DISTANCIA)\n\n");
            verConexoesRaio(headV, headA, headM);
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

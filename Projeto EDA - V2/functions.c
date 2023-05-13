#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Menu Login
void showMenu(cliente** headC, gestor** headG, meio** headM, registo** headR) {
    int opcao;
    do {
        printf("===================================\n");
        printf("|| Bem-vindo ao Sistema de Login! ||\n");
        printf("===================================\n\n");
        printf("Escolha uma opcao:\n");
        printf("1 - Registar novo cliente\n");
        printf("2 - Registar novo gestor\n");
        printf("3 - Login de cliente\n");
        printf("4 - Login de gestor\n");
        printf("0 - Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            system("clear || cls");
            printf("\nREGISTAR NOVO CLIENTE\n\n");
            registarCliente(headC);
            break;

        case 2:
            system("clear || cls");
            printf("\nREGISTAR NOVO GESTOR\n\n");
            registarGestor(headG);
            break;

        case 3:
            system("clear || cls");
            printf("\nLOGIN DO CLIENTE\n\n");
            loginCliente(headC, headR);
            break;

        case 4:
            system("clear || cls");
            printf("\nLOGIN DO GESTOR\n\n");           
            loginGestor(headG, headM, headR);
            break;
        
        case 0:
            printf("\nObrigado por utilizar o nosso sistema!\n");
            getchar();
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

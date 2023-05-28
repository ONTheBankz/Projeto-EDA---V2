#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Função para registar um meio
void registarMeio(meio** head, grafo** g) {
	*g = carregarGrafo();
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
	char nome[50], tipo[50], distancia[50], local[50], local_grafo[50];
	float custo, bateria;

	// Encontrar o último ID presente no ficheiro e incrementá-lo
	while (fscanf(f, "%d %s %s %f %f %s %s %d\n", &id, nome, tipo, &custo, &bateria,
		local, local_grafo, &reserva) != EOF) {
	}
	id++;  // Incrementar o último ID encontrado
	fclose(f); // Fechar o ficheiro

	// Preencher os campos do novo meio
	novo_meio->id = id;
	novo_meio->reserva = 0;
	printf("Digite o tipo do meio:\n\n");
	printf("1. Trotiente\n");
	printf("2. Bicicleta\n\n");
	int opcao;
	scanf("%d", &opcao);

	if (opcao == 1) {
		strcpy(novo_meio->tipo, "Trotiente");
	}
	else if (opcao == 2) {
		strcpy(novo_meio->tipo, "Bicicleta");
	}
	else {
		printf("Opcao invalida. O tipo do meio sera definido como vazio.\n");
		strcpy(novo_meio->tipo, "");
	}
	printf("\nDigite o nome do meio: ");
	scanf("%s", novo_meio->nome);

	printf("Digite o custo do meio: ");
	scanf("%f", &novo_meio->custo);

	printf("Digite a bateria do meio: ");
	scanf("%f", &novo_meio->bateria);

	printf("\nLocalizacoes:\n\n");
	// Mostra a lista de vértices e pede ao usuário o ID
	vertice* v = (*g)->vertices;
	imprimirVertices(*g);

	printf("\nDigite o ID correspondente a localizacao do meio: ");
	scanf("%d", &id_vert);

	// Encontra o vértice com o ID escolhido e copia o respetivo nome e geocodigo
	v = (*g)->vertices;
	while (v != NULL) {
		if (v->id == id_vert) {
			strcpy(novo_meio->local, v->local_meio);
			strcpy(novo_meio->local_grafo, v->nome);
			break;
		}
		v = v->seguinte;
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

	fprintf(f, "%d %s %s %.2f %.2f %s %s %d\n", novo_meio->id, novo_meio->nome, novo_meio->tipo, novo_meio->custo,
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
	printf("Meio registado com sucesso!\n");
	getchar();
}

// Função para passar os meios para uma lista ligada
void lerMeios(FILE* f, meio** head) {
	f = fopen("meios.txt", "r");
	if (f == NULL) {
		printf("Erro ao abrir arquivo!\n");
		getchar();
		exit(1);
	}
	meio* current = NULL;
	while (!feof(f)) {
		meio* new_meio = (meio*)malloc(sizeof(meio));
		fscanf(f, "%d %s %s %f %f %s %s %d\n", &(new_meio->id), new_meio->nome, new_meio->tipo, &(new_meio->custo),
			&(new_meio->bateria), new_meio->local, new_meio->local_grafo, &(new_meio->reserva));
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
	*f = fopen("meios.txt", "w");
	meio* curr_meio = head_meio;

	// Escreve os conteúdos de cada categoria
	while (curr_meio != NULL) {
		fprintf(*f, "%d %s %s %.2f %.2f %s %s %d\n", curr_meio->id, curr_meio->nome, curr_meio->tipo, curr_meio->custo,
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
	// Abre o ficheiro de meios
	FILE* txt_meio = fopen("meios.txt", "r");
	if (txt_meio == NULL) {
		printf("Erro ao abrir arquivo!\n");
		exit(1);
	}

	// Cria o array de meios
	meio m[100];
	int count = 0;
	while (fscanf(txt_meio, "%d %s %s %f %f %s %s %d\n", &m[count].id, m[count].nome, m[count].tipo, &m[count].custo,
		&m[count].bateria, m[count].local, m[count].local_grafo, &m[count].reserva) != EOF) {
		count++;
	}

	// Fecha o ficheiro
	fclose(txt_meio);

	// Ordena por bateria
	if (order_by == 'b') {
		for (int i = 0; i < count - 1; i++) {
			for (int j = 0; j < count - i - 1; j++) {
				if (m[j].bateria < m[j + 1].bateria) {
					meio temp = m[j];
					m[j] = m[j + 1];
					m[j + 1] = temp;
				}
			}
		}
		printf("\nLista de meios ordenada por bateria:\n\n");
	}
	else if (order_by == 'c') {
		// Ordena por custo
		for (int i = 0; i < count - 1; i++) {
			for (int j = 0; j < count - i - 1; j++) {
				if (m[j].custo < m[j + 1].custo) {
					meio temp = m[j];
					m[j] = m[j + 1];
					m[j + 1] = temp;
				}
			}
		}
		printf("\nLista de meios ordenada por custo:\n\n");
	}
	else {
		printf("Opcao de ordenacao invalida.\n");
		return;
	}
	// Mostra a lista de meios
	printf("ID | Nome      | Tipo      | Custo  | Bateria  | Local             | Geocodigo                    | Reserva\n");
	printf("---+-----------+-----------+--------+----------+-------------------+------------------------------+---------\n");
	for (int i = 0; i < count; i++) {
		printf("%-3d| %-10s| %-10s| %-7.2f| %-9.2f| %-18s| %-29s| %-6d\n",
			m[i].id, m[i].nome, m[i].tipo, m[i].custo, m[i].bateria, m[i].local, m[i].local_grafo, m[i].reserva);
	}
	getchar();
}

// Função para remover um meio
void removerMeio() {
	int id;

	// Abrir ficheiros em modo de escrita e leitura
	FILE* txt_file = fopen("meios.txt", "r");
	FILE* bin_file = fopen("meios.bin", "ab+");

	// Ler todos os meios para uma lista ligada
	meio* head = NULL;
	meio* curr = NULL;
	lerMeios(txt_file, &head);
	fclose(txt_file);

	// Mostra a lista de meios
	printf("ID | Nome      | Tipo      | Custo  | Bateria  | Local             | Geocodigo                    | Reserva\n");
	printf("---+-----------+-----------+--------+----------+-------------------+------------------------------+---------\n");
	curr = head;
	while (curr != NULL) {
		printf("%-3d| %-10s| %-10s| %-7.2f| %-9.2f| %-18s| %-29s| %-6d\n",
			curr->id, curr->nome, curr->tipo, curr->custo, curr->bateria, curr->local, curr->local_grafo, curr->reserva);
		curr = curr->seguinte;
	}

	// Pedir ao user o ID do meio a remover
	printf("\nDigite o ID do meio a remover: ");
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
		atualizarBinMeio(&bin_file, head);

		system("clear || cls");
		printf("Meio com ID %d removido com sucesso!\n", id);
		getchar();
	}
}

// Função para alterar um meio
void alterarMeio(grafo** g) {
	*g = carregarGrafo();
	int id;
	int id_vert;
	// Abrir ficheiro em modo leitura
	FILE* file = fopen("meios.txt", "r");
	FILE* bin_file = fopen("meios.bin", "ab+");

	// Ler todos os meios para uma lista ligada
	meio* head = NULL;
	meio* curr = NULL;
	lerMeios(file, &head);
	fclose(file);

	// Mostra a lista de meios
	printf("ID | Nome      | Tipo      | Custo  | Bateria  | Local             | Geocodigo                    | Reserva\n");
	printf("---+-----------+-----------+--------+----------+-------------------+------------------------------+---------\n");
	curr = head;
	while (curr != NULL) {
		printf("%-3d| %-10s| %-10s| %-7.2f| %-9.2f| %-18s| %-29s| %-6d\n",
			curr->id, curr->nome, curr->tipo, curr->custo, curr->bateria, curr->local, curr->local_grafo, curr->reserva);
		curr = curr->seguinte;
	}

	// Pedir ao user o ID do meio a alterar
	printf("\nDigite o ID do meio a alterar: ");
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
			printf("\nDigite o campo a alterar (nome, tipo, custo, bateria, local, geocodigo ou reserva), ou 'fim' para terminar: ");
			scanf("%s", campo);
			if (strcmp(campo, "fim") == 0) {
				break;
			}
			if (strcmp(campo, "local") == 0 || strcmp(campo, "geocodigo") == 0) {
				printf("\n");
				// Mostra a lista de vértices e pede ao usuário o ID
				vertice* v = (*g)->vertices;
				imprimirVertices(*g);
				printf("\nDigite o ID correspondente a localizacao do meio: ");
				scanf("%d", &id_vert);

				// Encontra o vértice com o ID escolhido e copia o respetivo nome
				v = (*g)->vertices;
				while (v != NULL) {
					if (v->id == id_vert) {
						strcpy(curr->local, v->local_meio);
						strcpy(curr->local_grafo, v->nome);
						break;
					}
					v = v->seguinte;
				}
			}
			else if (strcmp(campo, "tipo") == 0) {
				printf("\nDigite o novo tipo de meio:\n\n");
				printf("1. Bicicleta\n");
				printf("2. Trotinete\n\n");
				int tipo_opcao;
				scanf("%d", &tipo_opcao);

				if (tipo_opcao == 1) {
					strcpy(curr->tipo, "Bicicleta");
				}
				else if (tipo_opcao == 2) {
					strcpy(curr->tipo, "Trotinete");
				}
				else {
					printf("\nOpcao invalida. O campo nao sera alterado.\n");
				}
			}
			else {
				printf("\nDigite o novo valor: ");
				scanf("%s", novo_valor);

				if (strcmp(campo, "nome") == 0) {
					strcpy(curr->nome, novo_valor);
				}
				else if (strcmp(campo, "custo") == 0) {
					curr->custo = atof(novo_valor);
				}
				else if (strcmp(campo, "bateria") == 0) {
					curr->bateria = atof(novo_valor);
				}
				else if (strcmp(campo, "reserva") == 0) {
					curr->reserva = atoi(novo_valor);
				}
			}

		} while (strcmp(campo, "fim") != 0);

		// atualizar os meios no ficheiro
		atualizarMeio(&file, head);

		// atualizar os meios no ficheiro
		atualizarBinMeio(&bin_file, head);

		system("clear || cls");
		printf("Meio com ID %d alterado com sucesso!\n", id);
		getchar();

	}
}
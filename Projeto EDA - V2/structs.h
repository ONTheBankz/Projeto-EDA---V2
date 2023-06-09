#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct cliente {
	int id;
	int nif;
	char nome[50];
	char utilizador[50];
	char password[50];
	char morada[50];
	float saldo;
	struct cliente* seguinte;
} cliente;

typedef struct gestor {
	int id;
	char nome[50];
	char utilizador[50];
	char password[50];
	struct gestor* seguinte;
} gestor;

typedef struct meio {
	int id;
	int reserva;
	float custo;
	float bateria;
	char nome[50];
	char tipo[50];
	char local[50];
	char local_grafo[50];
	struct meio* seguinte;
} meio;

typedef struct registo {
	int id;
	int cliente_id;
	int meio_id;
	int dia;
	int mes;
	int ano;
	int horas;
	int minutos;
	struct registo* seguinte;
} registo;

typedef struct aresta {
	int id_origem;      // id do v�rtice de origem
	int id_destino;     // id do v�rtice de destino
	int peso;
	struct aresta* proxima;
} aresta;

typedef struct vertice {
	int id;
	char nome[50];
	char local_meio[50];
	meio* meios;
	aresta* arestas;
	struct vertice* seguinte;
} vertice;

typedef struct grafo {
	int num_vertices;
	int num_arestas;
	vertice* vertices;
} grafo;

// FUN��ES CLIENTE
void registarCliente(cliente** head);
cliente* lerCliente(FILE* f);
void lerClientes(FILE* f, cliente** head);
void atualizarCliente(FILE** f, cliente* head);
void atualizarBinCliente(FILE** f, cliente* head);
void loginCliente(cliente** head, meio** headM, registo** headR, grafo** headV, aresta** headA);
void listarCliente();
void removerCliente();
void alterarCliente();
void listarAluguerCliente(id_cliente);
void listarMeioCliente(char order_by);
void listarHistoricoCliente(int id_cliente);
void terminarAluguer(id_cliente);
void mostrarMeiosCliente(grafo* g, meio* m, char** nomesVertices, int numVertices, const char* tipo_meio);
void carregarSaldo(id_cliente);

// FUN��ES GESTOR
void registarGestor(gestor** head);
gestor* lerGestor(FILE* f);
void lerGestores(FILE* f, gestor** head);
void atualizarGestor(FILE** f, gestor* head);
void atualizarBinGestor(FILE** f, gestor* head);
void loginGestor(gestor** head, meio** headM, registo** headR, grafo** headV, aresta** headA);
void listarGestor();
void removerGestor();
void alterarGestor();
void carregarSaldoGestor();
void registarAluguerGestor(registo** headR);

// FUN��ES MEIO
void registarMeio(meio** head, grafo** g);
void listarMeio(char order_by);
void lerMeios(FILE* f, meio** head);
void atualizarMeio(FILE** f, meio* head_meio);
void atualizarBinMeio(FILE** f, meio* head_meio);
void removerMeio();
void alterarMeio(grafo** g);

// FUN��ES GRAFO
grafo* criarGrafo();
aresta* criarConexao();
grafo* carregarGrafo();
aresta* carregarAresta();
vertice* buscarVertice(grafo* g, int id);
void salvarGrafo(grafo* g);
void criarVertice(grafo* g);
void editarVertice(grafo* g);
void removerVertice(grafo* g);
void imprimirVertices(grafo* g);
void atualizarVertices(vertice* vertices);
char* obterNomeVertice(grafo* g, int id_vertice);
void mostrarMeios(grafo* g, meio* m, char** nomesVertices, int numVertices, const char* tipo_meio);
void criarAresta(grafo* g);
void salvarAresta(aresta* a);
void removerAresta(aresta* a);
void imprimirAresta(aresta* a);
void atualizarAresta(aresta* a);
void verConexoesRaio(grafo* g, aresta* a, meio* m, int caller);
void encontrarConexoes(grafo* g, aresta* a, meio* m, int id_origem, float raio, const char* tipo_meio, int caller);
void imprimirConexao(int id_origem, int destino, float distancia, int* caminho, int tamanho);


// FUN��ES ALUGUER
void registarAluguer(int id_cliente, registo** head);
void salvarAluguer(registo* new_registo);
int encontraID();
void lerAluguer(FILE* f, registo** head);
void atualizarAluguer(FILE** f, registo* head_registo);
void atualizarBinAluguer(FILE** f, registo* head_registo);
void listarAluguer();
void listarHistorico();
void cancelarAluguer();

// FUN��ES MENU
int fileExiste(const char* filename);
void showMenu(cliente** headC, gestor** headG, meio** headM, registo** headR, grafo** headV, aresta** headA);
void showMenuGestor(meio** headM, registo** headR, grafo** headV, aresta** headA);
void showMenuCliente(registo** headR, meio** headM, grafo** headV, aresta** headA);

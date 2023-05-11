
typedef struct cliente {
    int id;
    int nif;
    char nome[50];
    char utilizador[50];
    char password[50];
    char morada[50];
    char local_grafo[50];
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
    int id_origem;      // id do vértice de origem
    int id_destino;     // id do vértice de destino
    int peso;
    float distancia;    // distância entre os vértices
    struct aresta* proxima;
} aresta;

typedef struct vertice {
    int id;
    char nome[50];
    meio* meios;
    aresta* arestas;    // arestas adjacentes
    struct vertice* seguinte;
} vertice;

typedef struct grafo {
    int num_vertices;
    int num_arestas;
    vertice* vertices;
} grafo;


// FUNÇÕES CLIENTE
void registarCliente(cliente** head);
cliente* lerCliente(FILE* f);
void lerClientes(FILE* f, cliente** head);
void atualizarCliente(FILE** f, cliente* head);
void atualizarBinCliente(FILE** f, cliente* head);
void loginCliente(cliente** head, registo** headR);
void listarCliente();
void removerCliente();
void alterarCliente();
void listarAluguerCliente(id_cliente);
void listarMeioCliente(char order_by);
void terminarAluguer(id_cliente);
void carregarSaldo(id_cliente);

// FUNÇÕES GESTOR
void registarGestor(gestor** head);
gestor* lerGestor(FILE* f);
void lerGestores(FILE* f, gestor** head);
void atualizarGestor(FILE** f, gestor* head);
void atualizarBinGestor(FILE** f, gestor* head);
void loginGestor(gestor** head, meio** headM);
void listarGestor();
void removerGestor();
void alterarGestor();
void carregarSaldoGestor();
void registarAluguerGestor(registo** headR);

// FUNÇÕES MEIO
void registarMeio(meio** head);
void listarMeio(char order_by);
void lerMeios(FILE* f, meio** head);
void atualizarMeio(FILE** f, meio* head_meio);
void atualizarBinMeio(FILE** f, meio* head_meio);
void removerMeio();
void alterarMeio();

// FUNÇÕES GRAFO
grafo* criarGrafo();
grafo* carregarGrafo(const char* nome_arquivo);
void conectarVertices(grafo* g, int id_origem, int id_destino, int peso, float distancia);
void salvarGrafo(const grafo* g, const char* nome_arquivo);

// FUNÇÕES ALUGUER
void registarAluguer(int id_cliente, registo** head);
void lerAluguer(FILE* f, registo** head);
void atualizarAluguer(FILE** f, registo* head_registo);
void atualizarBinAluguer(FILE** f, registo* head_registo);
void listarAluguer();
void cancelarAluguer();

// FUNÇÕES MENU
void showMenu(cliente** headC, gestor** headG, meio** headM, registo** headR);
void showMenuGestor(meio** headM, registo** headR);
void showMenuCliente(registo** headR);

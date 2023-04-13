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
    char distancia[50];
    char tipo[50];
    char local[50];
    struct meio* seguinte;
} meio;

typedef struct registo {
    int id;
    int cliente_id;
    int meio_id;
    char data[50];
    struct registo* seguinte;
} registo;

// FUN합ES CLIENTE
void registarCliente(cliente** head);
cliente* lerCliente(FILE* f);
void loginCliente(cliente** head, registo** headR);
void remover_cliente();
void listar_cliente();
void alterar_cliente();
void listar_aluguer_cliente(id_cliente);
void listar_meio_cliente(char order_by);

// FUN합ES GESTOR
void registarGestor(gestor** head);
gestor* lerGestor(FILE* f);
void loginGestor(gestor** head, meio** headM);
void listar_gestor();
void remover_gestor();
void alterar_gestor();
void listar_aluguer_gestor();

// FUN합ES MEIO
void registarMeio(meio** head);
void remover_meio();
void alterar_meio();
void listar_meio(char order_by);

// FUN합ES ALUGUER
void registar_aluguer(int id_cliente, registo** head);
int gerar_id_registo();
int existe_id_registo(int id);

// FUN합ES MENU
void showMenu(cliente** headC, gestor** headG, meio** headM, registo** headR);
void showMenuGestor(meio** headM);
void showMenuCliente(registo** headR);

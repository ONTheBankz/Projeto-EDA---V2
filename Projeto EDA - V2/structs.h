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
    int dia;
    int mes;
    int ano;
    int horas;
    int minutos;
    struct registo* seguinte;
} registo;

// FUN합ES CLIENTE
void registarCliente(cliente** head);
cliente* lerCliente(FILE* f);
void loginCliente(cliente** head, registo** headR);
void listarCliente();
void removerCliente();
void alterarCliente();
void listarAluguerCliente(id_cliente);
void listarMeioCliente(char order_by);
void terminarAluguer(id_cliente);
void carregarSaldo(id_cliente);

// FUN합ES GESTOR
void registarGestor(gestor** head);
gestor* lerGestor(FILE* f);
void loginGestor(gestor** head, meio** headM);
void listarGestor();
void removerGestor();
void alterarGestor();
void carregarSaldoGestor();
void registarAluguerGestor(registo** headR);

// FUN합ES MEIO
void registarMeio(meio** head);
void listarMeio(char order_by);
void removerMeio();
void alterarMeio();

// FUN합ES ALUGUER
void RegistarAluguer(int id_cliente, registo** head);
void ListarAluguer();
void CancelarAluguer();

// FUN합ES MENU
void showMenu(cliente** headC, gestor** headG, meio** headM, registo** headR);
void showMenuGestor(meio** headM, registo** headR);
void showMenuCliente(registo** headR);

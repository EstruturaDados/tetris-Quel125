#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================== DEFINIÇÕES ==================

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define NUM_TIPOS_PECAS 4

// Estrutura para representar uma peça do Tetris
struct Peca {
    char nome;     // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;        // Identificador único
};

// Estrutura para representar a fila circular
struct Fila {
    struct Peca pecas[TAMANHO_FILA];  // Array para armazenar as peças
    int frente;                       // Índice da frente da fila
    int traseira;                     // Índice da traseira da fila
    int tamanho;                      // Número atual de peças na fila
    int proximoId;                    // Próximo ID a ser atribuído
};

// Estrutura para representar a pilha de reserva
struct Pilha {
    struct Peca pecas[TAMANHO_PILHA];  // Array para armazenar as peças reservadas
    int topo;                          // Índice do topo da pilha
    int tamanho;                       // Número atual de peças na pilha
};

// ================== PROTÓTIPOS DE FUNÇÕES ==================

void inicializarFila(struct Fila *fila);
void inicializarPilha(struct Pilha *pilha);
struct Peca gerarPeca(int id);
void exibirEstado(struct Fila *fila, struct Pilha *pilha);
void jogarPeca(struct Fila *fila, struct Pilha *pilha);
void reservarPeca(struct Fila *fila, struct Pilha *pilha);
void usarPecaReservada(struct Fila *fila, struct Pilha *pilha);
void exibirMenu();

// ================== FUNÇÕES DE INICIALIZAÇÃO ==================

// Função para inicializar a fila com peças iniciais
void inicializarFila(struct Fila *fila) {
    fila->frente = 0;
    fila->traseira = TAMANHO_FILA - 1;  // Inicialmente vazia
    fila->tamanho = 0;
    fila->proximoId = 0;
    
    // Gerar 5 peças iniciais automaticamente
    for (int i = 0; i < TAMANHO_FILA; i++) {
        // Calcular nova posição da traseira (circularmente)
        fila->traseira = (fila->traseira + 1) % TAMANHO_FILA;
        
        // Gerar e inserir a nova peça
        struct Peca novaPeca = gerarPeca(fila->proximoId);
        fila->pecas[fila->traseira] = novaPeca;
        
        fila->tamanho++;
        fila->proximoId++;
    }
}

// Função para inicializar a pilha de reserva
void inicializarPilha(struct Pilha *pilha) {
    pilha->topo = -1;  // Pilha vazia
    pilha->tamanho = 0;
}

// Função para gerar uma nova peça aleatória
struct Peca gerarPeca(int id) {
    struct Peca novaPeca;
    
    // Tipos de peças disponíveis no Tetris
    char tipos[NUM_TIPOS_PECAS] = {'I', 'O', 'T', 'L'};
    
    // Gerar tipo aleatório
    int indiceAleatorio = rand() % NUM_TIPOS_PECAS;
    novaPeca.nome = tipos[indiceAleatorio];
    novaPeca.id = id;
    
    return novaPeca;
}

// ================== FUNÇÕES DE EXIBIÇÃO ==================

// Função para exibir o estado atual da fila e pilha
void exibirEstado(struct Fila *fila, struct Pilha *pilha) {
    printf("\n=======================================\n");
    printf("      TETRIS STACK - ESTADO ATUAL      \n");
    printf("=======================================\n");
    
    // Exibir fila de peças futuras
    printf("\n--- FILA DE PEÇAS FUTURAS ---\n");
    printf("Peças na fila: %d/%d\n", fila->tamanho, TAMANHO_FILA);
    
    if (fila->tamanho == 0) {
        printf("Fila vazia\n");
    } else {
        printf("Ordem: ");
        
        // Percorrer a fila circularmente a partir da frente
        int i = fila->frente;
        int contador = 0;
        
        while (contador < fila->tamanho) {
            printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
            
            // Avançar circularmente
            i = (i + 1) % TAMANHO_FILA;
            contador++;
        }
        
        // Mostrar qual peça está na frente
        printf("\nPróxima peça: [%c %d]\n", 
               fila->pecas[fila->frente].nome, 
               fila->pecas[fila->frente].id);
    }
    
    // Exibir pilha de peças reservadas
    printf("\n--- PILHA DE RESERVA ---\n");
    printf("Peças na pilha: %d/%d\n", pilha->tamanho, TAMANHO_PILHA);
    
    if (pilha->tamanho == 0) {
        printf("Pilha vazia\n");
    } else {
        printf("Topo -> Base: ");
        
        // Exibir do topo para a base (da última para a primeira)
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
        
        // Mostrar qual peça está no topo
        printf("\nPeça no topo: [%c %d]\n", 
               pilha->pecas[pilha->topo].nome, 
               pilha->pecas[pilha->topo].id);
    }
    
    printf("=======================================\n");
}

// ================== FUNÇÕES DE MANIPULAÇÃO ==================

// Função para jogar uma peça (remover da frente da fila)
void jogarPeca(struct Fila *fila, struct Pilha *pilha) {
    if (fila->tamanho == 0) {
        printf("\nERRO: A fila está vazia! Não há peças para jogar.\n");
        return;
    }
    
    struct Peca pecaJogada = fila->pecas[fila->frente];
    
    printf("\n>>> JOGANDO PEÇA: [%c %d] <<<\n", 
           pecaJogada.nome, pecaJogada.id);
    
    // Remover da frente (avançar circularmente)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    printf("Peça jogada com sucesso!\n");
    
    // Gerar e adicionar nova peça ao final da fila (manter fila sempre cheia)
    if (fila->tamanho < TAMANHO_FILA) {
        fila->traseira = (fila->traseira + 1) % TAMANHO_FILA;
        struct Peca novaPeca = gerarPeca(fila->proximoId);
        fila->pecas[fila->traseira] = novaPeca;
        fila->tamanho++;
        fila->proximoId++;
        
        printf("Nova peça gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
    }
}

// Função para reservar uma peça (mover da frente da fila para o topo da pilha)
void reservarPeca(struct Fila *fila, struct Pilha *pilha) {
    if (fila->tamanho == 0) {
        printf("\nERRO: A fila está vazia! Não há peças para reservar.\n");
        return;
    }
    
    if (pilha->tamanho >= TAMANHO_PILHA) {
        printf("\nERRO: A pilha de reserva está cheia (%d/%d)!\n", 
               pilha->tamanho, TAMANHO_PILHA);
        printf("Use uma peça reservada primeiro ou jogue uma peça.\n");
        return;
    }
    
    struct Peca pecaReservada = fila->pecas[fila->frente];
    
    printf("\n>>> RESERVANDO PEÇA: [%c %d] <<<\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Remover da frente da fila
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    // Adicionar ao topo da pilha
    pilha->topo++;
    pilha->pecas[pilha->topo] = pecaReservada;
    pilha->tamanho++;
    
    printf("Peça movida para a pilha de reserva.\n");
    
    // Gerar e adicionar nova peça ao final da fila (manter fila sempre cheia)
    if (fila->tamanho < TAMANHO_FILA) {
        fila->traseira = (fila->traseira + 1) % TAMANHO_FILA;
        struct Peca novaPeca = gerarPeca(fila->proximoId);
        fila->pecas[fila->traseira] = novaPeca;
        fila->tamanho++;
        fila->proximoId++;
        
        printf("Nova peça gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
    }
}

// Função para usar uma peça reservada (remover do topo da pilha)
void usarPecaReservada(struct Fila *fila, struct Pilha *pilha) {
    if (pilha->tamanho == 0) {
        printf("\nERRO: A pilha de reserva está vazia!\n");
        printf("Reserve uma peça primeiro.\n");
        return;
    }
    
    struct Peca pecaUsada = pilha->pecas[pilha->topo];
    
    printf("\n>>> USANDO PEÇA RESERVADA: [%c %d] <<<\n", 
           pecaUsada.nome, pecaUsada.id);
    
    // Remover do topo da pilha
    pilha->topo--;
    pilha->tamanho--;
    
    printf("Peça reservada usada com sucesso!\n");
    
    // NOTA: Ao usar uma peça reservada, NÃO geramos nova peça na fila
    // Apenas a removemos da pilha
}

// ================== FUNÇÃO DO MENU ==================

void exibirMenu() {
    printf("\n=======================================\n");
    printf("          MENU DE AÇÕES               \n");
    printf("=======================================\n");
    printf("1. Jogar peça (remove da frente da fila)\n");
    printf("2. Reservar peça (fila -> pilha)\n");
    printf("3. Usar peça reservada (remove da pilha)\n");
    printf("0. Sair do jogo\n");
    printf("=======================================\n");
    printf("Digite sua escolha: ");
}

// ================== FUNÇÃO PRINCIPAL ==================

int main() {
    // Inicializar semente para números aleatórios
    srand(time(NULL));
    
    struct Fila filaPecas;
    struct Pilha pilhaReserva;
    
    printf("=======================================\n");
    printf("   TETRIS STACK - CONTROLE AVANÇADO    \n");
    printf("   Fila + Pilha de Reserva            \n");
    printf("=======================================\n");
    
    // Inicializar estruturas
    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);
    
    printf("\nSistema inicializado!\n");
    printf("- Fila de peças: %d posições\n", TAMANHO_FILA);
    printf("- Pilha de reserva: %d posições\n", TAMANHO_PILHA);
    
    int opcao;
    
    do {
        // Exibir estado atual
        exibirEstado(&filaPecas, &pilhaReserva);
        
        // Exibir menu de opções
        exibirMenu();
        
        // Ler opção do usuário
        if (scanf("%d", &opcao) != 1) {
            // Limpar buffer em caso de entrada inválida
            while (getchar() != '\n');
            printf("\nEntrada inválida! Digite 0, 1, 2 ou 3.\n");
            continue;
        }
        
        // Processar opção escolhida
        switch(opcao) {
            case 0:
                printf("\n=======================================\n");
                printf("   OBRIGADO POR JOGAR TETRIS STACK!    \n");
                printf("   Pontuação final:                    \n");
                printf("   - Peças jogadas: %d\n", filaPecas.proximoId - filaPecas.tamanho);
                printf("   - Peças reservadas usadas: %d\n", pilhaReserva.tamanho);
                printf("=======================================\n");
                break;
                
            case 1:
                jogarPeca(&filaPecas, &pilhaReserva);
                break;
                
            case 2:
                reservarPeca(&filaPecas, &pilhaReserva);
                break;
                
            case 3:
                usarPecaReservada(&filaPecas, &pilhaReserva);
                break;
                
            default:
                printf("\nOpção inválida! Digite 0, 1, 2 ou 3.\n");
        }
        
        // Pausa para melhor visualização
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpar buffer
            getchar(); // Aguardar Enter
        }
        
    } while (opcao != 0);
    
    return 0;
}
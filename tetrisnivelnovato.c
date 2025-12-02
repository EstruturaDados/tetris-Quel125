#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================== DEFINIÇÕES ==================

#define TAMANHO_FILA 5
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

// ================== PROTÓTIPOS DE FUNÇÕES ==================

void inicializarFila(struct Fila *fila);
struct Peca gerarPeca(int id);
void exibirFila(struct Fila *fila);
void jogarPeca(struct Fila *fila);
void inserirPeca(struct Fila *fila);
void exibirMenu();

// ================== FUNÇÕES PRINCIPAIS ==================

// Função para inicializar a fila com peças iniciais
void inicializarFila(struct Fila *fila) {
    fila->frente = 0;
    fila->traseira = TAMANHO_FILA - 1;  // Inicialmente vazia
    fila->tamanho = 0;
    fila->proximoId = 0;
    
    // Gerar 5 peças iniciais automaticamente
    for (int i = 0; i < TAMANHO_FILA; i++) {
        inserirPeca(fila);
    }
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

// Função para exibir o estado atual da fila
void exibirFila(struct Fila *fila) {
    printf("\n=== FILA DE PEÇAS FUTURAS ===\n");
    printf("Peças na fila: %d/%d\n\n", fila->tamanho, TAMANHO_FILA);
    
    if (fila->tamanho == 0) {
        printf("A fila está vazia!\n");
        return;
    }
    
    printf("Ordem de peças (da frente para o final):\n");
    
    // Percorrer a fila circularmente a partir da frente
    int i = fila->frente;
    int contador = 0;
    
    while (contador < fila->tamanho) {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        
        // Avançar circularmente
        i = (i + 1) % TAMANHO_FILA;
        contador++;
    }
    printf("\n");
    
    // Mostrar qual peça está na frente
    if (fila->tamanho > 0) {
        printf("\nPróxima peça a ser jogada: [%c %d]\n", 
               fila->pecas[fila->frente].nome, 
               fila->pecas[fila->frente].id);
    }
}

// Função para jogar uma peça (remover da frente da fila)
void jogarPeca(struct Fila *fila) {
    if (fila->tamanho == 0) {
        printf("\nERRO: A fila está vazia! Não há peças para jogar.\n");
        return;
    }
    
    struct Peca pecaJogada = fila->pecas[fila->frente];
    
    printf("\n>>> JOGANDO PEÇA: [%c %d] <<<\n", pecaJogada.nome, pecaJogada.id);
    
    // Remover da frente (avançar circularmente)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    printf("Peça removida da fila com sucesso!\n");
    
    // Inserir automaticamente uma nova peça no final (se houver espaço)
    if (fila->tamanho < TAMANHO_FILA) {
        printf("Gerando nova peça para o final da fila...\n");
        inserirPeca(fila);
    }
}

// Função para inserir uma nova peça no final da fila
void inserirPeca(struct Fila *fila) {
    if (fila->tamanho >= TAMANHO_FILA) {
        printf("\nERRO: A fila está cheia! Não é possível adicionar mais peças.\n");
        printf("Jogue uma peça para liberar espaço.\n");
        return;
    }
    
    // Calcular nova posição da traseira (circularmente)
    fila->traseira = (fila->traseira + 1) % TAMANHO_FILA;
    
    // Gerar e inserir a nova peça
    struct Peca novaPeca = gerarPeca(fila->proximoId);
    fila->pecas[fila->traseira] = novaPeca;
    
    fila->tamanho++;
    fila->proximoId++;
    
    printf("\n>>> NOVA PEÇA ADICIONADA: [%c %d] <<<\n", novaPeca.nome, novaPeca.id);
    printf("Peça inserida no final da fila.\n");
}

// Função para exibir o menu de opções
void exibirMenu() {
    printf("\n=======================================\n");
    printf("        TETRIS STACK - CONTROLE        \n");
    printf("=======================================\n");
    printf("1. Jogar peça (dequeue - remove da frente)\n");
    printf("2. Inserir nova peça (enqueue - adiciona no final)\n");
    printf("0. Sair do jogo\n");
    printf("=======================================\n");
    printf("Digite sua escolha: ");
}

// ================== FUNÇÃO PRINCIPAL ==================

int main() {
    // Inicializar semente para números aleatórios
    srand(time(NULL));
    
    struct Fila filaPecas;
    
    printf("=======================================\n");
    printf("   BEM-VINDO AO TETRIS STACK!          \n");
    printf("   Sistema de Controle de Peças        \n");
    printf("=======================================\n");
    
    // Inicializar a fila com 5 peças
    inicializarFila(&filaPecas);
    printf("\nFila inicializada com %d peças.\n", TAMANHO_FILA);
    
    int opcao;
    
    do {
        // Exibir estado atual da fila
        exibirFila(&filaPecas);
        
        // Exibir menu de opções
        exibirMenu();
        
        // Ler opção do usuário
        if (scanf("%d", &opcao) != 1) {
            // Limpar buffer em caso de entrada inválida
            while (getchar() != '\n');
            printf("\nEntrada inválida! Digite 0, 1 ou 2.\n");
            continue;
        }
        
        // Processar opção escolhida
        switch(opcao) {
            case 0:
                printf("\n=======================================\n");
                printf("   OBRIGADO POR JOGAR TETRIS STACK!    \n");
                printf("   Encerrando o sistema...             \n");
                printf("=======================================\n");
                break;
                
            case 1:
                jogarPeca(&filaPecas);
                break;
                
            case 2:
                inserirPeca(&filaPecas);
                break;
                
            default:
                printf("\nOpção inválida! Digite 0, 1 ou 2.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}
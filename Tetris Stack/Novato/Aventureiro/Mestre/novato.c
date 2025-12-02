#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// ------------------------------
// Struct da peça
// ------------------------------
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// ------------------------------
// Fila circular
// ------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ------------------------------
// Protótipos
// ------------------------------
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
Peca gerarPeca();
void exibirFila(Fila *f);

// ------------------------------
// Função principal
// ------------------------------
int main() {
    Fila fila;
    int opcao;
    int proximoID = 0;

    srand(time(NULL));

    inicializarFila(&fila);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        nova.id = proximoID++;
        enqueue(&fila, nova);
    }

    do {
        printf("\n===== Fila de Peças =====\n");
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if (filaVazia(&fila)) {
                    printf("\nA fila está vazia! Não há peças para jogar.\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                }
                break;

            case 2:
                if (filaCheia(&fila)) {
                    printf("\nA fila está cheia! Não é possível inserir nova peça.\n");
                } else {
                    Peca nova = gerarPeca();
                    nova.id = proximoID++;
                    enqueue(&fila, nova);
                    printf("\nPeça gerada e inserida: [%c %d]\n", nova.nome, nova.id);
                }
                break;

            case 0:
                printf("\nEncerrando programa...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ------------------------------
// Inicializa fila
// ------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

// ------------------------------
// Verifica se está cheia
// ------------------------------
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

// ------------------------------
// Verifica se está vazia
// ------------------------------
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// ------------------------------
// Insere peça na fila
// ------------------------------
void enqueue(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// ------------------------------
// Remove peça da fila
// ------------------------------
Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// ------------------------------
// Gera uma peça aleatória
// ------------------------------
Peca gerarPeca() {
    Peca p;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];  // Seleciona uma peça aleatória
    return p;
}

// ------------------------------
// Exibe a fila inteira
// ------------------------------
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }

    int idx = f->inicio;

    for (int i = 0; i < f->quantidade; i++) {
        Peca p = f->itens[idx];
        printf("[%c %d] ", p.nome, p.id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("\n");
}

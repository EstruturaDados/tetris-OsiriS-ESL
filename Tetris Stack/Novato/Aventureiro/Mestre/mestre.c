#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// -------------------------------------------------
// STRUCT DE PEÇA (Tetris Stack)
// -------------------------------------------------
typedef struct {
    char nome;  // Tipo: I, O, T, L
    int id;     // Identificador único
} Peca;

// -------------------------------------------------
// FILA CIRCULAR
// -------------------------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// -------------------------------------------------
// PILHA LINEAR
// -------------------------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// -------------------------------------------------
// PROTÓTIPOS
// -------------------------------------------------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);

int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);

void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);

void push(Pilha *p, Peca pc);
Peca pop(Pilha *p);

Peca gerarPeca(int id);

void exibirFila(Fila *f);
void exibirPilha(Pilha *p);

// Trocas solicitadas
void trocarPeçaAtual(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

// -------------------------------------------------
// PROGRAMA PRINCIPAL
// -------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, proximoID = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca(proximoID++));

    do {
        printf("\n=======================================\n");
        printf("             ESTADO ATUAL\n");
        printf("=======================================\n");

        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da fila\n");
        printf("2 - Reservar peça (fila → pilha)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("4 - Trocar peça atual (fila ↔ pilha)\n");
        printf("5 - Troca múltipla (3 da fila ↔ 3 da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: { // Jogar peça
                if (filaVazia(&fila)) {
                    printf("\nA fila está vazia!\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(proximoID++));
                }
            } break;

            case 2: { // Reservar peça
                if (filaVazia(&fila))
                    printf("\nFila vazia!\n");
                else if (pilhaCheia(&pilha))
                    printf("\nPilha cheia! Não é possível reservar.\n");
                else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("\nPeça reservada: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca(proximoID++));
                }
            } break;

            case 3: { // Usar peça reservada
                if (pilhaVazia(&pilha))
                    printf("\nPilha vazia!\n");
                else {
                    Peca usada = pop(&pilha);
                    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                }
            } break;

            case 4: // Trocar peça atual
                trocarPeçaAtual(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// -------------------------------------------------
// FUNÇÕES BÁSICAS DE FILA E PILHA
// -------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) { return f->quantidade == 0; }
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

void enqueue(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

void push(Pilha *p, Peca pe) {
    p->itens[++p->topo] = pe;
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

// -------------------------------------------------
// GERAR PEÇA ALEATÓRIA
// -------------------------------------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// -------------------------------------------------
// VISUALIZAÇÕES
// -------------------------------------------------
void exibirFila(Fila *f) {
    printf("Fila: ");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--)
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    printf("\n");
}

// -------------------------------------------------
// TROCA DA PEÇA ATUAL (fila ↔ pilha)
// -------------------------------------------------
void trocarPeçaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Não é possível trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Não há peça para trocar.\n");
        return;
    }

    int idx_frente = f->inicio;
    int idx_topo = p->topo;

    // Troca direta
    Peca temp = f->itens[idx_frente];
    f->itens[idx_frente] = p->itens[idx_topo];
    p->itens[idx_topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha!\n");
}

// -------------------------------------------------
// TROCA MÚLTIPLA (3 primeiras da fila ↔ 3 da pilha)
// -------------------------------------------------
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\nNão há peças suficientes para troca múltipla!\n");
        return;
    }

    printf("\nTrocando 3 peças entre fila e pilha...\n");

    for (int i = 0; i < 3; i++) {
        int idx_fila = (f->inicio + i) % TAM_FILA;
        int idx_pilha = p->topo - i;

        Peca temp = f->itens[idx_fila];
        f->itens[idx_fila] = p->itens[idx_pilha];
        p->itens[idx_pilha] = temp;
    }

    printf("Troca múltipla concluída!\n");
}

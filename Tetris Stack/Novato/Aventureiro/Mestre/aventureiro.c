#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// --------------------------------------
// Struct que representa uma peça do jogo
// --------------------------------------
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// --------------------------------------
// Fila circular
// --------------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// --------------------------------------
// Pilha linear
// --------------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// --------------------------------------
// Prototipação das funções
// --------------------------------------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);

void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);

void push(Pilha *p, Peca pc);
Peca pop(Pilha *p);

Peca gerarPeca(int id);

void exibirFila(Fila *f);
void exibirPilha(Pilha *p);

// --------------------------------------
// Programa principal
// --------------------------------------
int main() {
    Fila fila;
    Pilha pilha;

    int opcao;
    int proximoID = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(proximoID++));
    }

    do {
        printf("\n========================================\n");
        printf("            ESTADO ATUAL\n");
        printf("========================================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {

            // Jogar peça (remove da fila)
            case 1:
                if (filaVazia(&fila)) {
                    printf("\nA fila está vazia! Nada para jogar.\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);

                    // Gera nova peça para manter a fila sempre cheia
                    enqueue(&fila, gerarPeca(proximoID++));
                }
                break;

            // Reservar peça (fila -> pilha)
            case 2:
                if (filaVazia(&fila)) {
                    printf("\nA fila está vazia! Nada para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nA pilha de reserva está cheia! Não é possível reservar mais.\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\nPeça reservada: [%c %d]\n", reservada.nome, reservada.id);

                    // Repor peça na fila
                    enqueue(&fila, gerarPeca(proximoID++));
                }
                break;

            // Usar peça reservada (pop)
            case 3:
                if (pilhaVazia(&pilha)) {
                    printf("\nA pilha está vazia! Nenhuma peça reservada disponível.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
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

// --------------------------------------
// Inicialização da fila e pilha
// --------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// --------------------------------------
// Verificações
// --------------------------------------
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// --------------------------------------
// Operações da fila (enqueue / dequeue)
// --------------------------------------
void enqueue(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// --------------------------------------
// Operações da pilha (push / pop)
// --------------------------------------
void push(Pilha *p, Peca peca) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = peca;
    }
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

// --------------------------------------
// Geração aleatória de peças
// --------------------------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// --------------------------------------
// Exibição da fila e pilha
// --------------------------------------
void exibirFila(Fila *f) {
    printf("\nFila de peças: ");
    int idx = f->inicio;

    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");

    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

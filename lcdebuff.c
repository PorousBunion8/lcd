#include "buffer.h"
#include "genlib.h"
#include <stdio.h>
#include <stdlib.h>

/*** Tipos de Dados ***/

// Tipo: celulaTCD
// Define uma célula da lista circular duplamente encadeada
typedef struct celulaTCD {
    char letra;                // Caractere armazenado
    struct celulaTCD *proximo; // Ponteiro para a próxima célula
    struct celulaTCD *anterior; // Ponteiro para a célula anterior
} celulaTCD;

typedef struct celulaTCD *celulaTAD;

// Tipo: bufferTCD
// Representa o buffer, com uma célula "dummy" e um cursor
struct bufferTCD {
    celulaTAD dummy;  // Célula sentinela
    celulaTAD cursor; // Posição atual no buffer
};

/*** Funções Privadas ***/

// cria uma nova célula
static celulaTAD criar_celula(void) {
    celulaTAD nova_celula = (celulaTAD)malloc(sizeof(celulaTCD));
    if (nova_celula == NULL) {
        fprintf(stderr, "Erro: não foi possível alocar memória para célula.\n");
        exit(1);
    }
    nova_celula->proximo = nova_celula->anterior = nova_celula; // Célula se referencia
    return nova_celula;
}

// remove uma célula da memória
static void remover_celula(celulaTAD *celula) {
    if (celula != NULL && *celula != NULL) {
        free(*celula);
        *celula = NULL;
    }
}

/*** Funções Expostas ***/

// cria e inicializa um buffer vazio
bufferTAD criar_buffer(void) {
    bufferTAD novo_buffer = (bufferTAD)malloc(sizeof(struct bufferTCD));
    if (novo_buffer == NULL) {
        fprintf(stderr, "Erro: não foi possível alocar memória para o buffer.\n");
        exit(1);
    }
    novo_buffer->dummy = criar_celula(); // Cria célula "dummy"
    novo_buffer->cursor = novo_buffer->dummy; // Inicializa cursor
    return novo_buffer;
}

// libera a memória alocada para o buffer e suas células
void liberar_buffer(bufferTAD *buffer) {
    if (buffer == NULL || *buffer == NULL) {
        fprintf(stderr, "Erro: buffer inválido.\n");
        return;
    }
    celulaTAD atual = (*buffer)->dummy->proximo;
    while (atual != (*buffer)->dummy) { // Percorre todas as células
        celulaTAD temp = atual;
        atual = atual->proximo;
        remover_celula(&temp); // Remove célula atual
    }
    remover_celula(&(*buffer)->dummy); // Remove célula "dummy"
    free(*buffer);
    *buffer = NULL;
}

// insere um caractere após o cursor
void inserir_caractere(bufferTAD buffer, char c) {
    if (buffer == NULL) {
        fprintf(stderr, "Erro: buffer inválido.\n");
        exit(1);
    }
    celulaTAD nova_celula = criar_celula(); // Cria nova célula
    nova_celula->letra = c; // Armazena o caractere
    nova_celula->proximo = buffer->cursor->proximo; // Atualiza ponteiros
    nova_celula->anterior = buffer->cursor;
    buffer->cursor->proximo->anterior = nova_celula;
    buffer->cursor->proximo = nova_celula;
    buffer->cursor = nova_celula; // Move cursor
}

// apaga o caractere após o cursor
void apagar_caractere(bufferTAD buffer) {
    if (buffer == NULL || buffer->cursor->proximo == buffer->dummy) {
        return; // Nada para apagar
    }
    celulaTAD apagar = buffer->cursor->proximo;
    buffer->cursor->proximo = apagar->proximo; // Atualiza ponteiros
    apagar->proximo->anterior = buffer->cursor;
    remover_celula(&apagar); // Libera célula
}

// move o cursor uma posição para frente
void mover_cursor_para_frente(bufferTAD buffer) {
    if (buffer != NULL && buffer->cursor->proximo != buffer->dummy) {
        buffer->cursor = buffer->cursor->proximo;
    }
}

// move o cursor uma posição para trás
void mover_cursor_para_tras(bufferTAD buffer) {
    if (buffer != NULL && buffer->cursor->anterior != buffer->dummy) {
        buffer->cursor = buffer->cursor->anterior;
    }
}

// move o cursor para o início do buffer
void mover_cursor_para_inicio(bufferTAD buffer) {
    if (buffer != NULL) {
        buffer->cursor = buffer->dummy->proximo;
    }
}

// move o cursor para o final do buffer
void mover_cursor_para_final(bufferTAD buffer) {
    if (buffer != NULL) {
        buffer->cursor = buffer->dummy->anterior;
    }
}

// exibe o conteúdo do buffer no terminal
void exibir_buffer(bufferTAD buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "Erro: buffer inválido.\n");
        return;
    }
    celulaTAD temp;
    for (temp = buffer->dummy->proximo; temp != buffer->dummy; temp = temp->proximo) {
        printf(" %c", temp->letra); // Exibe caractere
    }
    printf("\n");
    for (temp = buffer->dummy; temp != buffer->cursor; temp = temp->proximo) {
        printf("  "); // Indica posição do cursor
    }
    printf("^\n");
}


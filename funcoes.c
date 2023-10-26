#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Cliente {
    char cpf[16];
    char nome[100];
    char tipo_de_conta[10];
    float saldo;
    char senha[5];
};

struct Extrato {
    char data[11];
    char cpf[16];
    char descricao[100];
    float valor;
    float tarifa;
};

void novo_cliente() {
    FILE *arquivo;
    struct Cliente cliente;
    int existe = 0;

    // Abre o arquivo binário para leitura
    arquivo = fopen("Clientes.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


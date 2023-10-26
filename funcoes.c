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

    printf("Criar novo cliente:\n\n");

    printf("CPF: ");
    fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
    cliente.cpf[strcspn(cliente.cpf, "\n")] = '\0';
    
    // Verifica o tipo de conta
    if (strcmp(cliente.tipo_de_conta, "1") == 0) {
        strcpy(cliente.tipo_de_conta, "Comum");
    } else if (strcmp(cliente.tipo_de_conta, "2") == 0) {
        strcpy(cliente.tipo_de_conta, "Plus");
    } else {
        printf("\nTipo de conta inválido\n");
        return;
    }

    printf("Nome: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

    printf("Tipo de conta (1-Comum, 2-Plus): ");
    fgets(cliente.tipo_de_conta, sizeof(cliente.tipo_de_conta), stdin);
    cliente.tipo_de_conta[strcspn(cliente.tipo_de_conta, "\n")] = '\0';

    printf("Valor inicial: ");
    scanf("%f" , &cliente.saldo);
    getchar();

    printf("Digite a senha: ");
    fgets(cliente.senha, sizeof(cliente.senha), stdin);
    cliente.senha[strcspn(cliente.senha, "\n")] = '\0';


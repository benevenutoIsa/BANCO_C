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

void excluirCliente(char cpf[16]) {
    FILE *arquivo;
    struct Cliente cliente;
    int encontrado = 0;

    // Abre o arquivo original para leitura e escrita em modo binário
    arquivo = fopen("Clientes.bin", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    // Verifica se o CPF está no formato correto
    if (strlen(cpf) != 14 || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') {
        printf("\nCPF invalido!\n");
        fclose(arquivo);
        return;
    }

    // Procura pelo CPF no arquivo original
    while (fread(&cliente, sizeof(cliente), 1, arquivo)) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        // Move o ponteiro do arquivo para a posição anterior ao cliente encontrado
        fseek(arquivo, -sizeof(cliente), SEEK_CUR);
        // Preenche o espaço do cliente com zeros
        memset(&cliente, 0, sizeof(cliente));
        // Escreve os zeros no lugar do cliente no arquivo
        fwrite(&cliente, sizeof(cliente), 1, arquivo);
        printf("\nCliente removido!\n");
    } else {
        printf("\nInexistente\n");
    }

    fclose(arquivo);
}

void listar_clientes() {
    FILE *arquivo;
    struct Cliente cliente;

    // Abre o arquivo binário para leitura
    arquivo = fopen("Clientes.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Lista de Clientes:\n\n");

    // Lê o primeiro cliente do arquivo
    fread(&cliente, sizeof(struct Cliente), 1, arquivo);

    // Lista todos os clientes existentes e todas as suas informações 
    while (!feof(arquivo)) {
        if (strcmp(cliente.cpf, "") != 0) {
            printf("CPF: %s\n", cliente.cpf);
            printf("Nome: %s\n", cliente.nome);
            printf("Tipo: %s\n", cliente.tipo_de_conta);
            printf("Saldo: %.2f\n", cliente.saldo);
            printf("Senha: %s\n\n", cliente.senha);
        }

        // Lê o próximo cliente
        fread(&cliente, sizeof(struct Cliente), 1, arquivo);
    }

    fclose(arquivo);
}


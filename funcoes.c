// Created by:
// Isabela Benevenuto R.A.: 22.123
// Kayky Pires R.A.: 22.222.040-2
// Rafael Dias R.A.: 22.222.039-4

#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

struct Cliente {
    char cpf[16];
    char nome[100];
    char tipo_de_conta[10];
    float saldo;
    char senha[5];
};

#define MAX_STRING 100

void novo_cliente(char cpf[16]) {
    FILE *arquivo;
    struct Cliente cliente;
    int encontrado = 0;
    // Abre o arquivo binário para leitura
    arquivo = fopen("Clientes.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Copia o CPF informado para a struct cliente
    strcpy(cliente.cpf, cpf);

    // Valida o CPF
    if (strlen(cliente.cpf) != 14 || cliente.cpf[3] != '.' || cliente.cpf[7] != '.' || cliente.cpf[11] != '-') {
        printf("\nCPF invalido!\n");
        return;
    }

    // Verifica se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {
        encontrado = 0;
    } else {
        // Volta para o início do arquivo
        rewind(arquivo);

        // Procura pelo CPF no arquivo original
        char cpf_arquivo[16];
        struct Cliente temp_cliente;
        while (fread(&temp_cliente, sizeof(temp_cliente), 1, arquivo)) {
            strcpy(cpf_arquivo, temp_cliente.cpf);
            if (strcmp(cpf_arquivo, cpf) == 0) {
                encontrado = 1;
                break;
            }
        }
    }

    fclose(arquivo);
    if(encontrado == 1){
        printf("Cliente existente");
        return;
    } else {

        printf("Nome: ");
        fgets(cliente.nome, sizeof(cliente.nome), stdin);
        cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

        printf("Tipo de conta (1-Comum, 2-Plus): ");
        fgets(cliente.tipo_de_conta, sizeof(cliente.tipo_de_conta), stdin);
        cliente.tipo_de_conta[strcspn(cliente.tipo_de_conta, "\n")] = '\0';

        // Verifica o tipo de conta
        if (strcmp(cliente.tipo_de_conta, "1") == 0) {
            strcpy(cliente.tipo_de_conta, "Comum");
        } else if (strcmp(cliente.tipo_de_conta, "2") == 0) {
            strcpy(cliente.tipo_de_conta, "Plus");
        } else {
            printf("\nTipo de conta invalido\n");
            return;
        }

        printf("Valor inicial: ");
        scanf("%f" , &cliente.saldo);
        getchar();

        printf("Digite a senha (APENAS 4 NUMEROS): ");
        fgets(cliente.senha, sizeof(cliente.senha), stdin);
        cliente.senha[strcspn(cliente.senha, "\n")] = '\0';

        // Abre o arquivo binário para escrita em modo de adição
        arquivo = fopen("Clientes.bin", "ab");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return;
        }

        // Escreve o cliente no arquivo
        fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);

        fclose(arquivo);

        printf("\nNovo cliente criado com sucesso!\n");
    }
    }

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

    // Lista todos os clientes existentes
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

void deposito(char cpf[16]){
    //abre os arquivos
    FILE *arquivo;
    FILE *extrato_arquivo;
    
    struct Cliente cliente;

    // Variáveis de data e hora
    time_t t = time(NULL);
    struct tm *data_hora_atual = localtime(&t);
    char data_e_hora_em_texto[20];
    strftime(data_e_hora_em_texto, sizeof(data_e_hora_em_texto), "%d/%m/%Y %H:%M", data_hora_atual);

    // Abre o arquivo para leitura e escrita
    arquivo = fopen("Clientes.bin", "rb+");
    //Verifica se o arquivo existe, caso nao exista exibe a mensagem de erro.
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Verifica se o cliente existe
    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo)) {
        if (strcmp(cliente.cpf, cpf) == 0) {

            //variavel do valor a ser depositado
            float valor;

            printf("Valor para Depósito: ");
            scanf("%f", &valor);

            //Altera o saldo do cliente fazendo o deposito
            cliente.saldo += valor;
        
            // Faz as alterações do arquivo
            fseek(arquivo, -sizeof(struct Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);

            // Fecha o arquivo de clientes
            fclose(arquivo);

            //abre o arquivo de extrato
            extrato_arquivo = fopen("Extrato.bin", "ab");
            //Gera o extrato no arquivo de extrato
            fprintf(extrato_arquivo, "Data: %s | Deposito: + %.2f | Saldo: %.2f _ %s\n", data_e_hora_em_texto, valor, cliente.saldo, cpf);
            //fecha o arquivo de extrato
            fclose(extrato_arquivo);


            printf("\nValor depositado!\n");
            return;
        }
    }

    //Caso o cliente não exista , apresentará essa mensagem.
    printf("\nCPF inválido!\n");

    // Fecha o arquivo
    fclose(arquivo);
}

void transferencia(char cpf_origem[16], char senha_origem[5], char cpf_destino[16]){
    FILE *arquivo;
    struct Cliente cliente;
    struct Cliente cliente_origem, cliente_destino;


    // Variáveis de data e hora
    time_t t = time(NULL);
    struct tm *data_hora_atual = localtime(&t);
    char data_e_hora_em_texto[20];
    strftime(data_e_hora_em_texto, sizeof(data_e_hora_em_texto), "%d/%m/%Y %H:%M", data_hora_atual);

    float valor;

    printf("Digite o valor da transferencia: ");
    scanf("%f", &valor);


    // Abre o arquivo para leitura e escrita
    arquivo = fopen("Clientes.bin", "r+");
    //verifica se o arquivo existe, caso não exista, aparecerá uma mensagem de erro
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //variaveis para Verificar se os CPFs de origem e destino existem
    int origem_encontrado = 0;
    int destino_encontrado = 0;
  
    // Encontre o cliente de origem e destino
    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo)) {
        if (strcmp(cliente.cpf, cpf_origem) == 0 && strcmp(cliente.senha, senha_origem) == 0) {
            cliente_origem = cliente;
            origem_encontrado = 1;
        } else if (strcmp(cliente.cpf, cpf_destino) == 0) {
            cliente_destino = cliente;
            destino_encontrado = 1;
        }
    }
    //Caso não exista cliente de origem ou senha estejaminvalida , aparecerá erro
    if (!origem_encontrado) {
        printf("\nCPF de origem ou senha inválida!\n");
        fclose(arquivo);
        return;
    }
    //Caso cliente de destino não exista , aparecerá erro
    if (!destino_encontrado) {
        printf("\nCPF de destino inválido!\n");
        fclose(arquivo);
        return;
    }
  
    // Verifica se a conta de origem tem saldo suficiente
    if (cliente_origem.saldo < valor) {
        printf("\nSaldo insuficiente!\n");
        fclose(arquivo);
        return;
    }
  
    // Atualiza os saldos das contas de origem e destino
    cliente_origem.saldo -= valor;
    cliente_destino.saldo += valor;
  
    // Fecha o arquivo
    fclose(arquivo);
  
    // Abre um arquivo temporário para gravar os dados atualizados
    FILE *temp_arquivo = fopen("Clientes_temp.bin", "w");
  
    // Volte ao início do arquivo original
    arquivo = fopen("Clientes.bin", "r");
  
    // Copie os registros para o arquivo temporário com as atualizações
    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo)) {
        if (strcmp(cliente.cpf, cpf_origem) == 0) {
            fwrite(&cliente_origem, sizeof(struct Cliente), 1, temp_arquivo);
        } else if (strcmp(cliente.cpf, cpf_destino) == 0) {
            fwrite(&cliente_destino, sizeof(struct Cliente), 1, temp_arquivo);
        } else {
            fwrite(&cliente, sizeof(struct Cliente), 1, temp_arquivo);
        }
    }
  
    // Fecha ambos os arquivos
    fclose(arquivo);
    fclose(temp_arquivo);
  
    // Renomeie o arquivo temporário para o nome do arquivo original
    remove("Clientes.bin");
    rename("Clientes_temp.bin", "Clientes.bin");
}


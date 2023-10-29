// Created by:
// Isabela Benevenuto R.A.: 22.123
// Kayky Pires R.A.: 22.222.040-2
// Rafael Dias R.A.: 22.222.039-4

//Bibliotecas usadas
#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


//Struct de clientes
struct Cliente {
    char cpf[16];
    char nome[100];
    char tipo_de_conta[10];
    float saldo;
    char senha[5];
};

#define MAX_STRING 100

//Função de novo cliente
void novo_cliente(char cpf[16]) {
    FILE *arquivo;
    struct Cliente cliente;
    int encontrado = 0;
    // Abre o arquivo para leitura
    arquivo = fopen("Clientes.bin", "rb");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
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
    //informa se o cliente existe
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

        printf("Digite a senha: ");
        fgets(cliente.senha, sizeof(cliente.senha), stdin);
        cliente.senha[strcspn(cliente.senha, "\n")] = '\0';

        // Abre o arquivo binário para escrita em modo de adição
        arquivo = fopen("Clientes.bin", "ab");
        //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
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


//Função para excluir o cliente
void excluirCliente(char cpf[16]) {
    FILE *arquivo;
    struct Cliente cliente;
    int encontrado = 0;

    // Abre o arquivo original para leitura e escrita em modo binário
    arquivo = fopen("Clientes.bin", "rb+");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
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

//função para listar os clientes
void listar_clientes() {
    FILE *arquivo;
    struct Cliente cliente;

    // Abre o arquivo binário para leitura
    arquivo = fopen("Clientes.bin", "rb");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
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


//função para debito
void debito(char cpf[16], char senha[5]) {
    FILE *arquivo;
    struct Cliente cliente;

    // Variáveis de data e hora
    time_t t = time(NULL);
    struct tm *data_hora_atual = localtime(&t);
    char data_e_hora_em_texto[20];
    strftime(data_e_hora_em_texto, sizeof(data_e_hora_em_texto), "%d/%m/%Y %H:%M", data_hora_atual);

    // Abre o arquivo para leitura e escrita binária
    arquivo = fopen("Clientes.bin", "rb+");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fread(&cliente, sizeof(cliente), 1, arquivo)) {
        //verifica se o cliente e a senha estão corretos
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha,senha) == 0) {

            float valor;

            printf("Valor para Debito: ");
            scanf("%f", &valor);

            // Verifica se a conta é Comum e tem saldo suficiente
            if (strcmp(cliente.tipo_de_conta, "Comum") == 0 && cliente.saldo - valor < -1000) {
                printf("\nSaldo insuficiente\n");
                fclose(arquivo);
                return;
            }

                // Verifica se a conta é Plus e tem saldo suficiente
            else if (strcmp(cliente.tipo_de_conta, "Plus") == 0 && cliente.saldo - valor < -5000) {
                printf("\nSaldo insuficiente\n");
                fclose(arquivo);
                return;
            }

            else {
                // Débito para conta Comum
                if (strcmp(cliente.tipo_de_conta, "Comum") == 0) {
                    // Tarifa de 5%
                    float tarifa = valor * 0.05;
                    // Debita na conta do cliente
                    cliente.saldo -= valor + tarifa;

                    // Cria o extrato
                    FILE *extrato_arquivo;

                    //abre arquivo para adiconar o extrato
                    extrato_arquivo = fopen("Extrato.bin", "ab");
                    //adiciona o extrato no arquivo
                    fprintf(extrato_arquivo, "Data: %s | Debito: - %.2f |Tarifa: %.2f | Saldo: %.2f _ %s\n", data_e_hora_em_texto, valor, tarifa, cliente.saldo, cpf);
                    fclose(extrato_arquivo);

                    // Faz alteração no arquivo
                    fseek(arquivo, -sizeof(struct Cliente), SEEK_CUR);
                    fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);

                    // Fecha o arquivo
                    fclose(arquivo);

                    printf("\nValor debitado!\n");
                    return;
                }

                    // Débito para conta Plus
                else if (strcmp(cliente.tipo_de_conta, "Plus") == 0) {
                    // Tarifa de 3%
                    float tarifa = valor * 0.03;
                    // Debita na conta do cliente
                    cliente.saldo -= valor + tarifa;

                    // Cria o extrato
                    FILE *extrato_arquivo;

                    //abre o arquivo para adicionar o extrato
                    extrato_arquivo = fopen("Extrato.bin", "ab");
                    //adiciona o extrato no arquivo
                    fprintf(extrato_arquivo, "Data: %s | Debito: - %.2f |Tarifa: %.2f | Saldo: %.2f _ %s\n", data_e_hora_em_texto, valor, tarifa, cliente.saldo, cpf);
                    fclose(extrato_arquivo);

                    // Faz alteração no arquivo
                    fseek(arquivo, -sizeof(struct Cliente), SEEK_CUR);
                    fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);

                    // Fecha o arquivo
                    fclose(arquivo);

                    printf("\nValor debitado!\n");
                    return;
                }
            }

        }
    }

    printf("\nCPF ou senha inválida!\n");

    // Fecha o arquivo
    fclose(arquivo);
}


//função para deposito
void deposito(char cpf[16]) {
    FILE *arquivo, *extrato_arquivo;
    struct Cliente cliente;

    // Variáveis de data e hora
    time_t t = time(NULL);
    struct tm *data_hora_atual = localtime(&t);
    char data_e_hora_em_texto[20];
    strftime(data_e_hora_em_texto, sizeof(data_e_hora_em_texto), "%d/%m/%Y %H:%M", data_hora_atual);


    // Abre o arquivo para leitura e escrita
    arquivo = fopen("Clientes.bin", "rb+");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Verifica se o cliente existe
    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo)) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            float valor;

            printf("Valor para Depósito: ");
            scanf("%f", &valor);

            // Deposita na conta do cliente
            cliente.saldo += valor;

            // Faz alteração no arquivo
            fseek(arquivo, -sizeof(struct Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);

            // Fecha o arquivo
            fclose(arquivo);

            // abre o arquivo para adicionar o extrato
            extrato_arquivo = fopen("Extrato.bin", "ab");
            //adiciona o extrato no arquivo
            fprintf(extrato_arquivo, "Data: %s | Deposito: + %.2f | Saldo: %.2f _ %s\n", data_e_hora_em_texto, valor, cliente.saldo, cpf);
            fclose(extrato_arquivo);

            printf("\nValor depositado!\n");
            return;
        }
    }

    printf("\nCPF inválido!\n");

    // Fecha o arquivo
    fclose(arquivo);
}


//função para transferencia
void transferencia(char cpf_origem[16], char senha_origem[5], char cpf_destino[16]) {
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
    //Verifica se o arquivo existe, caso não exista aparecerá uma mensagem de erro
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Verifica se os CPFs de origem e destino existem
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
    
    //verifica se o cliente de origem existe
    if (!origem_encontrado) {
        printf("\nCPF de origem ou senha inválida!\n");
        fclose(arquivo);
        return;
    }
    
    //verifica se o cliente de destino existe
    if (!destino_encontrado) {
        printf("\nCPF de destino inválido!\n");
        fclose(arquivo);
        return;
    }
  
    // Verifica se a conta de origem é Comum e tem saldo suficiente
    if (strcmp(cliente_origem.tipo_de_conta ,"Comum") == 0) {
      if (cliente_origem.saldo - valor < -1000){
        printf("\nSaldo insuficiente!\n");
        fclose(arquivo);
        return;
      }
    }

    // Verifica se a conta de origem é Plus e tem saldo suficiente
    if (strcmp(cliente_origem.tipo_de_conta, "Plus") == 0) {
      if (cliente_origem.saldo - valor < -5000){
        printf("\nSaldo insuficiente!\n");
        fclose(arquivo);
        return;
      }
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

    // Cria o extrato da operação od cliente de origem e destino
    FILE *extrato_arquivo;
    //Abre o arquivo para adicionar os extratos
    extrato_arquivo = fopen("Extrato.bin", "ab");
    //Extrato do cliente de origem
    fprintf(extrato_arquivo, "Data: %s | Tranferencia: - %.2f | Saldo:  %.2f _ %s\n", data_e_hora_em_texto, valor, cliente_origem.saldo, cpf_origem);

    //Extrato do cliente de destino
    fprintf(extrato_arquivo, "Data: %s | Tranferencia: + %.2f | Saldo:  %.2f _ %s\n", data_e_hora_em_texto, valor, cliente_destino.saldo, cpf_destino);

    //fecha o arquivo
    fclose(extrato_arquivo);

    printf("\nTransferência realizada com sucesso!\n");
}

void extratos(char cpf[16], char senha[5]) {
    FILE *arquivo_cliente;
    FILE *arquivo_extrato;
    struct Cliente cliente;

    //Abre o arquivo para leitura
    arquivo_cliente = fopen("Clientes.bin", "rb");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
    if (arquivo_cliente == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    //abre o arquivo para leitura
    arquivo_extrato = fopen("Extrato.bin", "rb");
    //verifica se o arquivo existe, caso não exista aparecerá mensagem de erro
    if (arquivo_extrato == NULL) {
        printf("Erro ao abrir o arquivo de extrato.\n");
        fclose(arquivo_cliente);
        return;
    }

    char linha[MAX_STRING];

    printf("\nExtratos:\n\n");

    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo_cliente)) {
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            while (fgets(linha, MAX_STRING, arquivo_extrato) != NULL) {
                // Verifica se a linha contém o CPF
                if (strstr(linha, cpf) != NULL) {
                    // Encontra a posição do caractere '_' na linha
                    char *underline_pos = strchr(linha, '_');
                    if (underline_pos != NULL) {
                        // Calcula o tamanho da substring a ser impressa
                        size_t len_to_print = underline_pos - linha;
                        // Imprime a parte relevante da linha
                        printf("%.*s\n", (int)len_to_print, linha);
                    }
                }
            }

            fclose(arquivo_extrato);
            fclose(arquivo_cliente);
            return;
        }
    }

    fclose(arquivo_cliente);
    fclose(arquivo_extrato);
    return;
}

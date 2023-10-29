// Created by:
// Isabela Benevenuto R.A.: 22.123.007-1
// Kayky Pires R.A.: 22.222.040-2
// Rafael Dias R.A.: 22.222.039-4

#include <stdio.h>
#include "funcoes.h"
#include <string.h>

struct Cliente {
    char cpf[16];
    char nome[100];
    char tipo_de_conta[10];
    float saldo;
    char senha[5];
};

int main() {

    FILE *arquivo_clientes, *arquivo_extrato;

    arquivo_clientes = fopen("Clientes.bin", "ab");
    if (arquivo_clientes == NULL) {
        printf("Erro ao criar o arquivo de clientes.\n");
        return 1; // Retorna um código de erro se a criação do arquivo falhar
    }
    fclose(arquivo_clientes);

    arquivo_extrato = fopen("Extrato.bin", "ab");
    if (arquivo_extrato == NULL) {
        printf("Erro ao criar o arquivo de extrato.\n");
        return 1; // Retorna um código de erro se a criação do arquivo falhar
    }
    fclose(arquivo_extrato);

    char cpf[16];
    char senha[5];
    char cpf_origem[16];
    char cpf_destino[16];
    char senha_origem[5];
    float valor;

    int x;

    printf("Bem-vindo ao sistema bancario!\n");

    do {
        printf("\n1 - Novo Cliente\n");
        printf("2 - Excluir Cliente\n");
        printf("3 - Listar Clientes\n");
        printf("4 - Debitos\n");
        printf("5 - Deposito\n");
        printf("6 - Tranferencia\n");
        printf("7 - Extratos\n");
        printf("8 - Sair\n");
        printf("\nEscolha a operacao: ");
        scanf("%d", &x);
        printf("\n");
        getchar();

        switch (x) {
            case 1:

                printf("Criar novo cliente:\n\n");

                printf("Digite o CPF (Digite com ponto e barra: ***.***.***-**): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                novo_cliente(cpf);
                getchar();
                break;
            case 2:

                printf("Excluir Cliente!\n");

                printf("Digite o CPF (Digite com ponto e barra: ***.***.***-**): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                excluirCliente(cpf);
                break;
            case 3:
                listar_clientes();
                break;
            case 4:

                printf("Débitos!\n");


                printf("Digite o CPF (Digite com ponto e barra: ***.***.***-**): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';

                debito(cpf,senha);
                break;
            case 5:

                printf("Deposito!\n");

                printf("Digite o CPF (Digite com ponto e barra: ***.***.***-**): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                deposito(cpf);
                break;
            case 6:

                printf("Transferencia!\n");

                printf("Digite o CPF de origem: ");
                fgets(cpf_origem, sizeof(cpf_origem), stdin);
                cpf_origem[strcspn(cpf_origem, "\n")] = '\0';


                printf("Digite a senha da conta de origem: ");
                fgets(senha_origem, sizeof(senha_origem), stdin);
                senha_origem[strcspn(senha_origem, "\n")] = '\0';
                getchar();

                printf("Digite o CPF de destino: ");
                fgets(cpf_destino, sizeof(cpf_destino), stdin);
                cpf_destino[strcspn(cpf_destino, "\n")] = '\0';


                transferencia(cpf_origem,senha_origem,cpf_destino);
                break;
            case 7:

                printf("Extrato\n");

                printf("Digite o CPF (Digite com ponto e barra: ***.***.***-**): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';

                extratos(cpf,senha);
                break;
            case 8:
                printf("\nFinalizando...\n");
                break;
            default:
                printf("\nOpcao invalida\n");
                break;
        }
    } while (x != 8);

    return 0;
}


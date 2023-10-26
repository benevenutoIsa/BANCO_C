#include <stdio.h>
#include "funcoes.h"

int main() {

    int x;
    while (1){
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

        getchar();

        if(x == 1){
            novo_cliente();
            getchar();
        } else if(x == 2) {
            excluirCliente();
        } else if (x == 3){
            listar_clientes();
        } else if (x == 7){
            extratos();
        }else if(x == 8){
            printf("\nFinalizando...\n");
            break;
        }else {
            printf("\nOpcao invalida\n");
        }

    }

}
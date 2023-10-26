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


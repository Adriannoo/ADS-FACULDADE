#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validarEntrada(char entrada[]) {
    int temPontoOuVirgula = 0;

    // Verifica se há caracteres inválidos ou número negativo
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] == ',') {
            entrada[i] = '.'; // Converte ',' para '.'
            temPontoOuVirgula++;
        } else if (entrada[i] == '.') {
            temPontoOuVirgula++;
        } else if (!isdigit(entrada[i])) { // Se não for número
            return 0; // Entrada inválida
        }
    }

    // Impede múltiplos pontos/vírgulas (número inválido como 12.3.4)
    if (temPontoOuVirgula > 1) {
        return 0;
    }

    return 1; // Entrada válida
}

int main() {
    char entrada[50];
    float numero;

    while (1) {
        printf("Digite um número positivo (use ',' ou '.'): ");
        scanf("%49s", entrada); // Lê como string para validar primeiro

        // Se a entrada for válida, converte para float
        if (validarEntrada(entrada) && sscanf(entrada, "%f", &numero) == 1 && numero >= 0) {
            printf("Número válido: %.2f\n", numero);
            break; // Sai do loop se for um número positivo válido
        } else {
            printf("Entrada inválida! Digite apenas números positivos.\n");
        }
    }

    return 0;
}

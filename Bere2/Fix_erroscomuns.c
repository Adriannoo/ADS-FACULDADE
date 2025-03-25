#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validarEntrada(char entrada[]) {
    int temPontoOuVirgula = 0;

    // Verifica se h� caracteres inv�lidos ou n�mero negativo
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] == ',') {
            entrada[i] = '.'; // Converte ',' para '.'
            temPontoOuVirgula++;
        } else if (entrada[i] == '.') {
            temPontoOuVirgula++;
        } else if (!isdigit(entrada[i])) { // Se n�o for n�mero
            return 0; // Entrada inv�lida
        }
    }

    // Impede m�ltiplos pontos/v�rgulas (n�mero inv�lido como 12.3.4)
    if (temPontoOuVirgula > 1) {
        return 0;
    }

    return 1; // Entrada v�lida
}

int main() {
    char entrada[50];
    float numero;

    while (1) {
        printf("Digite um n�mero positivo (use ',' ou '.'): ");
        scanf("%49s", entrada); // L� como string para validar primeiro

        // Se a entrada for v�lida, converte para float
        if (validarEntrada(entrada) && sscanf(entrada, "%f", &numero) == 1 && numero >= 0) {
            printf("N�mero v�lido: %.2f\n", numero);
            break; // Sai do loop se for um n�mero positivo v�lido
        } else {
            printf("Entrada inv�lida! Digite apenas n�meros positivos.\n");
        }
    }

    return 0;
}

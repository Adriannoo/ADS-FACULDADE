
#include <stdio.h>

int x, y;

void trocar(int *a, int *b) {
 printf("A soma eh: %d\n" , (*a + *b));
}


int main() {
    x = 5;
    y = 10;
    trocar(&x, &y);
    printf("X: %d Y: %d", x, y);

}


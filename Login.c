#include <stdio.h>
char login[50];
char senha[50];

void login(){
    printf("Login: ");
    fgets(login, sizeof(login), stdin);
    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);

    login[strcspn(login, "\n")] = '\0';
    senha[strcspn(senha, "\n")] = '\0';

    if (strcmp(login, "vendedor1") == 0 && strcmp(senha, "101010") == 0) {
        printf("\nLogin realizado com sucesso!!!");
    } else {
        printf("\nLogin e/ou senha invalidos!!!");
    }
}

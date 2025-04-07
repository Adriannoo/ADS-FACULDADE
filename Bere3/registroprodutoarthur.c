#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int produtosCadastrados;
bool registro = false;

struct produto{
int codigoProduto;
char nomeProduto[50];
float valorProduto;
int estoqueProduto;
};
produtosCadastrados = 1;

struct produto limpeza[100] = {{11,"Detergente",1.99,50}};
struct produto alimentos[100] = {{21,"Cafe",19.99,10}};
struct produto padaria[100] = {{31,"Pao de forma PCT",9.50}};

void registrarProduto(){
    int codigoCategoria;
    system("cls");
    printf("Registro de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    printf("4 - Voltar\n");
    printf("Selecione a categoria: ");
    scanf("%d", &codigoCategoria);

    switch(codigoCategoria){
        case 1:
                printf("Informe o codigo do produto: ");
                    scanf("%d", &limpeza[produtosCadastrados].codigoProduto);
                getchar();
                printf("Informe o nome do produto: ");
                    fgets(limpeza[produtosCadastrados].nomeProduto, sizeof(limpeza[produtosCadastrados].nomeProduto), stdin);
                    limpeza[produtosCadastrados].nomeProduto[strcspn(limpeza[produtosCadastrados].nomeProduto, "\n")] = '\0';
                printf("Informe o valor do produto: ");
                    scanf("%f", &limpeza[produtosCadastrados].valorProduto);
                printf("Informe o estoque do produto: ");
                    scanf("%d", &limpeza[produtosCadastrados].estoqueProduto);
                    registro = true;
                        if (registro = true)
                            produtosCadastrados++;
                printf("Produtos registrado com sucesso!!!");
                sleep(2);
                registrarProduto();
                break;

        case 2:
                printf("Informe o codigo do produto: ");
                    scanf("%d", &alimentos[produtosCadastrados].codigoProduto);
                getchar();
                printf("Informe o nome do produto: ");
                    fgets(alimentos[produtosCadastrados].nomeProduto, sizeof(alimentos[produtosCadastrados].nomeProduto), stdin);
                    alimentos[produtosCadastrados].nomeProduto[strcspn(alimentos[produtosCadastrados].nomeProduto, "\n")] = '\0';
                printf("Informe o valor do produto: ");
                    scanf("%f", &alimentos[produtosCadastrados].valorProduto);
                printf("Informe o estoque do produto: ");
                    scanf("%d", &alimentos[produtosCadastrados].estoqueProduto);
                    registro = true;
                        if (registro = true)
                            produtosCadastrados++;
                sleep(2);
                registrarProduto();
                break;

        case 3:
                printf("Informe o codigo do produto: ");
                    scanf("%d", &padaria[produtosCadastrados].codigoProduto);
                getchar();
                printf("Informe o nome do produto: ");
                    fgets(padaria[produtosCadastrados].nomeProduto, sizeof(padaria[produtosCadastrados].nomeProduto), stdin);
                    padaria[produtosCadastrados].nomeProduto[strcspn(padaria[produtosCadastrados].nomeProduto, "\n")] = '\0';
                printf("Informe o valor do produto: ");
                    scanf("%f", &padaria[produtosCadastrados].valorProduto);
                printf("Informe o estoque do produto: ");
                    scanf("%d", &padaria[produtosCadastrados].estoqueProduto);
                    registro = true;
                        if (registro = true)
                            produtosCadastrados++;
                sleep(2);
                registrarProduto();
                break;
        case 4:
            main();
    }
}

void listarProdutos(){
    int codigoLista;
    system("cls");
    printf("Lista de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    scanf("%d", &codigoLista);
    switch(codigoLista){
    case 1:
        printf("Limpeza\n");
            for(int i = 0; i < produtosCadastrados; i++){
                    printf("Codigo: %d Nome: %-23s Valor: %.2f Estoque: (%d)\n",
                       limpeza[i].codigoProduto, limpeza[i].nomeProduto, limpeza[i].valorProduto, limpeza[i].estoqueProduto);
    }
        sleep(2);
        main();
        break;
    case 2:
        printf("Alimentos\n");
            for(int j = 0; j < produtosCadastrados; j++){
                printf("Codigo: %d Nome: %-23s Valor: %.2f Estoque: (%d)\n", alimentos[j].codigoProduto, alimentos[j].nomeProduto, alimentos[j].valorProduto, alimentos[j].estoqueProduto);
    }
        sleep(2);
        main();
        break;
    case 3:
        printf("Padaria\n");
            for(int k = 0; k < produtosCadastrados; k++){
                printf("Codigo: %d Nome: %s Valor: %.2f Estoque: (%d)\n", padaria[k].codigoProduto, padaria[k].nomeProduto, padaria[k].valorProduto, padaria[k].estoqueProduto);
        }
        sleep(2);
        main();
        break;
    }
}

int menuProdutos(){
    int codigoMenu;
    system("cls");
    printf("Menu Principal\n");
    printf("1 - Registrar Produtos\n");
    printf("2 - Listar Produtos\n");
    scanf("%d", &codigoMenu);
    switch(codigoMenu){
        case 1:
            registrarProduto();
        break;
        case 2:
            listarProdutos();
        break;
    }
}

int main(){
    menuProdutos();

    return 0;
}

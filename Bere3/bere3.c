#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


// Estrutura para armazenar a venda atual
typedef struct {
    int codigoProduto;
    char descricao[100];
    float precoUnitario;
    int quantidade;
    float subtotal;
} ItemVenda;

//=== ESTRUTURAS DE DADOS ===//
typedef struct {
    int codigo;
    char nomeCompleto[100];
    char nomeSocial[100];
    char cpf[15];
    char endereco[100];
    char bairro[50];
    char celular[20];
} Cliente;

typedef struct {
    int codigo;
    char nomeProduto[100];
    char *categoriaProduto;
    float precoCompra;
    float percentual;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

typedef struct {
    char **categorias;
    int quantidade;
} ListaCategorias;

typedef struct {
    Produto *produtos;
    int quantidade;
} SistemaProdutos;

SistemaProdutos sistemaProdutos = {NULL, 0};

// ===== VARIÁVEIS GLOBAIS ===== //
ListaCategorias categoriasGlobais = {NULL, 0};
float totalCaixa = 0;
float totalVenda = 0;
int numeroVenda = 0;
float totalVendas = 0;
int numItens = 0;
int quantidadeProdutos = 0;
int opcaoMenu;
int caixaAberto = 0;
int proximoNumeroVenda = 1;
float vAbre = 0;

// ===== PROTÓTIPOS DE FUNÇÕES ===== //
void menu_principal(void);
void exibirMenu(void);
void menuCadastro(void);
void cadastrarCliente(Cliente **clientes, int *quantidadeClientes);
void cadastrarProduto(Produto **produtos, int *quantidadeProdutos);
void menuVendas(Produto *produtos, int quantidadeProdutos);
float retiradaCaixa(float *totalCaixa);
void menuAberturaCaixa(void);
void menuFechamentoCaixa(void);
void menuRelatorios(void);
void menu_pagamento(void);
void menu_novaVenda(void);


// Protótipos de funções
bool adicionarCategoria(const char *categoria);
bool categoriaExiste(const char *categoria);
void exibirCategorias();
void liberarCategorias();
void salvarCategorias();
void salvarProdutos();
void carregarProdutos();
void carregarCategorias();

FILE *arquivo_cliente, *arquivo_produto, *arquivo_vendas;

// ===== FUNÇÕES AUXILIARES ===== //

bool validarCPF(const char *cpf) { // Remove caracteres não numéricos
    char cpfLimpo[12]; // 11 dígitos + o Nulo
    int j = 0;
    for (int i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (isdigit(cpf[i])) {
            cpfLimpo[j++] = cpf[i];
        }
    }
    cpfLimpo[j] = '\0';

    // Verificação se tem 11 dígitos
    if (j != 11) {
        return false;
    }

    // Verificação se todos os dígitos são iguais (ex: 11111111111)
    bool todosIguais = true;
    for (int i = 1; i < 11; i++) {
        if (cpfLimpo[i] != cpfLimpo[0]) {
            todosIguais = false;
            break;
        }
    }
    if (todosIguais) {
        return false;
    }

    return true;
}

double truncarValor(double const valor, int const casasDecimais) {
    // Verifica se o número de casas decimais é válido
    if (casasDecimais < 0) {
        return valor; // Retorna o valor original se casas decimais for negativo
    }

    // Calcula o fator de multiplicação/divisão
    double fator = pow(10.0, casasDecimais);

    // Trunca o valor
    return trunc(valor * fator) / fator;
}

// ===== FUNÇÕES DE MENU ===== //
void exibirMenu() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t    Mercado Dona Bere\t\t\t|\n");
    printf("|=======================================================|\n");
    printf("|\t ------------ MENU PRINCIPAL ------------\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| -> 1 - CADASTROS \t\t\t\t\t|\n");
    printf("| -> 2 - VENDAS \t\t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 3 - ABERTURA DE CAIXA \t\t\t\t|\n");
    printf("| -> 4 - FECHAMENTO DE CAIXA \t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 5 - RELATORIOS \t\t\t\t\t|\n");
    printf("| -> 6 - SAIR \t\t\t\t\t\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| TOTAL DE VENDAS HOJE: R$ %.2f \t\t\t|\n", totalVendas);
    printf("|=======================================================|\n");
    printf("Opcao: ");
}

void menu_principal() {
    Produto *produtos;
    int quantidadeProdutos = 0;

    do {
        exibirMenu();
        scanf("%d", &opcaoMenu);
        getchar();

        switch (opcaoMenu) {
            case 1:
                system("cls");
                menuCadastro();
                break;
            case 2:
                menuVendas(produtos, quantidadeProdutos);
                break;
            /*case 3:
                menuAberturaCaixa();
                break;
            case 4:
                menuFechamentoCaixa();
                break;
            case 5:
                menuRelatorios();
                break;*/
            case 6:
                /*if (caixaAberto) {
                    printf("Caixa ainda aberto. Realize o fechamento primeiro.\n");
                    system("pause");
                } else {
                    // Liberar memória antes de sair
                    for (int i = 0; i < quantidadeProdutos; i++) {
                        free(produtos[i].categoriaProduto);
                    }
                    free(produtos);
                    liberarCategorias();
                    printf("Saindo do sistema...\n");
                    system("pause");
                    exit(0);
                }*/
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                system("pause");
        }
    } while (opcaoMenu != 6);
}

void menuCadastro() {
    static Cliente *clientes = NULL;
    static int quantidadeClientes = 0;
    static Produto *produtos = NULL;
    static int quantidadeProdutos = 0;

    while (1) {
        system("cls");
        printf("|=======================================================|\n");
        printf("|\t\t    MENU CADASTROS\t\t\t|\n");
        printf("|=======================================================|\n");
        printf("| -> 1 - CADASTRAR CLIENTE \t\t\t\t|\n");
        printf("| -> 2 - CADASTRAR PRODUTO \t\t\t\t|\n");
        printf("| -> 3 - VOLTAR AO MENU PRINCIPAL \t\t\t|\n");
        printf("|=======================================================|\n");
        printf("Opcao: ");

        scanf("%d", &opcaoMenu);
        getchar();

        switch (opcaoMenu) {
            case 1:
                cadastrarCliente(&clientes, &quantidadeClientes);
                break;
            case 2:
                cadastrarProduto(&produtos, &quantidadeProdutos);
                break;
            case 3:
                return;
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }
}

void cadastrarCliente(Cliente **clientes, int *quantidadeClientes) {
    Cliente *temp = realloc(*clientes, (*quantidadeClientes + 1) * sizeof(Cliente));
    if (temp == NULL) {
        printf("Erro!\n");
        return;
    }
    *clientes = temp;

    Cliente *novoCliente = &((*clientes)[*quantidadeClientes]);
    novoCliente->codigo = *quantidadeClientes + 1;

    printf("\n=== CADASTRO DE CLIENTE ===\n");
    printf("Nome Completo: ");
    fgets(novoCliente->nomeCompleto, sizeof(novoCliente->nomeCompleto), stdin);
    novoCliente->nomeCompleto[strcspn(novoCliente->nomeCompleto, "\n")] = '\0';
    arquivo_cliente = fopen("clientes.dat", "wb");
    fwrite();

    printf("Nome Social (opcional): ");
    fgets(novoCliente->nomeSocial, sizeof(novoCliente->nomeSocial), stdin);
    novoCliente->nomeSocial[strcspn(novoCliente->nomeSocial, "\n")] = '\0';

    printf("CPF: ");
    fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
    novoCliente->cpf[strcspn(novoCliente->cpf, "\n")] = '\0';
    while (!validarCPF(novoCliente->cpf)) {
        printf("\nCPF INVALIDO, DIGITE NOVAMENTE: ");
        fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
        novoCliente->cpf[strcspn(novoCliente->cpf, "\n")] = '\0';
    }

    printf("Endereco: ");
    fgets(novoCliente->endereco, sizeof(novoCliente->endereco), stdin);
    novoCliente->endereco[strcspn(novoCliente->endereco, "\n")] = '\0';

    printf("Bairro: ");
    fgets(novoCliente->bairro, sizeof(novoCliente->bairro), stdin);
    novoCliente->bairro[strcspn(novoCliente->bairro, "\n")] = '\0';

    printf("Celular: ");
    fgets(novoCliente->celular, sizeof(novoCliente->celular), stdin);
    novoCliente->celular[strcspn(novoCliente->celular, "\n")] = '\0';

    (*quantidadeClientes)++;
    printf("\nCliente cadastrado com sucesso! Codigo: %d\n", novoCliente->codigo);
    system("pause");
    free(temp);
}

bool adicionarCategoria(const char *categoria) {
    // Verifica se a categoria já existe
    if (categoriaExiste(categoria)) {
        return false;
    }

    // Realoca memória para a nova categoria
    char **temp = realloc(categoriasGlobais.categorias, (categoriasGlobais.quantidade + 1) * sizeof(char *));
    if (temp == NULL) {
        printf("Erro ao alocar memória para categorias.\n");
        return false;
    }
    categoriasGlobais.categorias = temp;

    // Aloca e copia a nova categoria
    categoriasGlobais.categorias[categoriasGlobais.quantidade] = strdup(categoria);
    if (categoriasGlobais.categorias[categoriasGlobais.quantidade] == NULL) {
        printf("Erro ao alocar memória para nova categoria.\n");
        return false;
    }

    categoriasGlobais.quantidade++;
    return true;
}

bool categoriaExiste(const char *categoria) {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        if (strcmp(categoriasGlobais.categorias[i], categoria) == 0) {
            return true;
        }
    }
    return false;
}

void exibirCategorias() {
    if (categoriasGlobais.quantidade == 0) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }

    printf("\n=== CATEGORIAS DISPONÍVEIS ===\n");
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        printf("%d - %s\n", i + 1, categoriasGlobais.categorias[i]);
    }
    printf("===============================\n");
}

void liberarCategorias() {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        free(categoriasGlobais.categorias[i]);
    }
    free(categoriasGlobais.categorias);
    categoriasGlobais.categorias = NULL;
    categoriasGlobais.quantidade = 0;
}

void salvarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de categorias para escrita!\n");
        return;
    }

    // Escreve a quantidade de categorias
    fwrite(&categoriasGlobais.quantidade, sizeof(int), 1, arquivo);

    // Escreve cada categoria
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        size_t tamanho = strlen(categoriasGlobais.categorias[i]) + 1;
        fwrite(&tamanho, sizeof(size_t), 1, arquivo);
        fwrite(categoriasGlobais.categorias[i], sizeof(char), tamanho, arquivo);
    }

    fclose(arquivo);
}

void salvarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de produtos para escrita!\n");
        return;
    }

    // Escreve a quantidade de produtos
    fwrite(&sistemaProdutos.quantidade, sizeof(int), 1, arquivo);

    // Escreve cada produto
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        Produto *p = &sistemaProdutos.produtos[i];

        // Escreve os dados básicos do produto
        fwrite(p, sizeof(Produto) - sizeof(char *), 1, arquivo);

        // Escreve a categoria (se existir)
        if (p->categoriaProduto != NULL) {
            size_t tamanho = strlen(p->categoriaProduto) + 1;
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
            fwrite(p->categoriaProduto, sizeof(char), tamanho, arquivo);
        } else {
            size_t tamanho = 0;
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
        }
    }

    fclose(arquivo);
}

// ===== FUNÇÕES DE CARREGAMENTO ===== //

void carregarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe ainda
    }

    // Lê a quantidade de categorias
    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo);

    // Carrega cada categoria
    for (int i = 0; i < quantidade; i++) {
        size_t tamanho;
        fread(&tamanho, sizeof(size_t), 1, arquivo);

        char *categoria = (char *)malloc(tamanho * sizeof(char));
        if (categoria == NULL) {
            printf("Erro ao alocar memória para categoria!\n");
            break;
        }

        fread(categoria, sizeof(char), tamanho, arquivo);

        // Adiciona a categoria ao sistema
        char **temp = realloc(categoriasGlobais.categorias,
                            (categoriasGlobais.quantidade + 1) * sizeof(char *));
        if (temp == NULL) {
            free(categoria);
            break;
        }

        categoriasGlobais.categorias = temp;
        categoriasGlobais.categorias[categoriasGlobais.quantidade++] = categoria;
    }

    fclose(arquivo);
}

void carregarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe ainda
    }

    // Lê a quantidade de produtos
    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo);

    // Carrega cada produto
    for (int i = 0; i < quantidade; i++) {
        // Realoca memória para o novo produto
        Produto *temp = realloc(sistemaProdutos.produtos,
                              (sistemaProdutos.quantidade + 1) * sizeof(Produto));
        if (temp == NULL) {
            printf("Erro ao alocar memória para produtos!\n");
            break;
        }
        sistemaProdutos.produtos = temp;

        Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade];

        // Lê os dados básicos do produto
        fread(novo, sizeof(Produto) - sizeof(char *), 1, arquivo);

        // Lê a categoria
        size_t tamanho;
        fread(&tamanho, sizeof(size_t), 1, arquivo);

        if (tamanho > 0) {
            novo->categoriaProduto = (char *)malloc(tamanho * sizeof(char));
            if (novo->categoriaProduto == NULL) {
                printf("Erro ao alocar memória para categoria do produto!\n");
                continue;
            }
            fread(novo->categoriaProduto, sizeof(char), tamanho, arquivo);
        } else {
            novo->categoriaProduto = NULL;
        }

        sistemaProdutos.quantidade++;
    }

    fclose(arquivo);
}

void cadastrarProduto(Produto **produtos, int *quantidadeProdutos) {
    // Realoca memória para o novo produto
    Produto *temp = realloc(sistemaProdutos.produtos, (sistemaProdutos.quantidade + 1) * sizeof(Produto));
    if (temp == NULL) {
        printf("Erro ao alocar memória para novo produto!\n");
        return;
    }
    sistemaProdutos.produtos = temp;
    *produtos = sistemaProdutos.produtos;
    *quantidadeProdutos = sistemaProdutos.quantidade;

    // Obtém referência para o novo produto
    Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade];
    memset(novo, 0, sizeof(Produto)); // Inicializa a estrutura

    // Atribui código ao novo produto
    novo->codigo = sistemaProdutos.quantidade + 1;

    printf("\n=== CADASTRO DE PRODUTO ===\n");

    // Nome do produto
    printf("Nome do Produto: ");
    fgets(novo->nomeProduto, sizeof(novo->nomeProduto), stdin);
    novo->nomeProduto[strcspn(novo->nomeProduto, "\n")] = '\0';

    // Seleção de categoria
    int opcaoCategoria;
    char nomeCategoria[100];
    bool categoriaDefinida = false;

    while (!categoriaDefinida) {
        system("cls");
        printf("\n=== SELECIONE UMA CATEGORIA ===\n");
        exibirCategorias();
        printf("\n1 - Selecionar categoria existente\n");
        printf("2 - Criar nova categoria\n");
        printf("0 - Cancelar cadastro\n");
        printf("Opção: ");
        scanf("%d", &opcaoCategoria);
        getchar();

        switch (opcaoCategoria) {
            case 1: {
                if (categoriasGlobais.quantidade == 0) {
                    printf("Nenhuma categoria disponível. Você precisa criar uma nova.\n");
                    system("pause");
                    break;
                }

                int categoriaSelecionada;
                printf("\nDigite o número da categoria: ");
                scanf("%d", &categoriaSelecionada);
                getchar();

                if (categoriaSelecionada > 0 && categoriaSelecionada <= categoriasGlobais.quantidade) {
                    novo->categoriaProduto = strdup(categoriasGlobais.categorias[categoriaSelecionada - 1]);
                    if (novo->categoriaProduto == NULL) {
                        printf("Erro ao alocar memória para categoria!\n");
                        break;
                    }
                    categoriaDefinida = true;
                } else {
                    printf("Opção inválida!\n");
                    system("pause");
                }
                break;
            }
            case 2: {
                printf("\nNome da nova categoria: ");
                fgets(nomeCategoria, sizeof(nomeCategoria), stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = '\0';

                // Validação do nome da categoria
                bool nomeValido = true;
                for (size_t i = 0; nomeCategoria[i] != '\0'; i++) {
                    if (!isalnum(nomeCategoria[i]) && nomeCategoria[i] != ' ') {
                        nomeValido = false;
                        break;
                    }
                }

                if (!nomeValido) {
                    printf("Nome de categoria inválido! Use apenas letras, números e espaços.\n");
                    system("pause");
                    break;
                }

                if (!categoriaExiste(nomeCategoria)) {
                    if (adicionarCategoria(nomeCategoria)) {
                        novo->categoriaProduto = strdup(nomeCategoria);
                        if (novo->categoriaProduto == NULL) {
                            printf("Erro ao alocar memória para categoria!\n");
                            break;
                        }
                        salvarCategorias();
                        categoriaDefinida = true;
                    } else {
                        printf("Falha ao adicionar nova categoria!\n");
                        system("pause");
                    }
                } else {
                    printf("Categoria já existe!\n");
                    system("pause");
                }
                break;
            }
            case 0:
                printf("Cadastro cancelado.\n");
                free(temp);
                return;
            default:
                printf("Opção inválida!\n");
                system("pause");
        }
    }

    // Preço de compra
    do {
        printf("\nPreço de Compra: R$ ");
        if (scanf("%f", &novo->precoCompra) != 1 || novo->precoCompra <= 0) {
            printf("Valor incorreto! Digite um valor positivo.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }
        break;
    } while (1);
    getchar();

    // Margem de lucro
    do {
        printf("\nMargem de Lucro (%%): ");
        if (scanf("%f", &novo->percentual) != 1 || novo->percentual < 0) {
            printf("Valor incorreto! Digite um percentual não negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Calcula preço de venda
    novo->precoVenda = novo->precoCompra * (1 + novo->percentual / 100);
    printf("\nPreço de Venda Calculado: R$ %.2f\n", novo->precoVenda);

    // Quantidade em estoque
    do {
        printf("\nQuantidade em Estoque: ");
        if (scanf("%d", &novo->estoque) != 1 || novo->estoque < 0) {
            printf("Valor incorreto! Digite um número inteiro não negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Estoque mínimo
    do {
        printf("\nEstoque Mínimo: ");
        if (scanf("%d", &novo->estoqueMinimo) != 1 || novo->estoqueMinimo < 0) {
            printf("Valor incorreto! Digite um número inteiro não negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Atualiza a quantidade de produtos
    sistemaProdutos.quantidade++;
    *quantidadeProdutos = sistemaProdutos.quantidade;

    // Persistência de dados
    salvarProdutos();

    printf("\nProduto cadastrado com sucesso! Código: %d\n", novo->codigo);
    system("pause");
}

float retiradaCaixa(float *totalCaixa);
void menuVendas(Produto *produtos, int quantidadeProdutos) {
    while (1) {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU VENDAS\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");

        // Exibe produtos cadastrados
        if (sistemaProdutos.quantidade > 0) {
            printf("|====================================================================|\n");
            printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
            printf("|====================================================================|\n");
            printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
            printf("|======|==========================|==============|=========|=========|\n");
            for (int i = 0; i < sistemaProdutos.quantidade; i++) {
                printf("|%-5d | %-24s | %-12s | %-7.2f | %-8d|\n",
                        sistemaProdutos.produtos[i].codigo,
                        sistemaProdutos.produtos[i].nomeProduto,
                        sistemaProdutos.produtos[i].categoriaProduto,
                        sistemaProdutos.produtos[i].precoVenda,
                        sistemaProdutos.produtos[i].estoque);
                printf("|------|--------------------------|--------------|---------|---------|\n");
            }
        } else {
            printf("\n|AVISO: Nenhum produto cadastrado!\n");
        }

        printf("|====================================================================|\n");
        printf("\n|-> 1 - NOVA VENDA\n");
        printf("|-> 2 - RETIRADA DE CAIXA (SANGRIA)\n");
        printf("|-> 3 - PAGAMENTO\n");
        printf("|-> 4 - VOLTAR AO MENU PRINCIPAL\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("|OPCAO: ");

        int opcao;
        scanf("%d", &opcao);
        getchar();

        if (opcao == 4) {
            return;
        }

        switch (opcao) {
            case 1:
                system("cls");
                menu_novaVenda();
                break;
            case 2:
                break;
        }
    }
}

void menu_novaVenda(){
    if (sistemaProdutos.quantidade == 0) {
                printf("NAO HA PRODUTOS PARA VENDER!\n");
                system("pause");
                return;
            }


    ItemVenda carrinho[100];
    int numItens = 0;
    float totalVenda = 0;
    static int numeroVenda = 1; // Contador estático para número da venda

    while (1) {
        system("cls");
        // Mostra produtos disponíveis
        printf("|====================================================================|\n");
        printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");
        printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
        printf("|======|==========================|==============|=========|=========|\n");
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            printf("|%-5d | %-24s | %-12s | %-7.2f | %-8d|\n",
                   sistemaProdutos.produtos[i].codigo,
                   sistemaProdutos.produtos[i].nomeProduto,
                   sistemaProdutos.produtos[i].categoriaProduto,
                   sistemaProdutos.produtos[i].precoVenda,
                   sistemaProdutos.produtos[i].estoque);
            printf("|------|--------------------------|--------------|---------|---------|\n");
        }

        // Mostra carrinho atual
        printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroVenda);
        printf("|====================================================================|\n");
        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
        printf("|======|========================|=====|===========|==================|\n");
        for (int i = 0; i < numItens; i++) {
            printf("|%-5d | %-22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                   carrinho[i].codigoProduto,
                   carrinho[i].descricao,
                   carrinho[i].quantidade,
                   carrinho[i].precoUnitario,
                   carrinho[i].subtotal);
            printf("|------|------------------------|-----|-----------|-----------------|\n");
        }
        printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
        printf("|====================================================================|\n");

        int codigoProduto, quantidade;
        printf("\nInforme o codigo do produto a ser comprado (0 para finalizar): ");
        scanf("%d", &codigoProduto);
        getchar();

        if (codigoProduto == 0) {
            break;
        }

        printf("Informe a Quantidade: ");
        scanf("%d", &quantidade);
        if (sistemaProdutos.quantidade == 0 && quantidade >= 0) {
            printf("Nao ha produtos para vender!\n");
            system("pause");
            break;
        }
        getchar();

        bool produtoEncontrado = false;
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            if (sistemaProdutos.produtos[i].codigo == codigoProduto) {
                if (sistemaProdutos.produtos[i].estoque >= quantidade) {
                    // Adiciona ao carrinho
                    carrinho[numItens].codigoProduto = codigoProduto;
                    strcpy(carrinho[numItens].descricao, sistemaProdutos.produtos[i].nomeProduto);
                    carrinho[numItens].precoUnitario = sistemaProdutos.produtos[i].precoVenda;
                    carrinho[numItens].quantidade = quantidade;
                    carrinho[numItens].subtotal = sistemaProdutos.produtos[i].precoVenda * quantidade;

                    // Atualiza totais
                    totalVenda += carrinho[numItens].subtotal;
                    numItens++;
                    if (sistemaProdutos.produtos[i].estoque > 0){
                        // Atualiza estoque
                            sistemaProdutos.produtos[i].estoque -= quantidade;

                    printf("Adicionado: %d x %s (R$ %.2f cada)\n", quantidade,
                           sistemaProdutos.produtos[i].nomeProduto,
                           sistemaProdutos.produtos[i].precoVenda);
                } else {
                    printf("Estoque insuficiente! Disponivel: %d\n", sistemaProdutos.produtos[i].estoque);
                }
                produtoEncontrado = true;
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto nao encontrado!\n");
        }
        system("pause");
    }

    if (numItens > 0) {
        char opcao;
        printf("\nDESEJA ADICIONAR MAIS ITENS AO CARRINHO? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

        if (tolower(opcao) == 'n') {
            float desconto, totalDesconto;
            char opcaoDesconto;
            desconto = totalVenda * 0.05;
            totalDesconto = totalVenda - desconto;
            system("cls");
            printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroVenda);
            printf("|====================================================================|\n");
            printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
            printf("|======|========================|=====|===========|==================|\n");
            for (int i = 0; i < numItens; i++) {
                printf("|%-5d | %-22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                       carrinho[i].codigoProduto,
                       carrinho[i].descricao,
                       carrinho[i].quantidade,
                       carrinho[i].precoUnitario,
                       carrinho[i].subtotal);
                printf("|------|------------------------|-----|-----------|-----------------|\n");
            }
            printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
            printf("|====================================================================|\n");
            printf("|HA DESCONTO (INFORME 0(PARA NAO) OU %% CONCEDIDO): ===_ %.2f_ ===    |\n", totalDesconto);
            printf("|--------------------------------------------------------------------|\n");
            opcaoDesconto = getchar();
                if (opcaoDesconto == '%'){
                totalVenda = totalDesconto;
                menu_pagamento();
            }
            menu_pagamento();

        }
    }
    }
}

void menu_pagamento(){
            ItemVenda *carrinho = NULL;
                carrinho = realloc(carrinho, 100 * sizeof(ItemVenda));
            int opcaoPagamento, opcaoCartao, opcaoDinheiro;
            float totalDinheiro = 0, totalCartao = 0;
            const float totalVendaOriginal = totalVenda;
            printf("|====================================================================|\n");
            printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
            printf("|====================================================================|\n");
            printf("|MENU FORMA DE PAGAMENTO:                                            |\n");
            printf("|1. PAGAMENTO EM CARTAO                                              |\n");
            printf("|2. PAGAMENTO EM DINHEIRO                                            |\n");
            printf("|3. RETORNAR AO MENU PRINCIPAL                                       |\n");
            printf("|--------------------------------------------------------------------|\n");
            printf("OPCAO: ");
            scanf("%d", &opcaoPagamento);
            getchar();

            do {
                if (opcaoPagamento == 1) {
                    printf("|====================================================================|\n");
                    printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
                    printf("|====================================================================|\n");
                    printf("|MENU PAGAMENTO CARTAO:                                              |\n");
                    printf("|1. PAGAMENTO NA MAQUININHA OK!                                      |\n");
                    printf("|2. PAGAMENTO NO CARTAO NAO OK.                                      |\n");
                    printf("|3. RETORNAR AO MENU ANTERIOR                                        |\n");
                    printf("|--------------------------------------------------------------------|\n");
                    printf("OPCAO: ");
                    scanf("%d", &opcaoCartao);
                    getchar();
                    switch(opcaoCartao){
                        case 1:
                            system("cls");
                            printf("PAGAMENTO REALIZADO COM SUCESSO!\n");
                            totalCartao += totalVenda;
                            system("pause");
                            printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
                            printf("  |Numero da venda: %d\n", numeroVenda);
                            printf("  |Total: R$ %.2f\n", totalVendaOriginal);
                            printf("  |CARTAO: R$ %.2f\n", totalCartao);
                            printf("  |DINHEIRO: R$ %.2f\n", totalDinheiro);
                            system("pause");

                            // Atualiza o contador de vendas
                            numeroVenda++;
                            totalVendas += totalVendaOriginal;
                            free(carrinho);
                            menu_principal();
                            break;
                        case 2:
                            system("cls");
                            printf("ERRO AO EFETUAR PAGAMENTO EM CARTAO, INFORME NOVA FORMA DE PAGAMENTO: ");
                            system("pause");
                            menu_pagamento();
                            break;
                        case 3:
                            system("cls");
                            menu_pagamento();
                            break;
                        default:
                            system("cls");
                            printf("OPCAO INVALIDA, TENTE NOVAMENTE: ");
                            system("pause");
                            return;
                    }
                }
                    else if (opcaoPagamento == 2){
                        float pagamento = 0;
                        int opcaoFalta;
                        system("cls");
                        printf("|====================================================================|\n");
                        printf("| TOTAL DA VENDA: R$ %-40.2f                                         |\n", totalVenda);
                        printf("|====================================================================|\n");
                        printf("|MENU PAGAMENTO DINHEIRO:                                            |\n");
                        printf("|--------------------------------------------------------------------|\n");
                        printf("\n| CARRINHO DE COMPRAS - VENDA #%d                                  |\n", numeroVenda);
                        printf("|====================================================================|\n");
                        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
                        printf("|======|========================|=====|===========|==================|\n");
                        for (int i = 0; i < numItens; i++) {
                            printf("|%-5.2d | %-22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                               carrinho[i].codigoProduto,
                               carrinho[i].descricao,
                               carrinho[i].quantidade,
                               carrinho[i].precoUnitario,
                               carrinho[i].subtotal);
                            printf("|------|------------------------|-----|-----------|-----------------|\n");
                        }
                        printf("EFETUAR PAGAMENTO: \nDIGITE O VALOR: ");
                        scanf("%f", &pagamento);
                        getchar();
                        if (totalVenda > pagamento){
                            totalVenda -= pagamento;
                            totalDinheiro += pagamento;
                            totalCaixa += pagamento;
                            printf("\n\n\nFALTA PAGAR: %.2f", totalVenda);
                            printf("\nDESEJA EFETUAR O RESTANTE DO PAGAMENTO EM CARTAO? DIGITE 0 PARA CONFIRMAR");
                            scanf("%d", &opcaoFalta);
                            if (opcaoFalta == 0){
                                printf("|====================================================================|\n");
                                printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
                                printf("|====================================================================|\n");
                                printf("|MENU PAGAMENTO CARTAO:                                              |\n");
                                printf("|1. PAGAMENTO NA MAQUININHA OK!                                      |\n");
                                printf("|2. PAGAMENTO NO CARTAO NAO OK.                                      |\n");
                                printf("|3. RETORNAR AO MENU ANTERIOR                                        |\n");
                                printf("|--------------------------------------------------------------------|\n");
                                printf("OPCAO: ");
                                scanf("%d", &opcaoCartao);
                                getchar();
                                switch(opcaoCartao){
                                    case 1:
                                        system("cls");
                                        printf("PAGAMENTO REALIZADO COM SUCESSO!\n");
                                        totalCartao += totalVenda;
                                        system("pause");
                                        printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
                                        printf("  |Numero da venda: %d\n", numeroVenda);
                                        printf("  |Total: R$ %.2f\n", totalVendaOriginal + pagamento);
                                        printf("  |CARTAO: R$ %.2f\n", totalCartao);
                                        printf("  |DINHEIRO: R$ %.2f\n", totalDinheiro);
                                        system("pause");

                                        // Atualiza o contador de vendas
                                        numeroVenda++;
                                        totalVendas += totalVendaOriginal;
                                        free(carrinho);
                                        menu_principal();
                                        break;
                                    case 2:
                                        system("cls");
                                        printf("ERRO AO EFETUAR PAGAMENTO EM CARTAO, INFORME NOVA FORMA DE PAGAMENTO: ");
                                        system("pause");
                                        menu_pagamento();
                                        break;
                                    case 3:
                                        system("cls");
                                        menu_pagamento();
                                        break;
                                    default:
                                        system("cls");
                                        printf("OPCAO INVALIDA, TENTE NOVAMENTE: ");
                                        system("pause");
                                        return;
                                }
                            }
                        }

                        else if (pagamento > totalVenda){
                            if (pagamento - totalVenda > totalCaixa){
                                system("cls");
                                printf("NAO HA VALOR NO CAIXA SUFICIENTE PARA O TROCO, INFORME NOVO VALOR DE PAGAMENTO: ");
                                system("pause");
                                menu_pagamento();
                            }
                            float troco = pagamento - totalVenda;
                            totalDinheiro = totalVenda;
                            totalCaixa -= troco;
                            printf("TROCO: %.2f", troco);
                            system("pause");
                            system("cls");
                            printf("PAGAMENTO REALIZADO COM SUCESSO!\n");
                            system("pause");
                            printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
                            printf("  |Numero da venda: %d\n", numeroVenda);
                            printf("  |Total: R$ %.2f\n", totalVendaOriginal);
                            printf("  |CARTAO: R$ %.2f\n", totalCartao);
                            printf("  |DINHEIRO: R$ %.2f\n", totalDinheiro);
                            system("pause");

                            // Atualiza o contador de vendas
                            numeroVenda++;
                            totalVendas += totalVendaOriginal;
                            menu_principal();
                        }
                        else if (pagamento == totalVenda){
                            totalDinheiro += totalVenda;
                            system("pause");
                            system("cls");
                            printf("PAGAMENTO REALIZADO COM SUCESSO!\n");
                            system("pause");
                            printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
                            printf("  |Numero da venda: %d\n", numeroVenda);
                            printf("  |Total: R$ %.2f\n", totalVendaOriginal);
                            printf("  |CARTAO: R$ %.2f\n", totalCartao);
                            printf("  |DINHEIRO: R$ %.2f\n", totalDinheiro);
                            system("pause");

                            // Atualiza o contador de vendas
                            numeroVenda++;
                            totalVendas += totalVendaOriginal;
                            menu_principal();
                        }
                        else{
                            system("cls");
                            printf("OPCAO INVALIDA, DIGITE NOVAMENTE!! ");
                            system("pause");
                            return;
                        }

                    }
                }while (opcaoPagamento != 3 && totalVenda > 0);
            }

float retiradaCaixa(float *totalCaixa){
    system("cls");
        printf("|===========================================================|\n");
        printf("|\t\t   RETIRADA DE CAIXA (SANGRIA)\t\t\t   |\n");
        printf("|===========================================================|\n");
        printf("| SALDO ATUAL EM CAIXA: R$ %.2f\n", *totalCaixa);
        printf("| DIGITE O VALOR A SER RETIRADO: R$ ");
            
        float valorRetirada;
        scanf("%f", &valorRetirada);
        getchar();
            if (valorRetirada <= 0){
                printf("| VALOR INVALIDO\n");
                system("pause");
                return *totalCaixa;
            }
            else if (valorRetirada > *totalCaixa){
                printf("| SALDO INSUFICIENTE. SALDO ATUAL: R$ %.2f\n", *totalCaixa);
                system("pause");
                return *totalCaixa;
            }
            else if (*totalCaixa - valorRetirada > 50){
                printf("NECESSARIO DEIXAR VALOR MINIMO DE 50 REAIS EM CAIXA, SALDO ATUAL: R$ %.2f\n", *totalCaixa);
                system("pause");
                return *totalCaixa;
            }
            *totalCaixa -= valorRetirada;
            printf("| RETIRADA REALIZADA COM SUCESSO!\n");
            printf("| NOVO SALDO EM CAIXA: R$ %.2f\n", *totalCaixa);
            system("pause");
            return *totalCaixa;
}

void menuAberturaCaixa(void){
    int opcaoAbre;
    printf("|====================================================================|\n");
    printf("|\t\t    MENU ABERTURA\t\t\t\t     |\n");
    printf("|====================================================================|\n");
    printf("|-> 1. ABRIR CAIXA\n");
    printf("|-> 2. RETORNAR AO MENU PRINCIPAL\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("| OPCAO: ");
    scanf("|%d", &opcaoAbre);
    getchar();
    switch(opcaoAbre){
        case 1:
            system("cls");
            if (caixaAberto == 1){
                printf("JA HA UM CAIXA EM ABERTO, REALIZE O FECHAMENTO");
                system("pause");
                menu_principal();
                break;
            }
            printf("REALIZANDO ABERTURA DE CAIXA...");
            Sleep(2000);
            system("cls");
            printf("DIGITE O VALOR DE ABERTURA: \n");
            printf("VALOR R$ ");
            scanf("%f", &vAbre);
            getchar();
            system("cls");
            if (vAbre < 0){
                system("cls");
                printf("VALOR DE ABERTURA INCORRETO, RETORNANDO...");
                system("pause");
                system("cls");
                aberturaCaixa();
                break;
            }
            printf("ABERTURA REALIZADA COM SUCESSO!\n");
            printf("VALOR DE ABERTURA: R$ %.2f", vAbre);
            totalCaixa = vAbre;
            system("pause");
            system("cls");
            caixaAberto = 1;
            menu_principal();
            break;
        case 2:
            system("cls");
            menu_principal();
            break;
        default:
            system("cls");
            printf("OPCAO INVALIDA, RETORNANDO...");
            system("pause");
            return;
    }
}

//==== FUNÇÃO PRINCIPAL ====//
int main() {
    system("color 0a"); // Cor vermelha no terminal;
    adicionarCategoria("LIMPEZA");
    // Carrega dados ao iniciar o programa
    carregarCategorias();
    carregarProdutos();

    // Inicializar com categorias padrão

    menu_principal();
    // Libera memória ao sair
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        free(sistemaProdutos.produtos[i].categoriaProduto);
    }
    free(sistemaProdutos.produtos);

    liberarCategorias();
    return 0;
}

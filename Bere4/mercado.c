#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ARQUIVO_LOG "relatorios.log"
#define ARQUIVO_USUARIOS "usuarios.dat"
#define ARQUIVO_CLIENTES "clientes.dat"
#define ARQUIVO_CATEGORIAS "categorias.dat"
#define ARQUIVO_PRODUTOS "produtos.dat"
#define MAX_ITENS_CARRINHO 100

// =============================================================================
// ESTRUTURAS DE DADOS (AGORA COM LISTAS LIGADAS)
// =============================================================================

// Estrutura para guardar informações de um Usuário
typedef struct {
    int codigo;
    char nome[100];
    char login[50];
    char senha[50];
} Usuario;

// Nó da lista de Usuários
typedef struct NodeUsuario {
    Usuario dados;
    struct NodeUsuario *proximo;
} NodeUsuario;

// Estrutura para guardar informações de um Cliente
typedef struct {
    int codigo;
    char nomeCompleto[100];
    char nomeSocial[100];
    char cpf[15];
    char endereco[100];
    char bairro[50];
    char celular[20];
} Cliente;

// Nó da lista de Clientes
typedef struct NodeCliente {
    Cliente dados;
    struct NodeCliente *proximo;
} NodeCliente;

// Estrutura para guardar informações de uma Categoria
typedef struct {
    int codigo;
    char nome[100];
} Categoria;

// Nó da lista de Categorias
typedef struct NodeCategoria {
    Categoria dados;
    struct NodeCategoria *proximo;
} NodeCategoria;

// Estrutura para guardar informações de um Produto
typedef struct {
    int codigo;
    char nomeProduto[100];
    int codigoCategoria; // Referencia a categoria pelo código
    float precoCompra;
    float percentual;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

// Nó da lista de Produtos
typedef struct NodeProduto {
    Produto dados;
    struct NodeProduto *proximo;
} NodeProduto;

// Estrutura para item no carrinho de compras
typedef struct {
    int codigoProduto;
    char descricao[100];
    float precoUnitario;
    int quantidade;
    float subtotal;
} ItemVenda;


// =============================================================================
// VARIÁVEIS GLOBAIS E CABEÇAS DAS LISTAS
// =============================================================================

NodeUsuario *listaUsuarios = NULL;
NodeCliente *listaClientes = NULL;
NodeCategoria *listaCategorias = NULL;
NodeProduto *listaProdutos = NULL;

const float SALDO_MINIMO = 50.0f;
float totalCaixa = 0;
float totalVendas = 0;
int caixaAberto = 0;
int proximoNumeroVenda = 1;
float vAbre = 0;
float totalDinheiroF = 0, totalCartaoF = 0;

// Contadores para o tipo de venda
int numVendasDinheiro = 0;
int numVendasCartao = 0;
int numVendasMisto = 0;

// =============================================================================
// PROTÓTIPOS DE FUNÇÕES
// =============================================================================

// Funções de Menu
void menu_principal(void);
void exibirMenuPrincipal(void);
void menuGerenciamento(void);
void menuVendas(void);
void menuAberturaCaixa(void);
void menuFechamentoCaixa(void);
void menuRelatorios(void);
void menu_pagamento(float valorAPagar, ItemVenda carrinho[], int numItens);
void menu_novaVenda(void);

// Funções de Cadastro (Create)
void cadastrarUsuario();
void cadastrarCliente();
void cadastrarCategoria();
void cadastrarProduto();

// Funções de Exclusão (Delete)
void excluirUsuario();
void excluirCliente();
void excluirCategoria();
void excluirProduto();

// Funções de Exibição (Read)
void exibirUsuarios();
void exibirClientes();
void exibirCategorias(bool comPause);
void exibirProdutos();
void exibirRelatorioVendas();
void exibirLogs();

// Funções de Persistência (Salvar/Carregar)
void salvarTudo();
void carregarTudo();
void salvarUsuarios();
void carregarUsuarios();
void salvarClientes();
void carregarClientes();
void salvarCategorias();
void carregarCategorias();
void salvarProdutos();
void carregarProdutos();

// Funções de Liberação de Memória
void liberarListas();
void liberarListaUsuarios();
void liberarListaClientes();
void liberarListaCategorias();
void liberarListaProdutos();

// Funções Auxiliares e de Negócio
void registrarLog(const char *mensagem);
void verificarEstoqueMinimo();
void retiradaCaixa();
bool validarCPF(const char *cpf);
int obterProximoCodigoUsuario();
int obterProximoCodigoCliente();
int obterProximoCodigoCategoria();
int obterProximoCodigoProduto();
const char* obterNomeCategoria(int codCategoria);

// =============================================================================
// FUNÇÃO MAIN
// =============================================================================

int main() {
    system("color 0A"); // Cor do console (Verde sobre preto)
    carregarTudo();     // Carrega todos os dados dos arquivos ao iniciar
    verificarEstoqueMinimo(); // Verifica estoque baixo na inicialização
    menu_principal();   // Inicia o menu principal
    liberarListas();    // Libera toda a memória alocada antes de sair
    return 0;
}


// =============================================================================
// FUNÇÕES DE MENU E FLUXO PRINCIPAL
// =============================================================================

void menu_principal() {
    int opcao;
    bool sair = false;

    do {
        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1) {
            opcao = 0; // Previne loop infinito se a entrada não for um número
        }
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1: menuGerenciamento(); break;
            case 2:
                if (caixaAberto) menuVendas();
                else { printf("\n>> ERRO: Caixa fechado. Efetue a abertura primeiro.\n"); system("pause"); }
                break;
            case 3:
                if (!caixaAberto) menuAberturaCaixa();
                else { printf("\n>> AVISO: O caixa ja esta aberto.\n"); system("pause"); }
                break;
            case 4:
                if (caixaAberto) menuFechamentoCaixa();
                else { printf("\n>> AVISO: O caixa ja esta fechado.\n"); system("pause"); }
                break;
            case 5: menuRelatorios(); break;
            case 6:
                if (caixaAberto) {
                    printf("\n>> ERRO: O caixa ainda esta aberto. Realize o fechamento primeiro.\n");
                    system("pause");
                } else {
                    printf("\nSalvando todos os dados...\n");
                    salvarTudo();
                    printf("Saindo do sistema. Ate logo!\n");
                    sair = true;
                    system("pause");
                }
                break;
            default:
                printf("\n>> ERRO: Opcao invalida! Tente novamente.\n");
                system("pause");
        }
    } while (!sair);
}

void exibirMenuPrincipal() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t    Mercado Dona Bere\t\t\t|\n");
    printf("|=======================================================|\n");
    printf("|\t ------------ MENU PRINCIPAL ------------\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| -> 1 - GERENCIAMENTO (Cadastros / Exclusoes) \t\t|\n");
    printf("| -> 2 - VENDAS \t\t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 3 - ABERTURA DE CAIXA \t\t\t\t|\n");
    printf("| -> 4 - FECHAMENTO DE CAIXA \t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 5 - RELATORIOS \t\t\t\t\t|\n");
    printf("| -> 6 - SALVAR E SAIR \t\t\t\t\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| TOTAL DE VENDAS HOJE: R$ %-25.2f |\n", totalVendas);
    printf("|=======================================================|\n");
    printf(" Opcao: ");
}

void menuGerenciamento() {
    int opcao = 0;
    while (opcao != 9) {
        system("cls");
        printf("|=========================================================|\n");
        printf("|\t\t MENU DE GERENCIAMENTO \t\t\t  |\n");
        printf("|=========================================================|\n");
        printf("| CLIENTES\t| PRODUTOS\t| CATEGORIAS\t| USUARIOS\t  |\n");
        printf("|---------------|---------------|---------------|---------------|\n");
        printf("| 1-Cadastrar\t| 3-Cadastrar\t| 5-Cadastrar\t| 7-Cadastrar\t  |\n");
        printf("| 2-Excluir\t| 4-Excluir\t| 6-Excluir\t| 8-Excluir\t  |\n");
        printf("|---------------------------------------------------------|\n");
        printf("| 9- Voltar ao Menu Principal \t\t\t\t  |\n");
        printf("|=========================================================|\n");
        printf(" Opcao: ");

        if (scanf("%d", &opcao) != 1) opcao = 0;
        getchar();

        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: excluirCliente(); break;
            case 3: cadastrarProduto(); break;
            case 4: excluirProduto(); break;
            case 5: cadastrarCategoria(); break;
            case 6: excluirCategoria(); break;
            case 7: cadastrarUsuario(); break;
            case 8: excluirUsuario(); break;
            case 9: break; // Sai do loop e volta ao menu principal
            default: printf("\n>> ERRO: Opcao invalida!\n"); system("pause");
        }
    }
}

void menuVendas() {
    int opcao;
    while (true) {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t\t    MENU VENDAS\t\t\t\t     |\n");
        printf("|====================================================================|\n");

        exibirProdutos(); // Mostra a lista de produtos atualizada

        printf("\n|-> 1 - NOVA VENDA\n");
        printf("|-> 2 - RETIRADA DE CAIXA (SANGRIA)\n");
        printf("|-> 3 - VOLTAR AO MENU PRINCIPAL\n");
        printf("|--------------------------------------------------------------------|\n");
        printf(" OPCAO: ");

        if (scanf("%d", &opcao) != 1) opcao = 0;
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                if (listaProdutos != NULL) menu_novaVenda();
                else { printf("\nNao ha produtos cadastrados para vender!\n"); system("pause"); }
                break;
            case 2:
                retiradaCaixa();
                break;
            case 3:
                return; // Volta ao menu principal
            default:
                printf("\nOpcao invalida!\n");
                system("pause");
        }
    }
}

void menu_novaVenda() {
    ItemVenda carrinho[MAX_ITENS_CARRINHO];
    int numItens = 0;
    float totalVenda = 0;
    int codProd, qtd;

    while(true) {
        system("cls");
        printf("--- NOVA VENDA (Venda #%d) ---\n", proximoNumeroVenda);
        exibirProdutos();

        printf("\n--- CARRINHO DE COMPRAS ---\n");
        if(numItens == 0){
            printf("Carrinho vazio.\n");
        } else {
            for(int i = 0; i < numItens; i++){
                printf("%d x %s (R$ %.2f) - Subtotal: R$ %.2f\n", carrinho[i].quantidade, carrinho[i].descricao, carrinho[i].precoUnitario, carrinho[i].subtotal);
            }
        }
        printf("----------------------------------\n");
        printf("TOTAL: R$ %.2f\n", totalVenda);
        printf("----------------------------------\n");

        printf("\nDigite o codigo do produto (0 para finalizar): ");
        if(scanf("%d", &codProd) != 1) codProd = -1;
        getchar();

        if(codProd == 0) break;

        NodeProduto* p_aux = listaProdutos;
        bool encontrado = false;
        while(p_aux){
            if(p_aux->dados.codigo == codProd){
                encontrado = true;
                break;
            }
            p_aux = p_aux->proximo;
        }

        if(!encontrado){
            printf(">> ERRO: Produto nao encontrado!\n");
            system("pause");
            continue;
        }

        printf("Digite a quantidade: ");
        if(scanf("%d", &qtd) != 1 || qtd <= 0) {
            printf(">> ERRO: Quantidade invalida!\n");
            system("pause");
            continue;
        }
        getchar();

        if(p_aux->dados.estoque < qtd){
            printf(">> ERRO: Estoque insuficiente! Disponivel: %d\n", p_aux->dados.estoque);
            system("pause");
            continue;
        }

        if (numItens >= MAX_ITENS_CARRINHO) {
            printf(">> ERRO: Carrinho cheio!\n");
            system("pause");
            break;
        }

        // Adiciona item ao carrinho
        carrinho[numItens].codigoProduto = codProd;
        strcpy(carrinho[numItens].descricao, p_aux->dados.nomeProduto);
        carrinho[numItens].precoUnitario = p_aux->dados.precoVenda;
        carrinho[numItens].quantidade = qtd;
        carrinho[numItens].subtotal = p_aux->dados.precoVenda * qtd;
        totalVenda += carrinho[numItens].subtotal;
        numItens++;
    }

    if(numItens > 0){
        menu_pagamento(totalVenda, carrinho, numItens);
    } else {
        printf("\nVenda cancelada.\n");
        system("pause");
    }
}


void menuAberturaCaixa() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t   ABERTURA DE CAIXA\t\t\t|\n");
    printf("|=======================================================|\n");
    float valorAbertura;
    do {
        printf("Digite o valor de abertura do caixa: R$ ");
        if (scanf("%f", &valorAbertura) != 1 || valorAbertura < 0) {
            printf(">> ERRO: Valor invalido! Digite um valor positivo.\n");
            while(getchar() != '\n'); // Limpar buffer
            valorAbertura = -1;
        }
    } while (valorAbertura < 0);
    getchar();

    vAbre = valorAbertura;
    totalCaixa = vAbre;
    caixaAberto = 1;

    char logMsg[100];
    sprintf(logMsg, "Caixa aberto com R$ %.2f", vAbre);
    registrarLog(logMsg);

    printf("\n>> Caixa aberto com sucesso com R$ %.2f!\n", vAbre);
    system("pause");
}

void menuFechamentoCaixa() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t   FECHAMENTO DE CAIXA\t\t\t|\n");
    printf("|=======================================================|\n");

    float valorEsperado = vAbre + totalDinheiroF;
    float diferenca = totalCaixa - valorEsperado;

    printf("| RESUMO DO DIA\n");
    printf("|-------------------------------------------------------|\n");
    printf("| Valor de Abertura:\t\t R$ %10.2f\n", vAbre);
    printf("| Total Vendas Dinheiro:\t R$ %10.2f\n", totalDinheiroF);
    printf("| Total Vendas Cartao:\t\t R$ %10.2f\n", totalCartaoF);
    printf("| Faturamento Total:\t\t R$ %10.2f\n", totalVendas);
    printf("|-------------------------------------------------------|\n");
    printf("| Valor Esperado em Caixa:\t R$ %10.2f\n", valorEsperado);
    printf("| Valor Apurado em Caixa:\t R$ %10.2f\n", totalCaixa);
    printf("|-------------------------------------------------------|\n");
    printf("| DIFERENCA:\t\t\t R$ %10.2f\n", diferenca);
    printf("|=======================================================|\n\n");

    if (fabs(diferenca) > 0.01) {
        printf("!! ATENCAO: DIVERGENCIA ENCONTRADA NO CAIXA !!\n");
    }

    printf("Confirmar o fechamento do caixa? (s/n): ");
    char confirma;
    scanf(" %c", &confirma);
    getchar();

    if (tolower(confirma) == 's') {
        caixaAberto = 0;

        char logMsg[256];
        sprintf(logMsg, "Caixa fechado. Esperado: R$%.2f, Apurado: R$%.2f, Diferenca: R$%.2f", valorEsperado, totalCaixa, diferenca);
        registrarLog(logMsg);

        // Resetar valores para o próximo dia
        totalVendas = 0;
        totalDinheiroF = 0;
        totalCartaoF = 0;
        vAbre = 0;
        proximoNumeroVenda = 1;
        numVendasDinheiro = 0;
        numVendasCartao = 0;
        numVendasMisto = 0;


        printf("\n>> Caixa fechado com sucesso!\n");
    } else {
        printf("\nFechamento cancelado.\n");
    }
    system("pause");
}


void menuRelatorios() {
    int opcao = 0;
    while(opcao != 5){
        system("cls");
        printf("|=======================================================|\n");
        printf("|\t\t    MENU RELATORIOS\t\t\t|\n");
        printf("|=======================================================|\n");
        printf("| -> 1 - RELATORIO DE VENDAS DO DIA \t\t\t|\n");
        printf("| -> 2 - LISTAR PRODUTOS CADASTRADOS \t\t\t|\n");
        printf("| -> 3 - LISTAR CLIENTES CADASTRADOS \t\t\t|\n");
        printf("| -> 4 - VISUALIZAR LOGS DO SISTEMA \t\t\t|\n");
        printf("| -> 5 - VOLTAR AO MENU PRINCIPAL \t\t\t|\n");
        printf("|=======================================================|\n");
        printf(" Opcao: ");

        if(scanf("%d", &opcao)!=1) opcao = 0;
        getchar();

        switch(opcao){
            case 1: exibirRelatorioVendas(); break;
            case 2: system("cls"); exibirProdutos(); system("pause"); break;
            case 3: system("cls"); exibirClientes(); system("pause"); break;
            case 4: exibirLogs(); break;
            case 5: break;
            default: printf("Opcao invalida!\n"); system("pause");
        }
    }
}


// =============================================================================
// FUNÇÕES DE CADASTRO (CREATE)
// =============================================================================

void cadastrarCliente() {
    system("cls");
    NodeCliente *novo = (NodeCliente*) malloc(sizeof(NodeCliente));
    if (!novo) { printf("Erro de alocacao de memoria!\n"); return; }

    novo->dados.codigo = obterProximoCodigoCliente();

    printf("=== CADASTRO DE CLIENTE (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome Completo: ");
    fgets(novo->dados.nomeCompleto, 100, stdin);
    novo->dados.nomeCompleto[strcspn(novo->dados.nomeCompleto, "\n")] = 0;

    printf("Nome Social (opcional): ");
    fgets(novo->dados.nomeSocial, 100, stdin);
    novo->dados.nomeSocial[strcspn(novo->dados.nomeSocial, "\n")] = 0;

    while (true) {
        printf("CPF (formato XXX.XXX.XXX-XX): ");
        fgets(novo->dados.cpf, 15, stdin);
        novo->dados.cpf[strcspn(novo->dados.cpf, "\n")] = 0;
        if (validarCPF(novo->dados.cpf)) break;
        printf(">> CPF invalido! Tente novamente.\n");
    }

    printf("Endereco (Rua, Numero): ");
    fgets(novo->dados.endereco, 100, stdin);
    novo->dados.endereco[strcspn(novo->dados.endereco, "\n")] = 0;

    printf("Bairro: ");
    fgets(novo->dados.bairro, 50, stdin);
    novo->dados.bairro[strcspn(novo->dados.bairro, "\n")] = 0;

    printf("Celular (XX) XXXXX-XXXX: ");
    fgets(novo->dados.celular, 20, stdin);
    novo->dados.celular[strcspn(novo->dados.celular, "\n")] = 0;

    // Adiciona no início da lista
    novo->proximo = listaClientes;
    listaClientes = novo;

    printf("\n>> Cliente cadastrado com sucesso!\n");
    salvarClientes(); // Salva a alteração no arquivo
    system("pause");
}

void cadastrarProduto() {
    system("cls");
    if (listaCategorias == NULL) {
        printf(">> ERRO: Nenhuma categoria cadastrada. Cadastre uma categoria primeiro.\n");
        system("pause");
        return;
    }

    NodeProduto *novo = (NodeProduto*) malloc(sizeof(NodeProduto));
    if (!novo) { printf("Erro de alocacao!\n"); return; }

    novo->dados.codigo = obterProximoCodigoProduto();
    printf("=== CADASTRO DE PRODUTO (Codigo: %d) ===\n", novo->dados.codigo);

    printf("Nome do Produto: ");
    fgets(novo->dados.nomeProduto, 100, stdin);
    novo->dados.nomeProduto[strcspn(novo->dados.nomeProduto, "\n")] = 0;

    while (true) {
        exibirCategorias(false);
        printf("\nDigite o codigo da categoria para o produto: ");
        if (scanf("%d", &novo->dados.codigoCategoria) != 1) novo->dados.codigoCategoria = -1;
        getchar();

        if (obterNomeCategoria(novo->dados.codigoCategoria) != NULL) break;
        printf(">> Codigo de categoria invalido! Tente novamente.\n");
        system("pause");
        system("cls");
    }

    printf("Preco de Compra: R$ ");
    scanf("%f", &novo->dados.precoCompra);
    printf("Margem de Lucro (%%): ");
    scanf("%f", &novo->dados.percentual);
    novo->dados.precoVenda = novo->dados.precoCompra * (1 + novo->dados.percentual / 100);
    printf("Preco de Venda Calculado: R$ %.2f\n", novo->dados.precoVenda);
    printf("Quantidade em Estoque: ");
    scanf("%d", &novo->dados.estoque);
    printf("Estoque Minimo: ");
    scanf("%d", &novo->dados.estoqueMinimo);
    getchar();

    novo->proximo = listaProdutos;
    listaProdutos = novo;

    printf("\n>> Produto cadastrado com sucesso!\n");
    salvarProdutos();
    system("pause");
}

void cadastrarCategoria() {
    system("cls");
    NodeCategoria *novo = (NodeCategoria*) malloc(sizeof(NodeCategoria));
    if (!novo) { printf("Erro de alocacao!\n"); return; }

    novo->dados.codigo = obterProximoCodigoCategoria();

    printf("=== CADASTRO DE CATEGORIA (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome da Categoria: ");
    fgets(novo->dados.nome, 100, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0;

    novo->proximo = listaCategorias;
    listaCategorias = novo;

    printf("\n>> Categoria cadastrada com sucesso!\n");
    salvarCategorias();
    system("pause");
}

void cadastrarUsuario() {
    system("cls");
    NodeUsuario *novo = (NodeUsuario*) malloc(sizeof(NodeUsuario));
    if (!novo) { printf("Erro de alocacao!\n"); return; }

    novo->dados.codigo = obterProximoCodigoUsuario();

    printf("=== CADASTRO DE USUARIO (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome Completo do Usuario: ");
    fgets(novo->dados.nome, 100, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0;

    printf("Login de Acesso: ");
    fgets(novo->dados.login, 50, stdin);
    novo->dados.login[strcspn(novo->dados.login, "\n")] = 0;

    printf("Senha: ");
    fgets(novo->dados.senha, 50, stdin);
    novo->dados.senha[strcspn(novo->dados.senha, "\n")] = 0;

    novo->proximo = listaUsuarios;
    listaUsuarios = novo;

    printf("\n>> Usuario cadastrado com sucesso!\n");
    salvarUsuarios();
    system("pause");
}


// =============================================================================
// FUNÇÕES DE EXCLUSÃO (DELETE)
// =============================================================================

void excluirCliente() {
    system("cls");
    exibirClientes();
    if (!listaClientes) {
        printf("\nNenhum cliente para excluir.\n");
        system("pause");
        return;
    }

    int cod;
    printf("\nDigite o codigo do cliente a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    if (cod == 0) return;

    NodeCliente *atual = listaClientes, *anterior = NULL;

    while (atual != NULL && atual->dados.codigo != cod) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\n>> ERRO: Cliente com codigo %d nao encontrado.\n", cod);
        system("pause");
        return;
    }

    printf("\nTem certeza que deseja excluir o cliente '%s'? (s/n): ", atual->dados.nomeCompleto);
    char confirma;
    scanf(" %c", &confirma);

    if (tolower(confirma) == 's') {
        if (anterior == NULL) { // Se é o primeiro elemento
            listaClientes = atual->proximo;
        } else { // Se está no meio ou fim
            anterior->proximo = atual->proximo;
        }
        free(atual); // Libera a memória do nó
        salvarClientes();
        printf("\n>> Cliente excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
    system("pause");
}

void excluirProduto() {
    system("cls");
    exibirProdutos();
    if (!listaProdutos) {
        printf("\nNenhum produto para excluir.\n");
        system("pause");
        return;
    }

    int cod;
    printf("\nDigite o codigo do produto a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    if (cod == 0) return;

    NodeProduto *atual = listaProdutos, *anterior = NULL;

    while(atual != NULL && atual->dados.codigo != cod) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\n>> ERRO: Produto com codigo %d nao encontrado.\n", cod);
        system("pause");
        return;
    }

    printf("\nTem certeza que deseja excluir o produto '%s'? (s/n): ", atual->dados.nomeProduto);
    char confirma;
    scanf(" %c", &confirma);

    if (tolower(confirma) == 's') {
        if (anterior == NULL) {
            listaProdutos = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        free(atual);
        salvarProdutos();
        printf("\n>> Produto excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
    system("pause");
}

void excluirCategoria() {
    system("cls");
    exibirCategorias(false);
    if (!listaCategorias) {
        printf("\nNenhuma categoria para excluir.\n");
        system("pause");
        return;
    }

    int cod;
    printf("\nDigite o codigo da categoria a ser excluida (0 para cancelar): ");
    scanf("%d", &cod);
    if (cod == 0) return;

    NodeProduto* p_aux = listaProdutos;
    while(p_aux != NULL){
        if(p_aux->dados.codigoCategoria == cod){
            printf("\n>> ERRO: Categoria em uso pelo produto '%s' (Cod: %d).\n", p_aux->dados.nomeProduto, p_aux->dados.codigo);
            printf("Nao e possivel excluir uma categoria em uso.\n");
            system("pause");
            return;
        }
        p_aux = p_aux->proximo;
    }

    NodeCategoria *atual = listaCategorias, *anterior = NULL;
    while(atual != NULL && atual->dados.codigo != cod) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\n>> ERRO: Categoria com codigo %d nao encontrada.\n", cod);
        system("pause");
        return;
    }

    printf("\nTem certeza que deseja excluir a categoria '%s'? (s/n): ", atual->dados.nome);
    char confirma;
    scanf(" %c", &confirma);

    if (tolower(confirma) == 's') {
        if (anterior == NULL) {
            listaCategorias = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        free(atual);
        salvarCategorias();
        printf("\n>> Categoria excluida com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
    system("pause");
}

void excluirUsuario() {
    system("cls");
    exibirUsuarios();
    if (!listaUsuarios) {
        printf("\nNenhum usuario para excluir.\n");
        system("pause");
        return;
    }

    int cod;
    printf("\nDigite o codigo do usuario a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    if (cod == 0) return;

    if (cod == 1) {
        printf("\n>> ERRO: O usuario principal (Cod: 1) nao pode ser excluido.\n");
        system("pause");
        return;
    }

    NodeUsuario *atual = listaUsuarios, *anterior = NULL;
    while(atual != NULL && atual->dados.codigo != cod) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\n>> ERRO: Usuario com codigo %d nao encontrado.\n", cod);
        system("pause");
        return;
    }

    printf("\nTem certeza que deseja excluir o usuario '%s'? (s/n): ", atual->dados.nome);
    char confirma;
    scanf(" %c", &confirma);

    if (tolower(confirma) == 's') {
        if (anterior == NULL) {
            listaUsuarios = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        free(atual);
        salvarUsuarios();
        printf("\n>> Usuario excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
    system("pause");
}

// =============================================================================
// FUNÇÕES DE PAGAMENTO (REFINADAS)
// =============================================================================
void menu_pagamento(float valorAPagar, ItemVenda carrinho[], int numItens) {
    float totalPagoDinheiro = 0, totalPagoCartao = 0;
    float restanteAPagar = valorAPagar;
    int opcaoPagamento;
    bool vendaConcluida = false;

    do {
        system("cls");
        printf("|====================================================================|\n");
        printf("| PAGAMENTO DA VENDA - PAGAMENTO MISTO PERMITIDO                     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| TOTAL A PAGAR: R$ %.2f\n", restanteAPagar);
        if ((totalPagoDinheiro + totalPagoCartao) > 0) {
            printf("|--------------------------------------------------------------------|\n");
            printf("| VALOR JA PAGO:\n");
            printf("| - Em Dinheiro: R$ %.2f\n", totalPagoDinheiro);
            printf("| - Em Cartao:   R$ %.2f\n", totalPagoCartao);
        }
        printf("|====================================================================|\n");
        printf("| ESCOLHA UMA OPCAO:\n");
        printf("| 1. Pagar (ou completar) com DINHEIRO\n");
        printf("| 2. Pagar (ou completar) com CARTAO\n");
        printf("| 3. CANCELAR VENDA\n");
        printf("|--------------------------------------------------------------------|\n");
        printf(" OPCAO: ");

        if(scanf("%d", &opcaoPagamento) != 1) opcaoPagamento = 0;
        getchar();

        switch(opcaoPagamento) {
            case 1: { // Pagamento em Dinheiro
                float valorRecebido;
                printf("Informe o valor recebido em dinheiro: R$ ");
                if (scanf("%f", &valorRecebido) != 1 || valorRecebido <= 0) {
                    printf("Valor invalido!\n"); system("pause"); continue;
                }
                getchar();

                if (valorRecebido >= restanteAPagar) {
                    float troco = valorRecebido - restanteAPagar;
                    if (troco > totalCaixa) {
                        printf(">> ERRO: Caixa nao tem troco suficiente (R$ %.2f)! Disponivel: R$ %.2f\n", troco, totalCaixa);
                        system("pause");
                        continue;
                    }
                    totalPagoDinheiro += restanteAPagar;
                    totalCaixa += restanteAPagar;
                    totalCaixa -= troco;
                    restanteAPagar = 0;
                    printf("Troco: R$ %.2f\n", troco);
                } else {
                    totalPagoDinheiro += valorRecebido;
                    totalCaixa += valorRecebido;
                    restanteAPagar -= valorRecebido;
                }
                system("pause");
                break;
            }
            case 2: { // Pagamento com Cartão
                float valorPagamentoCartao;
                printf("Quanto deseja pagar no cartao? (0 para pagar o total restante R$ %.2f): R$ ", restanteAPagar);
                if (scanf("%f", &valorPagamentoCartao) != 1 || valorPagamentoCartao < 0) {
                    printf("Valor invalido!\n"); system("pause"); continue;
                }
                getchar();

                if (valorPagamentoCartao == 0) valorPagamentoCartao = restanteAPagar;

                if (valorPagamentoCartao > restanteAPagar + 0.009) {
                    printf(">> ERRO: Valor no cartao (R$ %.2f) maior que o devido (R$ %.2f)!\n", valorPagamentoCartao, restanteAPagar);
                    system("pause"); continue;
                }

                printf("Processando R$ %.2f no cartao...\n", valorPagamentoCartao);
                printf("Pagamento com cartao APROVADO!\n");
                totalPagoCartao += valorPagamentoCartao;
                restanteAPagar -= valorPagamentoCartao;
                system("pause");
                break;
            }
            case 3:
                printf("Pagamento/Venda cancelada pelo usuario.\n");
                totalCaixa -= totalPagoDinheiro; // Devolve ao caixa o que foi pago em dinheiro
                system("pause");
                return; // Sai da função, a venda não é concluída
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }

        if (restanteAPagar < 0.01f) {
            vendaConcluida = true;
        }

    } while (!vendaConcluida);

    // Finalização da venda e recibo
    system("cls");
    printf("|====================================================================|\n");
    printf("| VENDA #%d FINALIZADA COM SUCESSO!\n", proximoNumeroVenda);
    printf("|====================================================================|\n");

    if (totalPagoDinheiro > 0 && totalPagoCartao > 0) {
        printf("| FORMA DE PAGAMENTO: Misto\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| TOTAL DA VENDA: R$ %.2f\n", valorAPagar);
        printf("| > Pago em Dinheiro: R$ %.2f\n", totalPagoDinheiro);
        printf("| > Pago em Cartao:   R$ %.2f\n", totalPagoCartao);
        numVendasMisto++;
    } else if (totalPagoDinheiro > 0) {
        printf("| FORMA DE PAGAMENTO: Dinheiro\n");
        printf("| TOTAL PAGO: R$ %.2f\n", valorAPagar);
        numVendasDinheiro++;
    } else {
        printf("| FORMA DE PAGAMENTO: Cartao\n");
        printf("| TOTAL PAGO: R$ %.2f\n", valorAPagar);
        numVendasCartao++;
    }
    printf("|====================================================================|\n");

    totalVendas += valorAPagar;
    totalDinheiroF += totalPagoDinheiro;
    totalCartaoF += totalPagoCartao;

    for (int i = 0; i < numItens; i++) {
        NodeProduto *p_aux = listaProdutos;
        while(p_aux) {
            if (p_aux->dados.codigo == carrinho[i].codigoProduto) {
                p_aux->dados.estoque -= carrinho[i].quantidade;
                break;
            }
            p_aux = p_aux->proximo;
        }
    }
    salvarProdutos();
    proximoNumeroVenda++;
    registrarLog("Venda finalizada com sucesso.");
    system("pause");
}


// =============================================================================
// FUNÇÕES DE EXIBIÇÃO (READ)
// =============================================================================

void exibirProdutos() {
    printf("|====================================================================|\n");
    printf("|COD | NOME PRODUTO\t   | CATEGORIA\t | PRECO R$  | ESTOQUE |\n");
    printf("|----|---------------------|---------------|-----------|---------|\n");

    if (listaProdutos == NULL) {
        printf("| NENHUM PRODUTO CADASTRADO.\t\t\t\t\t     |\n");
    } else {
        NodeProduto *aux = listaProdutos;
        while(aux != NULL) {
            const char* nomeCat = obterNomeCategoria(aux->dados.codigoCategoria);
            printf("|%-3d | %-19.19s | %-13.13s | %-9.2f | %-7d |\n",
                   aux->dados.codigo,
                   aux->dados.nomeProduto,
                   nomeCat ? nomeCat : "N/A",
                   aux->dados.precoVenda,
                   aux->dados.estoque);
            aux = aux->proximo;
        }
    }
    printf("|====================================================================|\n");
}

void exibirCategorias(bool comPause) {
    if (!comPause) {
        system("cls");
    }
    printf("|=======================================|\n");
    printf("|\tCATEGORIAS DISPONIVEIS\t\t|\n");
    printf("|=======================================|\n");
    printf("| CODIGO | NOME\t\t\t|\n");
    printf("|--------|-------------------------------|\n");
    if(listaCategorias == NULL){
        printf("| Nenhuma categoria cadastrada.\t\t|\n");
    } else {
        NodeCategoria* aux = listaCategorias;
        while(aux != NULL){
            printf("| %-6d | %-29.29s |\n", aux->dados.codigo, aux->dados.nome);
            aux = aux->proximo;
        }
    }
    printf("|=======================================|\n");
    if(comPause) system("pause");
}

void exibirClientes() {
    system("cls");
    printf("|=======================================================================================|\n");
    printf("|\t\t\t\tCLIENTES CADASTRADOS\t\t\t\t|\n");
    printf("|=======================================================================================|\n");
    printf("|COD | NOME COMPLETO\t\t\t| CPF\t\t   | CELULAR\t\t   |\n");
    printf("|====|=======================================|==================|=======================|\n");

    if (listaClientes == NULL) {
        printf("| NENHUM CLIENTE CADASTRADO AINDA.\t\t\t\t\t\t|\n");
    } else {
        NodeCliente *aux = listaClientes;
        while(aux != NULL) {
            printf("|%-3d | %-37.37s | %-16.16s | %-21.21s |\n",
                   aux->dados.codigo,
                   aux->dados.nomeCompleto,
                   aux->dados.cpf,
                   aux->dados.celular);
            aux = aux->proximo;
        }
    }
    printf("|=======================================================================================|\n");
}

void exibirUsuarios() {
    system("cls");
    printf("|===============================================================|\n");
    printf("|\t\t\tUSUARIOS CADASTRADOS\t\t\t|\n");
    printf("|===============================================================|\n");
    printf("| CODIGO | NOME COMPLETO\t\t\t| LOGIN\t\t|\n");
    printf("|========|=======================================|===============|\n");

    if (listaUsuarios == NULL) {
        printf("| NENHUM USUARIO CADASTRADO.\t\t\t\t\t|\n");
    } else {
        NodeUsuario *aux = listaUsuarios;
        while(aux != NULL) {
            printf("| %-6d | %-37.37s | %-13.13s |\n",
                   aux->dados.codigo,
                   aux->dados.nome,
                   aux->dados.login);
            aux = aux->proximo;
        }
    }
    printf("|===============================================================|\n");
}


// =============================================================================
// FUNÇÕES DE PERSISTÊNCIA (SALVAR/CARREGAR)
// =============================================================================

void carregarTudo() {
    carregarUsuarios();
    carregarClientes();
    carregarCategorias();
    carregarProdutos();
}

void salvarTudo() {
    salvarUsuarios();
    salvarClientes();
    salvarCategorias();
    salvarProdutos();
}

void salvarUsuarios() {
    FILE *f = fopen(ARQUIVO_USUARIOS, "wb");
    if (!f) return;
    for (NodeUsuario *aux = listaUsuarios; aux; aux = aux->proximo) {
        fwrite(&(aux->dados), sizeof(Usuario), 1, f);
    }
    fclose(f);
}

void carregarUsuarios() {
    FILE *f = fopen(ARQUIVO_USUARIOS, "rb");
    if (!f) return;
    liberarListaUsuarios();
    Usuario u;
    while(fread(&u, sizeof(Usuario), 1, f)) {
        NodeUsuario *novo = (NodeUsuario*) malloc(sizeof(NodeUsuario));
        if(!novo) continue;
        novo->dados = u;
        novo->proximo = listaUsuarios;
        listaUsuarios = novo;
    }
    fclose(f);
}

void salvarClientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "wb");
    if (!f) return;
    for(NodeCliente *aux = listaClientes; aux; aux = aux->proximo) {
        fwrite(&(aux->dados), sizeof(Cliente), 1, f);
    }
    fclose(f);
}

void carregarClientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) return;
    liberarListaClientes();
    Cliente c;
    while(fread(&c, sizeof(Cliente), 1, f)) {
        NodeCliente *novo = (NodeCliente*) malloc(sizeof(NodeCliente));
        if(!novo) continue;
        novo->dados = c;
        novo->proximo = listaClientes;
        listaClientes = novo;
    }
    fclose(f);
}

void salvarCategorias() {
    FILE *f = fopen(ARQUIVO_CATEGORIAS, "wb");
    if (!f) return;
    for(NodeCategoria *aux = listaCategorias; aux; aux = aux->proximo) {
        fwrite(&(aux->dados), sizeof(Categoria), 1, f);
    }
    fclose(f);
}

void carregarCategorias() {
    FILE *f = fopen(ARQUIVO_CATEGORIAS, "rb");
    if (!f) return;
    liberarListaCategorias();
    Categoria c;
    while(fread(&c, sizeof(Categoria), 1, f)) {
        NodeCategoria *novo = (NodeCategoria*) malloc(sizeof(NodeCategoria));
        if(!novo) continue;
        novo->dados = c;
        novo->proximo = listaCategorias;
        listaCategorias = novo;
    }
    fclose(f);
}

void salvarProdutos() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "wb");
    if (!f) return;
    for(NodeProduto *aux = listaProdutos; aux; aux = aux->proximo) {
        fwrite(&(aux->dados), sizeof(Produto), 1, f);
    }
    fclose(f);
}

void carregarProdutos() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "rb");
    if (!f) return;
    liberarListaProdutos();
    Produto p;
    while(fread(&p, sizeof(Produto), 1, f)) {
        NodeProduto *novo = (NodeProduto*) malloc(sizeof(NodeProduto));
        if(!novo) continue;
        novo->dados = p;
        novo->proximo = listaProdutos;
        listaProdutos = novo;
    }
    fclose(f);
}

// =============================================================================
// FUNÇÕES DE LIBERAÇÃO DE MEMÓRIA
// =============================================================================

void liberarListas() {
    liberarListaUsuarios();
    liberarListaClientes();
    liberarListaCategorias();
    liberarListaProdutos();
}

void liberarListaUsuarios() {
    NodeUsuario *atual = listaUsuarios;
    while (atual != NULL) {
        NodeUsuario *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaUsuarios = NULL;
}

void liberarListaClientes() {
    NodeCliente *atual = listaClientes;
    while (atual != NULL) {
        NodeCliente *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaClientes = NULL;
}

void liberarListaCategorias() {
    NodeCategoria *atual = listaCategorias;
    while (atual != NULL) {
        NodeCategoria *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaCategorias = NULL;
}

void liberarListaProdutos() {
    NodeProduto *atual = listaProdutos;
    while (atual != NULL) {
        NodeProduto *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaProdutos = NULL;
}

// =============================================================================
// FUNÇÕES AUXILIARES
// =============================================================================
const char* obterNomeCategoria(int codCategoria) {
    NodeCategoria *aux = listaCategorias;
    while (aux != NULL) {
        if (aux->dados.codigo == codCategoria) {
            return aux->dados.nome;
        }
        aux = aux->proximo;
    }
    return NULL;
}

int obterProximoCodigoUsuario() {
    if (listaUsuarios == NULL) return 1;
    int maxCod = 0;
    for(NodeUsuario* aux = listaUsuarios; aux; aux = aux->proximo) {
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo;
    }
    return maxCod + 1;
}

int obterProximoCodigoCliente() {
    if (listaClientes == NULL) return 1;
    int maxCod = 0;
    for(NodeCliente* aux = listaClientes; aux; aux = aux->proximo) {
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo;
    }
    return maxCod + 1;
}

int obterProximoCodigoCategoria() {
    if (listaCategorias == NULL) return 1;
    int maxCod = 0;
    for(NodeCategoria* aux = listaCategorias; aux; aux = aux->proximo) {
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo;
    }
    return maxCod + 1;
}

int obterProximoCodigoProduto() {
    if (listaProdutos == NULL) return 1;
    int maxCod = 0;
    for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo) {
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo;
    }
    return maxCod + 1;
}

bool validarCPF(const char *cpf) {
    char cpfLimpo[12];
    int j = 0;
    for (int i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (isdigit(cpf[i])) {
            cpfLimpo[j++] = cpf[i];
        }
    }
    cpfLimpo[j] = '\0';
    if (j != 11) return false;
    bool todosIguais = true;
    for (int i = 1; i < 11; i++) {
        if (cpfLimpo[i] != cpfLimpo[0]) {
            todosIguais = false;
            break;
        }
    }
    return !todosIguais;
}

void registrarLog(const char *mensagem) {
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    if (local == NULL) return;

    FILE *logFile = fopen(ARQUIVO_LOG, "a");
    if (logFile == NULL) return;

    fprintf(logFile, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
            local->tm_hour, local->tm_min, local->tm_sec,
            mensagem);
    fclose(logFile);
}

void retiradaCaixa() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t   RETIRADA DE CAIXA (SANGRIA)\t\t|\n");
    printf("|=======================================================|\n");
    printf("Saldo atual em caixa: R$ %.2f\n", totalCaixa);

    float valorRetirada;
    printf("Digite o valor a ser retirado (0 para cancelar): R$ ");
    if(scanf("%f", &valorRetirada) != 1 || valorRetirada < 0){
        printf(">> ERRO: Valor invalido!\n");
        system("pause");
        return;
    }
    getchar();

    if(valorRetirada == 0) {
        printf("Operacao cancelada.\n");
        system("pause");
        return;
    }

    if((totalCaixa - valorRetirada) < SALDO_MINIMO){
        printf(">> ERRO: Saldo insuficiente! A retirada deixaria o caixa abaixo do minimo de R$ %.2f\n", SALDO_MINIMO);
        system("pause");
        return;
    }

    totalCaixa -= valorRetirada;
    char logMsg[100];
    sprintf(logMsg, "Retirada de caixa (sangria) de R$ %.2f. Novo saldo: R$ %.2f", valorRetirada, totalCaixa);
    registrarLog(logMsg);

    printf("\n>> Retirada de R$ %.2f realizada com sucesso.\n", valorRetirada);
    printf("Novo saldo do caixa: R$ %.2f\n", totalCaixa);
    system("pause");
}

void verificarEstoqueMinimo() {
    bool alerta = false;
    for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo){
        if(aux->dados.estoque <= aux->dados.estoqueMinimo){
            alerta = true;
            break;
        }
    }
    if(alerta){
        printf("\n\n/!\\ ALERTA DE ESTOQUE BAIXO /!\\\n");
        printf("==============================================================\n");
        printf("Os seguintes produtos estao com estoque baixo ou zerado:\n");
        for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo){
            if(aux->dados.estoque <= aux->dados.estoqueMinimo){
                printf("- %s (Cod: %d) | Estoque: %d | Minimo: %d\n", aux->dados.nomeProduto, aux->dados.codigo, aux->dados.estoque, aux->dados.estoqueMinimo);
            }
        }
        printf("==============================================================\n");
        printf("Pressione ENTER para continuar...\n");
        getchar();
    }
}

void exibirRelatorioVendas() {
    system("cls");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("|=======================================================|\n");
    printf("|\t\tRELATORIO DE VENDAS DO DIA\t\t|\n");
    printf("| Data: %02d/%02d/%04d\t\t\t\t\t|\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    printf("|=======================================================|\n");
    printf("| TOTAL DE VENDAS: %-36d |\n", proximoNumeroVenda - 1);
    printf("|-------------------------------------------------------|\n");
    printf("| QUANTIDADE POR TIPO DE PAGAMENTO:\t\t\t|\n");
    printf("| - Apenas Dinheiro: %-32d |\n", numVendasDinheiro);
    printf("| - Apenas Cartao: %-34d |\n", numVendasCartao);
    printf("| - Misto: %-42d |\n", numVendasMisto);
    printf("|-------------------------------------------------------|\n");
    printf("| FATURAMENTO TOTAL: R$ %-30.2f |\n", totalVendas);
    printf("| - Total Recebido em Dinheiro: R$ %-21.2f |\n", totalDinheiroF);
    printf("| - Total Recebido em Cartao: R$ %-23.2f |\n", totalCartaoF);
    printf("|=======================================================|\n");
    system("pause");
}

void exibirLogs() {
    system("cls");
    FILE *f = fopen(ARQUIVO_LOG, "r");
    printf("|=======================================================|\n");
    printf("|\t\t\tLOGS DO SISTEMA\t\t\t|\n");
    printf("|=======================================================|\n");
    if (f == NULL) {
        printf("| Nenhum log encontrado.\t\t\t\t|\n");
    } else {
        char linha[256];
        while (fgets(linha, sizeof(linha), f)) {
            printf("| %s", linha);
        }
        fclose(f);
    }
    printf("|=======================================================|\n");
    printf("Fim dos logs.\n");
    system("pause");
}

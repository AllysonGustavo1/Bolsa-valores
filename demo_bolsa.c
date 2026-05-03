#include <stdio.h>
#include "Bolsa_Valores_Imp.h"
#include "demo_bolsa.h"

enum {
    DEMO_ERRO = 0,
    DEMO_OK = 1
};

#ifndef OP_CADASTRAR_CLIENTE
#define OP_CADASTRAR_CLIENTE(res, cliente) \
    Bolsa_Valores_Imp__cadastrarCliente(&(res), (cliente))
#endif

#ifndef OP_CADASTRAR_ATIVO
#define OP_CADASTRAR_ATIVO(res, ativo) \
    Bolsa_Valores_Imp__cadastrarAtivo(&(res), (ativo))
#endif

#ifndef OP_DEPOSITAR_SALDO
#define OP_DEPOSITAR_SALDO(res, cliente, valor) \
    Bolsa_Valores_Imp__depositarSaldo(&(res), (cliente), (valor))
#endif

#ifndef OP_CREDITAR_CUSTODIA
#define OP_CREDITAR_CUSTODIA(res, cliente, ativo, quantidade) \
    Bolsa_Valores_Imp__creditarCustodia(&(res), (cliente), (ativo), (quantidade))
#endif

#ifndef OP_ABRIR_ORDEM_COMPRA
#define OP_ABRIR_ORDEM_COMPRA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemCompra(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

#ifndef OP_ABRIR_ORDEM_VENDA
#define OP_ABRIR_ORDEM_VENDA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemVenda(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

static int passo(const char *descricao, int resultado, int esperado) {
    if (resultado == esperado) {
        printf("[OK]   %s\n", descricao);
        return 1;
    }

    printf("[FALHA] %s | retorno=%d | esperado=%d\n",
           descricao, resultado, esperado);
    return 0;
}

int demo_bolsa_popular(void) {
    int r = DEMO_ERRO;

    puts("\nCarregando dados de demonstracao...");

    OP_CADASTRAR_CLIENTE(r, 1);
    if (!passo("Cliente 1 cadastrado (comprador principal)", r, DEMO_OK)) return 0;

    OP_CADASTRAR_CLIENTE(r, 2);
    if (!passo("Cliente 2 cadastrado (vendedor principal)", r, DEMO_OK)) return 0;

    OP_CADASTRAR_CLIENTE(r, 3);
    if (!passo("Cliente 3 cadastrado (comprador secundario)", r, DEMO_OK)) return 0;

    OP_CADASTRAR_CLIENTE(r, 4);
    if (!passo("Cliente 4 cadastrado (vendedor secundario)", r, DEMO_OK)) return 0;

    OP_CADASTRAR_CLIENTE(r, 5);
    if (!passo("Cliente 5 cadastrado (poucos recursos)", r, DEMO_OK)) return 0;

    OP_CADASTRAR_ATIVO(r, 1);
    if (!passo("Ativo 1 cadastrado", r, DEMO_OK)) return 0;

    OP_CADASTRAR_ATIVO(r, 2);
    if (!passo("Ativo 2 cadastrado", r, DEMO_OK)) return 0;

    OP_DEPOSITAR_SALDO(r, 1, 100000);
    if (!passo("Cliente 1 recebeu saldo 100000", r, DEMO_OK)) return 0;

    OP_DEPOSITAR_SALDO(r, 2, 10000);
    if (!passo("Cliente 2 recebeu saldo 10000", r, DEMO_OK)) return 0;

    OP_DEPOSITAR_SALDO(r, 3, 50000);
    if (!passo("Cliente 3 recebeu saldo 50000", r, DEMO_OK)) return 0;

    OP_DEPOSITAR_SALDO(r, 4, 30000);
    if (!passo("Cliente 4 recebeu saldo 30000", r, DEMO_OK)) return 0;

    OP_DEPOSITAR_SALDO(r, 5, 100);
    if (!passo("Cliente 5 recebeu saldo 100", r, DEMO_OK)) return 0;

    OP_CREDITAR_CUSTODIA(r, 2, 1, 100);
    if (!passo("Cliente 2 recebeu 100 unidades do ativo 1", r, DEMO_OK)) return 0;

    OP_CREDITAR_CUSTODIA(r, 4, 1, 50);
    if (!passo("Cliente 4 recebeu 50 unidades do ativo 1", r, DEMO_OK)) return 0;

    OP_CREDITAR_CUSTODIA(r, 4, 2, 80);
    if (!passo("Cliente 4 recebeu 80 unidades do ativo 2", r, DEMO_OK)) return 0;

    /* Caso 1: casamento total. */
    OP_ABRIR_ORDEM_COMPRA(r, 1, 1, 1, 10, 100);
    if (!passo("Ordem 1 aberta: COMPRA C1 A1 qtd 10 preco 100", r, DEMO_OK)) return 0;

    OP_ABRIR_ORDEM_VENDA(r, 2, 2, 1, 10, 90);
    if (!passo("Ordem 2 aberta: VENDA C2 A1 qtd 10 preco 90", r, DEMO_OK)) return 0;

    /* Caso 2: casamento parcial. */
    OP_ABRIR_ORDEM_COMPRA(r, 3, 3, 1, 20, 96);
    if (!passo("Ordem 3 aberta: COMPRA C3 A1 qtd 20 preco 96", r, DEMO_OK)) return 0;

    OP_ABRIR_ORDEM_VENDA(r, 4, 4, 1, 12, 94);
    if (!passo("Ordem 4 aberta: VENDA C4 A1 qtd 12 preco 94", r, DEMO_OK)) return 0;

    /* Caso 3: ordem cancelavel no ativo 2. */
    OP_ABRIR_ORDEM_COMPRA(r, 5, 1, 2, 8, 80);
    if (!passo("Ordem 5 aberta: COMPRA C1 A2 qtd 8 preco 80", r, DEMO_OK)) return 0;

    OP_ABRIR_ORDEM_VENDA(r, 6, 4, 2, 8, 75);
    if (!passo("Ordem 6 aberta: VENDA C4 A2 qtd 8 preco 75", r, DEMO_OK)) return 0;

    /* Caso 4: ordem incompatível por preco. */
    OP_ABRIR_ORDEM_VENDA(r, 7, 2, 1, 15, 110);
    if (!passo("Ordem 7 aberta: VENDA C2 A1 qtd 15 preco 110", r, DEMO_OK)) return 0;

    /*
     * Caso 5: operacao invalida proposital.
     * Cliente 5 tem saldo 100 e tenta comprar 10 * 50 = 500.
     */
    OP_ABRIR_ORDEM_COMPRA(r, 8, 5, 1, 10, 50);
    if (!passo("Rejeicao esperada da ordem 8 por saldo insuficiente", r, DEMO_ERRO)) return 0;

    puts("Carga de demonstracao concluida.\n");
    return 1;
}

void demo_bolsa_roteiro(void) {
    puts("\n================ ROTEIRO RAPIDO ================");
    puts("1) Consultar saldo do cliente 1 e custodia do cliente 2 no ativo 1.");
    puts("2) Executar casamento TOTAL:");
    puts("   opcao 8 -> compra=1, venda=2, quantidade=10, preco=95");
    puts("   esperado: ordens 1 e 2 executadas; C1 recebe ativo; C2 recebe saldo.");
    puts("");
    puts("3) Executar casamento PARCIAL:");
    puts("   opcao 8 -> compra=3, venda=4, quantidade=5, preco=95");
    puts("   esperado: ordens 3 e 4 seguem abertas com quantidade restante.");
    puts("");
    puts("4) Tentar casamento INVALIDO por preco:");
    puts("   opcao 8 -> compra=3, venda=7, quantidade=5, preco=100");
    puts("   esperado: operacao rejeitada, pois venda 7 exige preco 110 e compra 3 paga ate 96.");
    puts("");
    puts("5) Cancelar uma ordem aberta:");
    puts("   opcao 9 -> ordem=5 ou ordem=6");
    puts("   esperado: ordem cancelada e recursos bloqueados devolvidos.");
    puts("");
    puts("6) Consultar status:");
    puts("   opcao 12 -> ordens 1,2,3,4,5,6,7,8");
    puts("================================================\n");
}
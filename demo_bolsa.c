#include <stdint.h>
#include <stdio.h>

#include "bolsa_api.h"
#include "console_ui.h"
#include "demo_bolsa.h"

typedef struct {
    int32_t ordem;
    const char *tipo;
    int32_t cliente;
    int32_t ativo;
    int32_t quantidade;
    int32_t preco;
    const char *observacao;
} OrdemPlanejada;

static void tabela_saldos(const int32_t *clientes, int quantidade) {
    int32_t disponivel = 0;
    int32_t bloqueado = 0;

    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Cliente | Disponivel  | Bloqueado  |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    for (int i = 0; i < quantidade; ++i) {
        bolsa_consultar_saldo(clientes[i], &disponivel, &bloqueado);
        printf("| %7d | %11d | %10d |\n", (int)clientes[i], (int)disponivel, (int)bloqueado);
    }

    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static void tabela_custodias(const int32_t pares[][2], int quantidade) {
    int32_t disponivel = 0;
    int32_t bloqueado = 0;

    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Cliente | Ativo | Disponivel | Bloqueada |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    for (int i = 0; i < quantidade; ++i) {
        bolsa_consultar_custodia(pares[i][0], pares[i][1], &disponivel, &bloqueado);
        printf("| %7d | %5d | %10d | %9d |\n",
               (int)pares[i][0],
               (int)pares[i][1],
               (int)disponivel,
               (int)bloqueado);
    }

    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static void tabela_status_ordens(const int32_t *ordens, int quantidade) {
    int32_t status = 0;

    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Ordem | Status     |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    for (int i = 0; i < quantidade; ++i) {
        bolsa_consultar_status_ordem(ordens[i], &status);
        printf("| %5d | %s%-10s%s |\n",
               (int)ordens[i],
               ui_cor_status(status),
               ui_texto_status(status),
               ui_c(UI_RESET));
    }

    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static void tabela_ordens_planejadas(void) {
    OrdemPlanejada ordens[] = {
        {1,  "compra", 1, 1, 900, 1000, "par total com ordem 2"},
        {2,  "venda",  2, 1, 900,  920, "par total com ordem 1"},
        {3,  "compra", 3, 1, 700,  900, "par parcial com ordem 4"},
        {4,  "venda",  4, 1, 500,  880, "par parcial com ordem 3"},
        {5,  "compra", 1, 2, 200,  500, "preparada para cancelamento"},
        {6,  "venda",  4, 2, 200,  480, "consulta/cancelamento manual"},
        {7,  "venda",  2, 1,  50, 1000, "preco incompativel com ordem 3"},
        {8,  "compra", 5, 1,  50,   50, "rejeitada por saldo insuficiente"},
        {9,  "compra", 6, 2, 400, 1000, "par alto volume com ordem 10"},
        {10, "venda",  4, 2, 300,  900, "par alto volume com ordem 9"}
    };

    printf("%s+-------+--------+---------+-------+------------+-------+--------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Ordem | Tipo   | Cliente | Ativo | Quantidade | Preco | Observacao                           |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+-------+--------+---------+-------+------------+-------+--------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    for (int i = 0; i < (int)(sizeof(ordens) / sizeof(ordens[0])); ++i) {
        printf("| %5d | %-6s | %7d | %5d | %10d | %5d | %-36s |\n",
               (int)ordens[i].ordem,
               ordens[i].tipo,
               (int)ordens[i].cliente,
               (int)ordens[i].ativo,
               (int)ordens[i].quantidade,
               (int)ordens[i].preco,
               ordens[i].observacao);
    }

    printf("%s+-------+--------+---------+-------+------------+-------+--------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static int passo(const char *descricao, int32_t resultado, int32_t esperado) {
    if (resultado == esperado) {
        printf("%s[OK]%s   %s\n", ui_c(UI_GREEN), ui_c(UI_RESET), descricao);
        return 1;
    }

    printf("%s[FALHA]%s %s | retorno=%d | esperado=%d\n",
           ui_c(UI_RED),
           ui_c(UI_RESET),
           descricao,
           (int)resultado,
           (int)esperado);
    return 0;
}

static void estado_cenario_total(void) {
    int32_t clientes[] = {1, 2};
    int32_t custodias[][2] = {{1, 1}, {2, 1}};
    int32_t ordens[] = {1, 2};

    ui_secao("Saldos relacionados");
    tabela_saldos(clientes, 2);

    ui_secao("Custodias relacionadas");
    tabela_custodias(custodias, 2);

    ui_secao("Ordens relacionadas");
    tabela_status_ordens(ordens, 2);
}

static void estado_cenario_parcial(void) {
    int32_t clientes[] = {3, 4};
    int32_t custodias[][2] = {{3, 1}, {4, 1}};
    int32_t ordens[] = {3, 4};

    ui_secao("Saldos relacionados");
    tabela_saldos(clientes, 2);

    ui_secao("Custodias relacionadas");
    tabela_custodias(custodias, 2);

    ui_secao("Ordens relacionadas");
    tabela_status_ordens(ordens, 2);
}

static void estado_cenario_ativo2(void) {
    int32_t clientes[] = {6, 4};
    int32_t custodias[][2] = {{6, 2}, {4, 2}};
    int32_t ordens[] = {9, 10};

    ui_secao("Saldos relacionados");
    tabela_saldos(clientes, 2);

    ui_secao("Custodias relacionadas");
    tabela_custodias(custodias, 2);

    ui_secao("Ordens relacionadas");
    tabela_status_ordens(ordens, 2);
}

int demo_bolsa_popular(void) {
    int32_t r = BOLSA_ERRO;

    ui_titulo("Carga de demonstracao");

    r = bolsa_cadastrar_cliente(1); if (!passo("Cliente 1 cadastrado (comprador institucional)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_cliente(2); if (!passo("Cliente 2 cadastrado (vendedor principal)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_cliente(3); if (!passo("Cliente 3 cadastrado (comprador secundario)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_cliente(4); if (!passo("Cliente 4 cadastrado (vendedor multiativo)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_cliente(5); if (!passo("Cliente 5 cadastrado (saldo baixo)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_cliente(6); if (!passo("Cliente 6 cadastrado (comprador do ativo 2)", r, BOLSA_OK)) return 0;

    r = bolsa_cadastrar_ativo(1); if (!passo("Ativo 1 cadastrado (ativo principal caro)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_ativo(2); if (!passo("Ativo 2 cadastrado (ativo secundario caro)", r, BOLSA_OK)) return 0;
    r = bolsa_cadastrar_ativo(3); if (!passo("Ativo 3 cadastrado (reserva para testes manuais)", r, BOLSA_OK)) return 0;

    r = bolsa_depositar_saldo(1, 1000000); if (!passo("Cliente 1 recebeu saldo 1.000.000", r, BOLSA_OK)) return 0;
    r = bolsa_depositar_saldo(2, 50000);   if (!passo("Cliente 2 recebeu saldo 50.000", r, BOLSA_OK)) return 0;
    r = bolsa_depositar_saldo(3, 800000);  if (!passo("Cliente 3 recebeu saldo 800.000", r, BOLSA_OK)) return 0;
    r = bolsa_depositar_saldo(4, 100000);  if (!passo("Cliente 4 recebeu saldo 100.000", r, BOLSA_OK)) return 0;
    r = bolsa_depositar_saldo(5, 500);     if (!passo("Cliente 5 recebeu saldo 500", r, BOLSA_OK)) return 0;
    r = bolsa_depositar_saldo(6, 600000);  if (!passo("Cliente 6 recebeu saldo 600.000", r, BOLSA_OK)) return 0;

    r = bolsa_creditar_custodia(2, 1, 1000); if (!passo("Cliente 2 recebeu 1.000 unidades do ativo 1", r, BOLSA_OK)) return 0;
    r = bolsa_creditar_custodia(2, 2, 400);  if (!passo("Cliente 2 recebeu 400 unidades do ativo 2", r, BOLSA_OK)) return 0;
    r = bolsa_creditar_custodia(4, 1, 600);  if (!passo("Cliente 4 recebeu 600 unidades do ativo 1", r, BOLSA_OK)) return 0;
    r = bolsa_creditar_custodia(4, 2, 1000); if (!passo("Cliente 4 recebeu 1.000 unidades do ativo 2", r, BOLSA_OK)) return 0;

    r = bolsa_abrir_ordem_compra(1, 1, 1, 900, 1000); if (!passo("Ordem 1 COMPRA: C1, A1, qtd 900, preco limite 1000", r, BOLSA_OK)) return 0;
    r = bolsa_abrir_ordem_venda(2, 2, 1, 900, 920);   if (!passo("Ordem 2 VENDA: C2, A1, qtd 900, preco minimo 920", r, BOLSA_OK)) return 0;

    r = bolsa_abrir_ordem_compra(3, 3, 1, 700, 900); if (!passo("Ordem 3 COMPRA: C3, A1, qtd 700, preco limite 900", r, BOLSA_OK)) return 0;
    r = bolsa_abrir_ordem_venda(4, 4, 1, 500, 880);  if (!passo("Ordem 4 VENDA: C4, A1, qtd 500, preco minimo 880", r, BOLSA_OK)) return 0;

    r = bolsa_abrir_ordem_compra(5, 1, 2, 200, 500); if (!passo("Ordem 5 COMPRA: C1, A2, qtd 200, preco limite 500", r, BOLSA_OK)) return 0;
    r = bolsa_abrir_ordem_venda(6, 4, 2, 200, 480);  if (!passo("Ordem 6 VENDA: C4, A2, qtd 200, preco minimo 480", r, BOLSA_OK)) return 0;

    r = bolsa_abrir_ordem_venda(7, 2, 1, 50, 1000); if (!passo("Ordem 7 VENDA: C2, A1, qtd 50, preco minimo 1000", r, BOLSA_OK)) return 0;

    r = bolsa_abrir_ordem_compra(8, 5, 1, 50, 50);
    if (!passo("Rejeicao esperada da ordem 8: C5 nao tem saldo para 50 * 50", r, BOLSA_ERRO_ABERTURA_COMPRA_INVALIDA)) return 0;

    r = bolsa_abrir_ordem_compra(9, 6, 2, 400, 1000); if (!passo("Ordem 9 COMPRA: C6, A2, qtd 400, preco limite 1000", r, BOLSA_OK)) return 0;
    r = bolsa_abrir_ordem_venda(10, 4, 2, 300, 900);  if (!passo("Ordem 10 VENDA: C4, A2, qtd 300, preco minimo 900", r, BOLSA_OK)) return 0;

    ui_sucesso("Demonstracao carregada com sucesso.");
    return 1;
}

void demo_bolsa_roteiro(void) {
    ui_titulo("Roteiro de demonstracao");

    printf("%s+----------+--------------------------------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Comando  | Acao                                                         |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+----------+--------------------------------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("| %-8s | %-60s |\n", "estado", "mostra saldos, custodias e status das ordens");
    printf("| %-8s | %-60s |\n", "total", "executa compra=1, venda=2, qtd=900, preco=950");
    printf("| %-8s | %-60s |\n", "parcial", "executa compra=3, venda=4, qtd=250, preco=890");
    printf("| %-8s | %-60s |\n", "invalida", "tenta compra=3, venda=7, qtd=50, preco=950");
    printf("| %-8s | %-60s |\n", "cancelar", "cancela a ordem 5");
    printf("| %-8s | %-60s |\n", "ativo2", "executa compra=9, venda=10, qtd=300, preco=950");
    printf("| %-8s | %-60s |\n", "reset", "reinicia a maquina e recarrega a demonstracao");
    printf("%s+----------+--------------------------------------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    ui_secao("Valores esperados");
    puts("total    -> bloqueio inicial de 900.000, execucao de 855.000 e devolucao de 45.000.");
    puts("parcial  -> ordens 3 e 4 continuam abertas com quantidades restantes.");
    puts("invalida -> operacao rejeitada por incompatibilidade de preco.");
}

void demo_bolsa_estado_resumido(void) {
    int32_t clientes[] = {1, 2, 3, 4, 5, 6};
    int32_t pares[][2] = {
        {1, 1}, {2, 1}, {3, 1}, {4, 1},
        {1, 2}, {2, 2}, {4, 2}, {6, 2}
    };
    int32_t ordens[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ui_titulo("Estado resumido");
    ui_secao("Saldos");
    tabela_saldos(clientes, 6);

    ui_secao("Custodias");
    tabela_custodias(pares, 8);

    ui_secao("Ordens planejadas");
    tabela_ordens_planejadas();

    ui_secao("Status das ordens");
    tabela_status_ordens(ordens, 10);
}

int32_t demo_bolsa_executar_total(void) {
    int32_t resultado = BOLSA_ERRO;

    ui_titulo("Casamento total de alto valor");
    puts("Operacao: compra=1, venda=2, quantidade=900, preco=950.");

    ui_secao("Antes");
    estado_cenario_total();

    resultado = bolsa_executar_casamento(1, 2, 900, 950);

    ui_secao("Resultado");
    ui_resultado_operacao(resultado);
    printf("%sMotivo:%s %s\n", ui_c(UI_BOLD), ui_c(UI_RESET), bolsa_descricao_resultado(resultado));

    ui_secao("Depois");
    estado_cenario_total();

    return resultado;
}

int32_t demo_bolsa_executar_parcial(void) {
    int32_t resultado = BOLSA_ERRO;

    ui_titulo("Casamento parcial");
    puts("Operacao: compra=3, venda=4, quantidade=250, preco=890.");

    ui_secao("Antes");
    estado_cenario_parcial();

    resultado = bolsa_executar_casamento(3, 4, 250, 890);

    ui_secao("Resultado");
    ui_resultado_operacao(resultado);
    printf("%sMotivo:%s %s\n", ui_c(UI_BOLD), ui_c(UI_RESET), bolsa_descricao_resultado(resultado));

    ui_secao("Depois");
    estado_cenario_parcial();

    return resultado;
}

int32_t demo_bolsa_tentar_casamento_invalido(void) {
    int32_t resultado = BOLSA_ERRO;
    int32_t ordens[] = {3, 7};

    ui_titulo("Tentativa invalida por preco");
    puts("Operacao: compra=3, venda=7, quantidade=50, preco=950.");
    ui_alerta("Esperado: ERRO, pois a compra 3 aceita ate 900 e a venda 7 exige 1000.");

    ui_secao("Antes");
    estado_cenario_parcial();
    tabela_status_ordens(ordens, 2);

    resultado = bolsa_executar_casamento(3, 7, 50, 950);

    ui_secao("Resultado");
    ui_resultado_operacao(resultado);
    printf("%sMotivo:%s %s\n", ui_c(UI_BOLD), ui_c(UI_RESET), bolsa_descricao_resultado(resultado));

    ui_secao("Depois");
    estado_cenario_parcial();
    tabela_status_ordens(ordens, 2);

    return resultado;
}

int32_t demo_bolsa_cancelar_ordem_padrao(void) {
    int32_t resultado = BOLSA_ERRO;
    int32_t clientes[] = {1};
    int32_t ordens[] = {5};

    ui_titulo("Cancelamento da ordem 5");

    ui_secao("Antes");
    tabela_saldos(clientes, 1);
    tabela_status_ordens(ordens, 1);

    resultado = bolsa_cancelar_ordem(5);

    ui_secao("Resultado");
    ui_resultado_operacao(resultado);
    printf("%sMotivo:%s %s\n", ui_c(UI_BOLD), ui_c(UI_RESET), bolsa_descricao_resultado(resultado));

    ui_secao("Depois");
    tabela_saldos(clientes, 1);
    tabela_status_ordens(ordens, 1);

    return resultado;
}

int32_t demo_bolsa_executar_ativo2(void) {
    int32_t resultado = BOLSA_ERRO;

    ui_titulo("Casamento de alto volume no ativo 2");
    puts("Operacao: compra=9, venda=10, quantidade=300, preco=950.");

    ui_secao("Antes");
    estado_cenario_ativo2();

    resultado = bolsa_executar_casamento(9, 10, 300, 950);

    ui_secao("Resultado");
    ui_resultado_operacao(resultado);
    printf("%sMotivo:%s %s\n", ui_c(UI_BOLD), ui_c(UI_RESET), bolsa_descricao_resultado(resultado));

    ui_secao("Depois");
    estado_cenario_ativo2();

    return resultado;
}
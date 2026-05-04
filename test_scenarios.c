#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bolsa_api.h"

static int failures = 0;

static void expect_i32(const char *label, int32_t got, int32_t expected) {
    if (got != expected) {
        printf("[FAIL] %-45s got=%d expected=%d\n", label, (int)got, (int)expected);
        failures++;
    } else {
        printf("[ OK ] %-45s %d\n", label, (int)got);
    }
}

static void expect_saldo(int32_t cliente, int32_t esperado_disp, int32_t esperado_bloq) {
    int32_t disp = -1;
    int32_t bloq = -1;
    char label[96];

    bolsa_consultar_saldo(cliente, &disp, &bloq);
    snprintf(label, sizeof(label), "cliente %d saldo disponivel", (int)cliente);
    expect_i32(label, disp, esperado_disp);
    snprintf(label, sizeof(label), "cliente %d saldo bloqueado", (int)cliente);
    expect_i32(label, bloq, esperado_bloq);
}

static void expect_custodia(int32_t cliente, int32_t ativo, int32_t esperado_disp, int32_t esperado_bloq) {
    int32_t disp = -1;
    int32_t bloq = -1;
    char label[96];

    bolsa_consultar_custodia(cliente, ativo, &disp, &bloq);
    snprintf(label, sizeof(label), "cliente %d ativo %d custodia disp", (int)cliente, (int)ativo);
    expect_i32(label, disp, esperado_disp);
    snprintf(label, sizeof(label), "cliente %d ativo %d custodia bloq", (int)cliente, (int)ativo);
    expect_i32(label, bloq, esperado_bloq);
}

static void expect_status(int32_t ordem, int32_t esperado) {
    int32_t status = -1;
    char label[96];

    bolsa_consultar_status_ordem(ordem, &status);
    snprintf(label, sizeof(label), "ordem %d status", (int)ordem);
    expect_i32(label, status, esperado);
}

static void assert_ok(const char *label, int32_t r) {
    expect_i32(label, r, BOLSA_OK);
}

static void assert_erro(const char *label, int32_t r) {
    expect_i32(label, r, BOLSA_ERRO);
}

static void popular_base(void) {
    assert_ok("cadastrar cliente 1", bolsa_cadastrar_cliente(1));
    assert_ok("cadastrar cliente 2", bolsa_cadastrar_cliente(2));
    assert_ok("cadastrar cliente 3", bolsa_cadastrar_cliente(3));
    assert_ok("cadastrar cliente 4", bolsa_cadastrar_cliente(4));
    assert_ok("cadastrar cliente 5", bolsa_cadastrar_cliente(5));
    assert_ok("cadastrar cliente 6", bolsa_cadastrar_cliente(6));

    assert_ok("cadastrar ativo 1", bolsa_cadastrar_ativo(1));
    assert_ok("cadastrar ativo 2", bolsa_cadastrar_ativo(2));
    assert_ok("cadastrar ativo 3", bolsa_cadastrar_ativo(3));

    assert_ok("depositar cliente 1", bolsa_depositar_saldo(1, 1000000));
    assert_ok("depositar cliente 2", bolsa_depositar_saldo(2, 50000));
    assert_ok("depositar cliente 3", bolsa_depositar_saldo(3, 800000));
    assert_ok("depositar cliente 4", bolsa_depositar_saldo(4, 100000));
    assert_ok("depositar cliente 5", bolsa_depositar_saldo(5, 500));
    assert_ok("depositar cliente 6", bolsa_depositar_saldo(6, 600000));

    assert_ok("creditar C2 A1", bolsa_creditar_custodia(2, 1, 1000));
    assert_ok("creditar C2 A2", bolsa_creditar_custodia(2, 2, 400));
    assert_ok("creditar C4 A1", bolsa_creditar_custodia(4, 1, 600));
    assert_ok("creditar C4 A2", bolsa_creditar_custodia(4, 2, 1000));

    assert_ok("abrir compra ordem 1", bolsa_abrir_ordem_compra(1, 1, 1, 900, 1000));
    assert_ok("abrir venda ordem 2", bolsa_abrir_ordem_venda(2, 2, 1, 900, 920));
    assert_ok("abrir compra ordem 3", bolsa_abrir_ordem_compra(3, 3, 1, 700, 900));
    assert_ok("abrir venda ordem 4", bolsa_abrir_ordem_venda(4, 4, 1, 500, 880));
    assert_ok("abrir compra ordem 5", bolsa_abrir_ordem_compra(5, 1, 2, 200, 500));
    assert_ok("abrir venda ordem 6", bolsa_abrir_ordem_venda(6, 4, 2, 200, 480));
    assert_ok("abrir venda ordem 7", bolsa_abrir_ordem_venda(7, 2, 1, 50, 1000));
    assert_erro("rejeitar ordem 8 por saldo insuficiente", bolsa_abrir_ordem_compra(8, 5, 1, 50, 50));
    assert_ok("abrir compra ordem 9", bolsa_abrir_ordem_compra(9, 6, 2, 400, 1000));
    assert_ok("abrir venda ordem 10", bolsa_abrir_ordem_venda(10, 4, 2, 300, 900));
}

static void verificar_estado_inicial_demo(void) {
    printf("\n== Estado inicial apos popular base ==\n");
    expect_saldo(1, 0, 1000000);
    expect_saldo(2, 50000, 0);
    expect_saldo(3, 170000, 630000);
    expect_saldo(4, 100000, 0);
    expect_saldo(5, 500, 0);
    expect_saldo(6, 200000, 400000);

    expect_custodia(1, 1, 0, 0);
    expect_custodia(2, 1, 50, 950);
    expect_custodia(3, 1, 0, 0);
    expect_custodia(4, 1, 100, 500);
    expect_custodia(1, 2, 0, 0);
    expect_custodia(2, 2, 400, 0);
    expect_custodia(4, 2, 500, 500);
    expect_custodia(6, 2, 0, 0);

    for (int32_t o = 1; o <= 7; ++o) {
        expect_status(o, 1);
    }
    expect_status(8, 0);
    expect_status(9, 1);
    expect_status(10, 1);
}

int main(void) {
    bolsa_inicializar();
    popular_base();
    verificar_estado_inicial_demo();

    printf("\n== Casamento total: ordem 1 x 2 ==\n");
    assert_ok("executar casamento total", bolsa_executar_casamento(1, 2, 900, 950));
    expect_saldo(1, 45000, 100000);
    expect_saldo(2, 905000, 0);
    expect_custodia(1, 1, 900, 0);
    expect_custodia(2, 1, 50, 50);
    expect_status(1, 3);
    expect_status(2, 3);

    printf("\n== Casamento parcial: ordem 3 x 4 ==\n");
    assert_ok("executar casamento parcial", bolsa_executar_casamento(3, 4, 250, 890));
    expect_saldo(3, 172500, 405000);
    expect_saldo(4, 322500, 0);
    expect_custodia(3, 1, 250, 0);
    expect_custodia(4, 1, 100, 250);
    expect_status(3, 1);
    expect_status(4, 1);

    printf("\n== Tentativa invalida por preco ==\n");
    assert_erro("rejeitar casamento por preco", bolsa_executar_casamento(3, 7, 50, 950));
    expect_saldo(3, 172500, 405000);
    expect_saldo(2, 905000, 0);
    expect_custodia(2, 1, 50, 50);
    expect_status(3, 1);
    expect_status(7, 1);

    printf("\n== Cancelamento da ordem 5 ==\n");
    assert_ok("cancelar ordem 5", bolsa_cancelar_ordem(5));
    expect_saldo(1, 145000, 0);
    expect_status(5, 2);

    printf("\n== Casamento do ativo 2: ordem 9 x 10 ==\n");
    assert_ok("executar casamento ativo 2", bolsa_executar_casamento(9, 10, 300, 950));
    expect_saldo(6, 215000, 100000);
    expect_saldo(4, 607500, 0);
    expect_custodia(6, 2, 300, 0);
    expect_custodia(4, 2, 500, 200);
    expect_status(9, 1);
    expect_status(10, 3);

    printf("\nFAILS=%d\n", failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
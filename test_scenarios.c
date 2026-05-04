#include <stdint.h>
#include <stdio.h>

#include "bolsa_api.h"

static int fails = 0;

static void assert_eq(const char *label, int32_t actual, int32_t expected) {
    if (actual != expected) {
        printf("[FAIL] %s | atual=%d esperado=%d (%s)\n",
               label, (int)actual, (int)expected, bolsa_descricao_resultado(actual));
        fails++;
    } else {
        printf("[OK]   %s | %d (%s)\n",
               label, (int)actual, bolsa_descricao_resultado(actual));
    }
}

static void assert_not_ok(const char *label, int32_t actual) {
    if (actual == BOLSA_OK) {
        printf("[FAIL] %s | operacao deveria ser rejeitada\n", label);
        fails++;
    } else {
        printf("[OK]   %s | rejeitada com codigo %d (%s)\n",
               label, (int)actual, bolsa_descricao_resultado(actual));
    }
}

int main(void) {
    int32_t status = 0;

    bolsa_inicializar();

    assert_eq("cadastrar cliente 1", bolsa_cadastrar_cliente(1), BOLSA_OK);
    assert_eq("cadastrar cliente 2", bolsa_cadastrar_cliente(2), BOLSA_OK);
    assert_eq("cadastrar cliente duplicado", bolsa_cadastrar_cliente(1), BOLSA_ERRO_CLIENTE_JA_CADASTRADO);

    assert_eq("cadastrar ativo 1", bolsa_cadastrar_ativo(1), BOLSA_OK);
    assert_eq("cadastrar ativo duplicado", bolsa_cadastrar_ativo(1), BOLSA_ERRO_ATIVO_JA_CADASTRADO);

    assert_eq("depositar saldo", bolsa_depositar_saldo(1, 100000), BOLSA_OK);
    assert_not_ok("depositar em cliente inexistente", bolsa_depositar_saldo(3, 1000));

    assert_eq("creditar custodia", bolsa_creditar_custodia(2, 1, 100), BOLSA_OK);

    assert_eq("abrir compra", bolsa_abrir_ordem_compra(1, 1, 1, 10, 100), BOLSA_OK);
    assert_eq("abrir venda", bolsa_abrir_ordem_venda(2, 2, 1, 10, 90), BOLSA_OK);
    assert_eq("reabrir ordem ocupada", bolsa_abrir_ordem_compra(1, 1, 1, 1, 100), BOLSA_ERRO_ABERTURA_COMPRA_INVALIDA);

    assert_eq("executar casamento total", bolsa_executar_casamento(1, 2, 10, 95), BOLSA_OK);

    bolsa_consultar_status_ordem(1, &status);
    assert_eq("status ordem compra executada", status, 3);

    bolsa_consultar_status_ordem(2, &status);
    assert_eq("status ordem venda executada", status, 3);

    assert_eq("cancelar ordem ja executada", bolsa_cancelar_ordem(1), BOLSA_ERRO_ORDEM_NAO_ABERTA);

    printf("FAILS=%d\n", fails);
    return fails == 0 ? 0 : 1;
}

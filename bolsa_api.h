#ifndef BOLSA_API_H
#define BOLSA_API_H

#ifndef Bolsa_Contexto__ERRO
#define Bolsa_Contexto__ERRO 0
#endif

#ifndef Bolsa_Contexto__OK
#define Bolsa_Contexto__OK 1
#endif

#include <stdint.h>
#include "Bolsa_Valores_Abstract.h"

enum {
    BOLSA_ERRO = 0,
    BOLSA_OK = 1,

    BOLSA_MIN_CLIENTE = 1,
    BOLSA_MAX_CLIENTE = 20,

    BOLSA_MIN_ATIVO = 1,
    BOLSA_MAX_ATIVO = 10,

    BOLSA_MIN_ORDEM = 1,
    BOLSA_MAX_ORDEM = 50,

    BOLSA_MIN_VALOR = 1,
    BOLSA_MAX_DINHEIRO = 1000000,
    BOLSA_MAX_QUANTIDADE = 1000,
    BOLSA_MAX_PRECO = 1000
};

static inline void bolsa_inicializar(void) {
    Bolsa_Valores_Abstract__INITIALISATION();
}

static inline int32_t bolsa_cadastrar_cliente(int32_t cliente_id) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__cadastrarCliente(cliente_id, &resultado);
    return resultado;
}

static inline int32_t bolsa_cadastrar_ativo(int32_t ativo_id) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__cadastrarAtivo(ativo_id, &resultado);
    return resultado;
}

static inline int32_t bolsa_depositar_saldo(int32_t cliente_id, int32_t valor) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__depositarSaldo(cliente_id, valor, &resultado);
    return resultado;
}

static inline int32_t bolsa_retirar_saldo(int32_t cliente_id, int32_t valor) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__retirarSaldo(cliente_id, valor, &resultado);
    return resultado;
}

static inline int32_t bolsa_creditar_custodia(int32_t cliente_id, int32_t ativo_id, int32_t quantidade) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__creditarCustodia(cliente_id, ativo_id, quantidade, &resultado);
    return resultado;
}

static inline int32_t bolsa_abrir_ordem_compra(
    int32_t ordem_id,
    int32_t cliente_id,
    int32_t ativo_id,
    int32_t quantidade,
    int32_t preco_limite
) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__abrirOrdemCompra(ordem_id, cliente_id, ativo_id, quantidade, preco_limite, &resultado);
    return resultado;
}

static inline int32_t bolsa_abrir_ordem_venda(
    int32_t ordem_id,
    int32_t cliente_id,
    int32_t ativo_id,
    int32_t quantidade,
    int32_t preco_limite
) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__abrirOrdemVenda(ordem_id, cliente_id, ativo_id, quantidade, preco_limite, &resultado);
    return resultado;
}

static inline int32_t bolsa_executar_casamento(
    int32_t ordem_compra,
    int32_t ordem_venda,
    int32_t quantidade,
    int32_t preco_execucao
) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__executarCasamento(ordem_compra, ordem_venda, quantidade, preco_execucao, &resultado);
    return resultado;
}

static inline int32_t bolsa_cancelar_ordem(int32_t ordem_id) {
    int32_t resultado = BOLSA_ERRO;
    Bolsa_Valores_Abstract__cancelarOrdem(ordem_id, &resultado);
    return resultado;
}

static inline void bolsa_consultar_saldo(int32_t cliente_id, int32_t *valor_disponivel, int32_t *valor_bloqueado) {
    Bolsa_Valores_Abstract__consultarSaldo(cliente_id, valor_disponivel, valor_bloqueado);
}

static inline void bolsa_consultar_custodia(
    int32_t cliente_id,
    int32_t ativo_id,
    int32_t *quantidade_disponivel,
    int32_t *quantidade_bloqueada
) {
    Bolsa_Valores_Abstract__consultarCustodia(cliente_id, ativo_id, quantidade_disponivel, quantidade_bloqueada);
}

static inline void bolsa_consultar_status_ordem(int32_t ordem_id, int32_t *status_codigo) {
    Bolsa_Valores_Abstract__consultarStatusOrdem(ordem_id, status_codigo);
}

#endif

#ifndef BOLSA_API_H
#define BOLSA_API_H

#ifndef Bolsa_Contexto__ERRO
#define Bolsa_Contexto__ERRO 0
#endif

#ifndef Bolsa_Contexto__OK
#define Bolsa_Contexto__OK 1
#endif

#ifndef Bolsa_Contexto__ERRO_CLIENTE_JA_CADASTRADO
#define Bolsa_Contexto__ERRO_CLIENTE_JA_CADASTRADO 2
#endif

#ifndef Bolsa_Contexto__ERRO_ATIVO_JA_CADASTRADO
#define Bolsa_Contexto__ERRO_ATIVO_JA_CADASTRADO 3
#endif

#ifndef Bolsa_Contexto__ERRO_DEPOSITO_INVALIDO
#define Bolsa_Contexto__ERRO_DEPOSITO_INVALIDO 4
#endif

#ifndef Bolsa_Contexto__ERRO_SALDO_INSUFICIENTE
#define Bolsa_Contexto__ERRO_SALDO_INSUFICIENTE 5
#endif

#ifndef Bolsa_Contexto__ERRO_CUSTODIA_INVALIDA
#define Bolsa_Contexto__ERRO_CUSTODIA_INVALIDA 6
#endif

#ifndef Bolsa_Contexto__ERRO_ABERTURA_COMPRA_INVALIDA
#define Bolsa_Contexto__ERRO_ABERTURA_COMPRA_INVALIDA 7
#endif

#ifndef Bolsa_Contexto__ERRO_ABERTURA_VENDA_INVALIDA
#define Bolsa_Contexto__ERRO_ABERTURA_VENDA_INVALIDA 8
#endif

#ifndef Bolsa_Contexto__ERRO_CASAMENTO_INVALIDO
#define Bolsa_Contexto__ERRO_CASAMENTO_INVALIDO 9
#endif

#ifndef Bolsa_Contexto__ERRO_LIMITE_EXCEDIDO
#define Bolsa_Contexto__ERRO_LIMITE_EXCEDIDO 10
#endif

#ifndef Bolsa_Contexto__ERRO_ORDEM_NAO_ABERTA
#define Bolsa_Contexto__ERRO_ORDEM_NAO_ABERTA 11
#endif

#include <stdint.h>
#include "Bolsa_Valores_Abstract.h"

enum {
    BOLSA_ERRO = 0,
    BOLSA_OK = 1,
    BOLSA_ERRO_CLIENTE_JA_CADASTRADO = 2,
    BOLSA_ERRO_ATIVO_JA_CADASTRADO = 3,
    BOLSA_ERRO_DEPOSITO_INVALIDO = 4,
    BOLSA_ERRO_SALDO_INSUFICIENTE = 5,
    BOLSA_ERRO_CUSTODIA_INVALIDA = 6,
    BOLSA_ERRO_ABERTURA_COMPRA_INVALIDA = 7,
    BOLSA_ERRO_ABERTURA_VENDA_INVALIDA = 8,
    BOLSA_ERRO_CASAMENTO_INVALIDO = 9,
    BOLSA_ERRO_LIMITE_EXCEDIDO = 10,
    BOLSA_ERRO_ORDEM_NAO_ABERTA = 11,

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


static inline const char *bolsa_descricao_resultado(int32_t resultado) {
    switch (resultado) {
        case BOLSA_OK:
            return "Operacao realizada com sucesso";
        case BOLSA_ERRO:
            return "Operacao rejeitada";
        case BOLSA_ERRO_CLIENTE_JA_CADASTRADO:
            return "Cliente ja cadastrado";
        case BOLSA_ERRO_ATIVO_JA_CADASTRADO:
            return "Ativo ja cadastrado";
        case BOLSA_ERRO_DEPOSITO_INVALIDO:
            return "Deposito rejeitado: cliente inexistente ou limite maximo excedido";
        case BOLSA_ERRO_SALDO_INSUFICIENTE:
            return "Saldo disponivel insuficiente ou cliente inexistente";
        case BOLSA_ERRO_CUSTODIA_INVALIDA:
            return "Custodia rejeitada: cliente/ativo inexistente ou limite excedido";
        case BOLSA_ERRO_ABERTURA_COMPRA_INVALIDA:
            return "Compra rejeitada: ordem indisponivel, cliente/ativo inexistente, saldo insuficiente ou limite excedido";
        case BOLSA_ERRO_ABERTURA_VENDA_INVALIDA:
            return "Venda rejeitada: ordem indisponivel, cliente/ativo inexistente, custodia insuficiente ou limite excedido";
        case BOLSA_ERRO_CASAMENTO_INVALIDO:
            return "Casamento rejeitado: ordens incompativeis ou recursos insuficientes";
        case BOLSA_ERRO_LIMITE_EXCEDIDO:
            return "Operacao rejeitada por limite interno ou recurso bloqueado insuficiente";
        case BOLSA_ERRO_ORDEM_NAO_ABERTA:
            return "Ordem nao esta aberta";
        default:
            return "Codigo de resultado desconhecido";
    }
}
#endif

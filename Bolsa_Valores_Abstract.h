#ifndef _Bolsa_Valores_Abstract_h
#define _Bolsa_Valores_Abstract_h

#include <stdint.h>
#include <stdbool.h>
/* Clause SEES */
#include "Bolsa_Contexto.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Clause SETS */

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */
/* Array and record constants */

/* Clause CONCRETE_VARIABLES */

extern void Bolsa_Valores_Abstract__INITIALISATION(void);

/* Clause OPERATIONS */

extern void Bolsa_Valores_Abstract__cadastrarCliente(int32_t clienteId, int32_t *resultado);
extern void Bolsa_Valores_Abstract__cadastrarAtivo(int32_t ativoId, int32_t *resultado);
extern void Bolsa_Valores_Abstract__depositarSaldo(int32_t clienteId, int32_t valorDeposito, int32_t *resultado);
extern void Bolsa_Valores_Abstract__retirarSaldo(int32_t clienteId, int32_t valorRetirada, int32_t *resultado);
extern void Bolsa_Valores_Abstract__creditarCustodia(int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t *resultado);
extern void Bolsa_Valores_Abstract__abrirOrdemCompra(int32_t ordemId, int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t precoLimite, int32_t *resultado);
extern void Bolsa_Valores_Abstract__abrirOrdemVenda(int32_t ordemId, int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t precoLimite, int32_t *resultado);
extern void Bolsa_Valores_Abstract__executarCasamento(int32_t ordemCompra, int32_t ordemVenda, int32_t quantidadeExecucao, int32_t precoExecucao, int32_t *resultado);
extern void Bolsa_Valores_Abstract__cancelarOrdem(int32_t ordemId, int32_t *resultado);
extern void Bolsa_Valores_Abstract__consultarSaldo(int32_t clienteId, int32_t *valorDisponivel, int32_t *valorBloqueado);
extern void Bolsa_Valores_Abstract__consultarCustodia(int32_t clienteId, int32_t ativoId, int32_t *quantidadeDisponivel, int32_t *quantidadeBloqueada);
extern void Bolsa_Valores_Abstract__consultarStatusOrdem(int32_t ordemId, int32_t *statusCodigo);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _Bolsa_Valores_Abstract_h */

#ifndef _Bolsa_Contexto_h
#define _Bolsa_Contexto_h

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Clause SETS */
typedef enum
{
    Bolsa_Contexto__lado_compra,
    Bolsa_Contexto__lado_venda
    
} Bolsa_Contexto__TIPO_LADO;
#define Bolsa_Contexto__TIPO_LADO__max 2
typedef enum
{
    Bolsa_Contexto__status_livre,
    Bolsa_Contexto__status_aberta,
    Bolsa_Contexto__status_cancelada,
    Bolsa_Contexto__status_executada
    
} Bolsa_Contexto__STATUS_ORDEM;
#define Bolsa_Contexto__STATUS_ORDEM__max 4

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */
/* TO DO: #define Bolsa_Contexto__MAX_CLIENTES */
/* TO DO: #define Bolsa_Contexto__MAX_ATIVOS */
/* TO DO: #define Bolsa_Contexto__MAX_ORDENS */
/* TO DO: #define Bolsa_Contexto__MAX_DINHEIRO */
/* TO DO: #define Bolsa_Contexto__MAX_QUANTIDADE */
/* TO DO: #define Bolsa_Contexto__MAX_PRECO */
/* TO DO: #define Bolsa_Contexto__ERRO */
/* TO DO: #define Bolsa_Contexto__OK */
/* TO DO: #define Bolsa_Contexto__ERRO_CLIENTE_JA_CADASTRADO */
/* TO DO: #define Bolsa_Contexto__ERRO_ATIVO_JA_CADASTRADO */
/* TO DO: #define Bolsa_Contexto__ERRO_DEPOSITO_INVALIDO */
/* TO DO: #define Bolsa_Contexto__ERRO_SALDO_INSUFICIENTE */
/* TO DO: #define Bolsa_Contexto__ERRO_CUSTODIA_INVALIDA */
/* TO DO: #define Bolsa_Contexto__ERRO_ABERTURA_COMPRA_INVALIDA */
/* TO DO: #define Bolsa_Contexto__ERRO_ABERTURA_VENDA_INVALIDA */
/* TO DO: #define Bolsa_Contexto__ERRO_CASAMENTO_INVALIDO */
/* TO DO: #define Bolsa_Contexto__ERRO_LIMITE_EXCEDIDO */
/* TO DO: #define Bolsa_Contexto__ERRO_ORDEM_NAO_ABERTA */
/* Array and record constants */



















/* Clause CONCRETE_VARIABLES */

extern void Bolsa_Contexto__INITIALISATION(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _Bolsa_Contexto_h */

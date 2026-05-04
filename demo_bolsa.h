#ifndef DEMO_BOLSA_H
#define DEMO_BOLSA_H

#include <stdint.h>

int demo_bolsa_popular(void);
void demo_bolsa_roteiro(void);
void demo_bolsa_estado_resumido(void);

int32_t demo_bolsa_executar_total(void);
int32_t demo_bolsa_executar_parcial(void);
int32_t demo_bolsa_tentar_casamento_invalido(void);
int32_t demo_bolsa_cancelar_ordem_padrao(void);
int32_t demo_bolsa_executar_ativo2(void);

#endif

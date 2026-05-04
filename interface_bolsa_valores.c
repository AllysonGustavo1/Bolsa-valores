#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bolsa_api.h"
#include "console_ui.h"
#include "demo_bolsa.h"

#define LINHA_TAM 128

static void remover_quebra_linha(char *texto) {
    if (texto != NULL) {
        texto[strcspn(texto, "\r\n")] = '\0';
    }
}

static void normalizar_minusculas(char *texto) {
    if (texto == NULL) {
        return;
    }

    for (; *texto != '\0'; ++texto) {
        *texto = (char)tolower((unsigned char)*texto);
    }
}

static int ler_linha(const char *prompt, char *buffer, size_t tamanho) {
    if (buffer == NULL || tamanho == 0) {
        return 0;
    }

    printf("%s%s%s", ui_c(UI_CYAN), prompt, ui_c(UI_RESET));

    if (fgets(buffer, tamanho, stdin) == NULL) {
        return 0;
    }

    remover_quebra_linha(buffer);
    return 1;
}

static int parse_inteiro_exato(const char *texto, int32_t minimo, int32_t maximo, int32_t *saida) {
    char *fim = NULL;
    long valor;

    if (texto == NULL || saida == NULL) {
        return 0;
    }

    while (*texto == ' ' || *texto == '\t') {
        texto++;
    }

    if (*texto == '\0') {
        return 0;
    }

    errno = 0;
    valor = strtol(texto, &fim, 10);

    if (texto == fim || errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
        return 0;
    }

    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }

    if (*fim != '\0') {
        return 0;
    }

    if (valor < minimo || valor > maximo) {
        return 0;
    }

    *saida = (int32_t)valor;
    return 1;
}

static int ler_inteiro(const char *rotulo, int32_t minimo, int32_t maximo, int32_t *saida) {
    char linha[LINHA_TAM];
    char prompt[160];

    for (;;) {
        snprintf(prompt, sizeof(prompt), "%s [%d..%d]: ", rotulo, (int)minimo, (int)maximo);

        if (!ler_linha(prompt, linha, sizeof(linha))) {
            return 0;
        }

        if (parse_inteiro_exato(linha, minimo, maximo, saida)) {
            return 1;
        }

        ui_erro("Entrada invalida. Digite um numero inteiro dentro do intervalo.");
    }
}

static void mostrar_resultado(int32_t resultado) {
    ui_resultado_operacao(resultado);
}

static void debug_saldo(int32_t cliente_id) {
    int32_t disponivel = 0;
    int32_t bloqueado = 0;

    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Cliente | Disponivel  | Bloqueado  |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    bolsa_consultar_saldo(cliente_id, &disponivel, &bloqueado);
    printf("| %7d | %11d | %10d |\n", (int)cliente_id, (int)disponivel, (int)bloqueado);

    printf("%s+---------+-------------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static void debug_custodia(int32_t cliente_id, int32_t ativo_id) {
    int32_t disponivel = 0;
    int32_t bloqueado = 0;

    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Cliente | Ativo | Disponivel | Bloqueada |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    bolsa_consultar_custodia(cliente_id, ativo_id, &disponivel, &bloqueado);
    printf("| %7d | %5d | %10d | %9d |\n",
           (int)cliente_id,
           (int)ativo_id,
           (int)disponivel,
           (int)bloqueado);

    printf("%s+---------+-------+------------+-----------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static void debug_status_ordem(int32_t ordem_id) {
    int32_t status = 0;

    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Ordem | Status     |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));

    bolsa_consultar_status_ordem(ordem_id, &status);
    printf("| %5d | %s%-10s%s |\n",
           (int)ordem_id,
           ui_cor_status(status),
           ui_texto_status(status),
           ui_c(UI_RESET));

    printf("%s+-------+------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static int perguntar_sim_nao(const char *pergunta, int padrao_sim) {
    char linha[LINHA_TAM];

    for (;;) {
        printf("%s%s [%s/%s]: %s",
               ui_c(UI_CYAN),
               pergunta,
               padrao_sim ? "S" : "s",
               padrao_sim ? "n" : "N",
               ui_c(UI_RESET));

        if (!ler_linha("", linha, sizeof(linha))) {
            return padrao_sim;
        }

        normalizar_minusculas(linha);

        if (linha[0] == '\0') {
            return padrao_sim;
        }

        if (strcmp(linha, "s") == 0 || strcmp(linha, "sim") == 0) {
            return 1;
        }

        if (strcmp(linha, "n") == 0 || strcmp(linha, "nao") == 0) {
            return 0;
        }

        ui_alerta("Responda com s ou n.");
    }
}

static void menu(void) {
    ui_titulo("Comandos disponiveis");

    printf("%s+------------+----------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("%s| Grupo      | Comandos                         |%s\n", ui_c(UI_BOLD), ui_c(UI_RESET));
    printf("%s+------------+----------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
    printf("| %-10s | %-32s |\n", "Basico", "1 cliente | 2 ativo");
    printf("| %-10s | %-32s |\n", "Financeiro", "3 depositar | 4 retirar");
    printf("| %-10s | %-32s |\n", "Custodia", "5 creditar custodia");
    printf("| %-10s | %-32s |\n", "Ordens", "6 compra | 7 venda | 8 casar");
    printf("| %-10s | %-32s |\n", "Ordens", "9 cancelar");
    printf("| %-10s | %-32s |\n", "Consultas", "10 saldo | 11 custodia | 12 status");
    printf("| %-10s | %-32s |\n", "Demo", "estado | roteiro | total");
    printf("| %-10s | %-32s |\n", "Demo", "parcial | invalida | cancelar");
    printf("| %-10s | %-32s |\n", "Demo", "ativo2 | reset");
    printf("| %-10s | %-32s |\n", "Sistema", "menu | sair");
    printf("%s+------------+----------------------------------+%s\n", ui_c(UI_DIM), ui_c(UI_RESET));
}

static int recarregar_demo(void) {
    bolsa_inicializar();

    if (!demo_bolsa_popular()) {
        ui_erro("Falha ao carregar dados de demonstracao.");
        return 0;
    }

    return 1;
}

static int comando_para_codigo(char *comando, int32_t *codigo) {
    if (comando == NULL || codigo == NULL) {
        return 0;
    }

    normalizar_minusculas(comando);

    if (strcmp(comando, "q") == 0 || strcmp(comando, "sair") == 0 || strcmp(comando, "exit") == 0) {
        *codigo = 0;
        return 1;
    }

    if (strcmp(comando, "m") == 0 || strcmp(comando, "menu") == 0 || strcmp(comando, "?") == 0 || strcmp(comando, "ajuda") == 0) {
        *codigo = 99;
        return 1;
    }

    if (strcmp(comando, "r") == 0 || strcmp(comando, "roteiro") == 0) {
        *codigo = 13;
        return 1;
    }

    if (strcmp(comando, "total") == 0) {
        *codigo = 14;
        return 1;
    }

    if (strcmp(comando, "parcial") == 0) {
        *codigo = 15;
        return 1;
    }

    if (strcmp(comando, "invalida") == 0) {
        *codigo = 16;
        return 1;
    }

    if (strcmp(comando, "cancelar") == 0) {
        *codigo = 17;
        return 1;
    }

    if (strcmp(comando, "estado") == 0 || strcmp(comando, "e") == 0) {
        *codigo = 18;
        return 1;
    }

    if (strcmp(comando, "reset") == 0 || strcmp(comando, "reiniciar") == 0) {
        *codigo = 19;
        return 1;
    }

    if (strcmp(comando, "ativo2") == 0) {
        *codigo = 20;
        return 1;
    }

    return parse_inteiro_exato(comando, 0, 99, codigo);
}

static void executar_acao(int32_t opcao) {
    int32_t resultado = BOLSA_ERRO;

    switch (opcao) {
        case 1: {
            int32_t cliente = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            resultado = bolsa_cadastrar_cliente(cliente);
            mostrar_resultado(resultado);
            debug_saldo(cliente);
            break;
        }

        case 2: {
            int32_t ativo = 0;
            if (!ler_inteiro("Ativo", BOLSA_MIN_ATIVO, BOLSA_MAX_ATIVO, &ativo)) return;
            resultado = bolsa_cadastrar_ativo(ativo);
            mostrar_resultado(resultado);
            printf("%sAtivo:%s %d\n", ui_c(UI_BOLD), ui_c(UI_RESET), (int)ativo);
            break;
        }

        case 3: {
            int32_t cliente = 0, valor = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Valor do deposito", BOLSA_MIN_VALOR, BOLSA_MAX_DINHEIRO, &valor)) return;
            ui_secao("Antes");
            debug_saldo(cliente);
            resultado = bolsa_depositar_saldo(cliente, valor);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_saldo(cliente);
            break;
        }

        case 4: {
            int32_t cliente = 0, valor = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Valor da retirada", BOLSA_MIN_VALOR, BOLSA_MAX_DINHEIRO, &valor)) return;
            ui_secao("Antes");
            debug_saldo(cliente);
            resultado = bolsa_retirar_saldo(cliente, valor);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_saldo(cliente);
            break;
        }

        case 5: {
            int32_t cliente = 0, ativo = 0, quantidade = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Ativo", BOLSA_MIN_ATIVO, BOLSA_MAX_ATIVO, &ativo)) return;
            if (!ler_inteiro("Quantidade", BOLSA_MIN_VALOR, BOLSA_MAX_QUANTIDADE, &quantidade)) return;
            ui_secao("Antes");
            debug_custodia(cliente, ativo);
            resultado = bolsa_creditar_custodia(cliente, ativo, quantidade);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_custodia(cliente, ativo);
            break;
        }

        case 6: {
            int32_t ordem = 0, cliente = 0, ativo = 0, quantidade = 0, preco = 0;
            if (!ler_inteiro("Ordem", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem)) return;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Ativo", BOLSA_MIN_ATIVO, BOLSA_MAX_ATIVO, &ativo)) return;
            if (!ler_inteiro("Quantidade", BOLSA_MIN_VALOR, BOLSA_MAX_QUANTIDADE, &quantidade)) return;
            if (!ler_inteiro("Preco limite", BOLSA_MIN_VALOR, BOLSA_MAX_PRECO, &preco)) return;
            ui_secao("Antes");
            debug_saldo(cliente);
            debug_status_ordem(ordem);
            resultado = bolsa_abrir_ordem_compra(ordem, cliente, ativo, quantidade, preco);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_saldo(cliente);
            debug_status_ordem(ordem);
            break;
        }

        case 7: {
            int32_t ordem = 0, cliente = 0, ativo = 0, quantidade = 0, preco = 0;
            if (!ler_inteiro("Ordem", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem)) return;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Ativo", BOLSA_MIN_ATIVO, BOLSA_MAX_ATIVO, &ativo)) return;
            if (!ler_inteiro("Quantidade", BOLSA_MIN_VALOR, BOLSA_MAX_QUANTIDADE, &quantidade)) return;
            if (!ler_inteiro("Preco limite", BOLSA_MIN_VALOR, BOLSA_MAX_PRECO, &preco)) return;
            ui_secao("Antes");
            debug_custodia(cliente, ativo);
            debug_status_ordem(ordem);
            resultado = bolsa_abrir_ordem_venda(ordem, cliente, ativo, quantidade, preco);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_custodia(cliente, ativo);
            debug_status_ordem(ordem);
            break;
        }

        case 8: {
            int32_t ordem_compra = 0, ordem_venda = 0, quantidade = 0, preco = 0;
            if (!ler_inteiro("Ordem de compra", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem_compra)) return;
            if (!ler_inteiro("Ordem de venda", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem_venda)) return;
            if (!ler_inteiro("Quantidade", BOLSA_MIN_VALOR, BOLSA_MAX_QUANTIDADE, &quantidade)) return;
            if (!ler_inteiro("Preco de execucao", BOLSA_MIN_VALOR, BOLSA_MAX_PRECO, &preco)) return;
            ui_secao("Antes");
            debug_status_ordem(ordem_compra);
            debug_status_ordem(ordem_venda);
            resultado = bolsa_executar_casamento(ordem_compra, ordem_venda, quantidade, preco);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_status_ordem(ordem_compra);
            debug_status_ordem(ordem_venda);
            break;
        }

        case 9: {
            int32_t ordem = 0;
            if (!ler_inteiro("Ordem", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem)) return;
            ui_secao("Antes");
            debug_status_ordem(ordem);
            resultado = bolsa_cancelar_ordem(ordem);
            ui_secao("Resultado");
            mostrar_resultado(resultado);
            ui_secao("Depois");
            debug_status_ordem(ordem);
            break;
        }

        case 10: {
            int32_t cliente = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            debug_saldo(cliente);
            break;
        }

        case 11: {
            int32_t cliente = 0, ativo = 0;
            if (!ler_inteiro("Cliente", BOLSA_MIN_CLIENTE, BOLSA_MAX_CLIENTE, &cliente)) return;
            if (!ler_inteiro("Ativo", BOLSA_MIN_ATIVO, BOLSA_MAX_ATIVO, &ativo)) return;
            debug_custodia(cliente, ativo);
            break;
        }

        case 12: {
            int32_t ordem = 0;
            if (!ler_inteiro("Ordem", BOLSA_MIN_ORDEM, BOLSA_MAX_ORDEM, &ordem)) return;
            debug_status_ordem(ordem);
            break;
        }

        case 13:
            demo_bolsa_roteiro();
            break;

        case 14:
            resultado = demo_bolsa_executar_total();
            mostrar_resultado(resultado);
            break;

        case 15:
            resultado = demo_bolsa_executar_parcial();
            mostrar_resultado(resultado);
            break;

        case 16:
            resultado = demo_bolsa_tentar_casamento_invalido();
            mostrar_resultado(resultado);
            break;

        case 17:
            resultado = demo_bolsa_cancelar_ordem_padrao();
            mostrar_resultado(resultado);
            break;

        case 18:
            demo_bolsa_estado_resumido();
            break;

        case 19:
            if (perguntar_sim_nao("Isso reinicia todo o estado. Continuar?", 0)) {
                if (recarregar_demo()) {
                    ui_sucesso("Maquina reiniciada e demonstracao recarregada.");
                }
            } else {
                ui_alerta("Reinicio cancelado.");
            }
            break;

        case 20:
            resultado = demo_bolsa_executar_ativo2();
            mostrar_resultado(resultado);
            break;

        case 99:
            menu();
            break;

        default:
            ui_alerta("Comando numerico sem acao associada. Digite 'menu' para ver as opcoes.");
            break;
    }
}

int main(void) {
    char comando[LINHA_TAM];
    int32_t opcao = -1;

    bolsa_inicializar();

    ui_titulo("Sistema de Bolsa");

    if (perguntar_sim_nao("Carregar demonstracao rica agora?", 1)) {
        if (!demo_bolsa_popular()) {
            ui_erro("Falha ao carregar dados de demonstracao.");
            return EXIT_FAILURE;
        }
    }

    menu();
    puts("Digite 'menu' quando quiser ver as opcoes novamente.");

    while (1) {
        if (!ler_linha("bolsa> ", comando, sizeof(comando))) {
            puts("\nEntrada encerrada.");
            break;
        }

        normalizar_minusculas(comando);

        if (comando[0] == '\0') {
            continue;
        }

        if (!comando_para_codigo(comando, &opcao)) {
            ui_alerta("Comando invalido. Digite 'menu' ou '?' para ver as opcoes.");
            continue;
        }

        if (opcao == 0) {
            puts("Encerrando.");
            break;
        }

        executar_acao(opcao);
    }

    return EXIT_SUCCESS;
}

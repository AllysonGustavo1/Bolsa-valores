#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UI_RESET  "\033[0m"
#define UI_BOLD   "\033[1m"
#define UI_DIM    "\033[2m"
#define UI_RED    "\033[31m"
#define UI_GREEN  "\033[32m"
#define UI_YELLOW "\033[33m"
#define UI_BLUE   "\033[34m"
#define UI_MAGENTA "\033[35m"
#define UI_CYAN   "\033[36m"

static inline int ui_color_enabled(void) {
    const char *no_color = getenv("NO_COLOR");
    const char *term = getenv("TERM");

    if (no_color != NULL && no_color[0] != '\0') {
        return 0;
    }

    if (term != NULL && strcmp(term, "dumb") == 0) {
        return 0;
    }

    return 1;
}

static inline const char *ui_c(const char *code) {
    return ui_color_enabled() ? code : "";
}

static inline void ui_linha(char ch, int largura) {
    for (int i = 0; i < largura; ++i) {
        putchar(ch);
    }
    putchar('\n');
}

static inline void ui_titulo(const char *texto) {
    printf("\n%s%s", ui_c(UI_BOLD), ui_c(UI_CYAN));
    ui_linha('=', 72);
    printf(" %s\n", texto);
    ui_linha('=', 72);
    printf("%s", ui_c(UI_RESET));
}

static inline void ui_secao(const char *texto) {
    printf("\n%s%s%s%s\n", ui_c(UI_BOLD), ui_c(UI_BLUE), texto, ui_c(UI_RESET));
    printf("%s", ui_c(UI_DIM));
    ui_linha('-', 72);
    printf("%s", ui_c(UI_RESET));
}

static inline void ui_sucesso(const char *texto) {
    printf("%s[OK]%s %s\n", ui_c(UI_GREEN), ui_c(UI_RESET), texto);
}

static inline void ui_erro(const char *texto) {
    printf("%s[ERRO]%s %s\n", ui_c(UI_RED), ui_c(UI_RESET), texto);
}

static inline void ui_alerta(const char *texto) {
    printf("%s[AVISO]%s %s\n", ui_c(UI_YELLOW), ui_c(UI_RESET), texto);
}

static inline const char *ui_cor_resultado(int32_t resultado) {
    return resultado == 1 ? ui_c(UI_GREEN) : ui_c(UI_RED);
}

static inline const char *ui_texto_resultado(int32_t resultado) {
    return resultado == 1 ? "OK" : "REJEITADO";
}

static inline const char *ui_texto_status(int32_t codigo) {
    switch (codigo) {
        case 0: return "livre";
        case 1: return "aberta";
        case 2: return "cancelada";
        case 3: return "executada";
        default: return "desconhecido";
    }
}

static inline const char *ui_cor_status(int32_t codigo) {
    switch (codigo) {
        case 0: return ui_c(UI_DIM);
        case 1: return ui_c(UI_YELLOW);
        case 2: return ui_c(UI_RED);
        case 3: return ui_c(UI_GREEN);
        default: return ui_c(UI_MAGENTA);
    }
}

static inline void ui_resultado_operacao(int32_t resultado) {
    printf("%sResultado:%s %s%s%s (%d)\n",
           ui_c(UI_BOLD),
           ui_c(UI_RESET),
           ui_cor_resultado(resultado),
           ui_texto_resultado(resultado),
           ui_c(UI_RESET),
           (int)resultado);
}

#endif

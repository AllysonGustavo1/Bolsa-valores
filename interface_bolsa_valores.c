#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "Bolsa_Valores_Imp.h"

#ifndef BOLSA_INIT
#define BOLSA_INIT() Bolsa_Valores_Imp__INITIALISATION()
#endif

#ifndef OP_CADASTRAR_CLIENTE
#define OP_CADASTRAR_CLIENTE(res, cliente) \
    Bolsa_Valores_Imp__cadastrarCliente(&(res), (cliente))
#endif

#ifndef OP_CADASTRAR_ATIVO
#define OP_CADASTRAR_ATIVO(res, ativo) \
    Bolsa_Valores_Imp__cadastrarAtivo(&(res), (ativo))
#endif

#ifndef OP_DEPOSITAR_SALDO
#define OP_DEPOSITAR_SALDO(res, cliente, valor) \
    Bolsa_Valores_Imp__depositarSaldo(&(res), (cliente), (valor))
#endif

#ifndef OP_RETIRAR_SALDO
#define OP_RETIRAR_SALDO(res, cliente, valor) \
    Bolsa_Valores_Imp__retirarSaldo(&(res), (cliente), (valor))
#endif

#ifndef OP_CREDITAR_CUSTODIA
#define OP_CREDITAR_CUSTODIA(res, cliente, ativo, quantidade) \
    Bolsa_Valores_Imp__creditarCustodia(&(res), (cliente), (ativo), (quantidade))
#endif

#ifndef OP_ABRIR_ORDEM_COMPRA
#define OP_ABRIR_ORDEM_COMPRA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemCompra(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

#ifndef OP_ABRIR_ORDEM_VENDA
#define OP_ABRIR_ORDEM_VENDA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemVenda(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

#ifndef OP_EXECUTAR_CASAMENTO
#define OP_EXECUTAR_CASAMENTO(res, compra, venda, quantidade, preco) \
    Bolsa_Valores_Imp__executarCasamento(&(res), (compra), (venda), (quantidade), (preco))
#endif

#ifndef OP_CANCELAR_ORDEM
#define OP_CANCELAR_ORDEM(res, ordem) \
    Bolsa_Valores_Imp__cancelarOrdem(&(res), (ordem))
#endif

#ifndef OP_CONSULTAR_SALDO
#define OP_CONSULTAR_SALDO(disponivel, bloqueado, cliente) \
    Bolsa_Valores_Imp__consultarSaldo(&(disponivel), &(bloqueado), (cliente))
#endif

#ifndef OP_CONSULTAR_CUSTODIA
#define OP_CONSULTAR_CUSTODIA(disponivel, bloqueado, cliente, ativo) \
    Bolsa_Valores_Imp__consultarCustodia(&(disponivel), &(bloqueado), (cliente), (ativo))
#endif

#ifndef OP_CONSULTAR_STATUS_ORDEM
#define OP_CONSULTAR_STATUS_ORDEM(status, ordem) \
    Bolsa_Valores_Imp__consultarStatusOrdem(&(status), (ordem))
#endif

enum {
    UI_MIN_CLIENTE = 1,
    UI_MAX_CLIENTE = 20,
    UI_MIN_ATIVO = 1,
    UI_MAX_ATIVO = 10,
    UI_MIN_ORDEM = 1,
    UI_MAX_ORDEM = 50,
    UI_MIN_VALOR = 1,
    UI_MAX_DINHEIRO = 1000000,
    UI_MAX_QUANTIDADE = 1000,
    UI_MAX_PRECO = 1000,
    B_ERRO = 0,
    B_OK = 1
};

static void limpar_linha(void) {
    int ch;

    do {
        ch = getchar();
    } while (ch != '\n' && ch != EOF);
}

static int ler_inteiro(const char *rotulo, int minimo, int maximo, int *saida) {
    char buffer[128];
    long valor;
    char *fim = NULL;

    if (saida == NULL) {
        return 0;
    }

    for (;;) {
        printf("%s [%d..%d]: ", rotulo, minimo, maximo);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }

        errno = 0;
        valor = strtol(buffer, &fim, 10);

        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            printf("Entrada fora do intervalo de inteiro.\n");
            continue;
        }

        while (*fim == ' ' || *fim == '\t') {
            fim++;
        }

        if (*fim != '\n' && *fim != '\0') {
            printf("Digite apenas numeros inteiros.\n");
            continue;
        }

        if (valor < minimo || valor > maximo) {
            printf("Valor invalido. Use um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        *saida = (int)valor;
        return 1;
    }
}

static void mostrar_resultado(int resultado) {
    if (resultado == B_OK) {
        printf("Operacao realizada com sucesso.\n");
    } else if (resultado == B_ERRO) {
        printf("Operacao rejeitada pela maquina B.\n");
    } else {
        printf("Resultado inesperado retornado pela maquina: %d\n", resultado);
    }
}

static const char *descricao_status(int codigo) {
    switch (codigo) {
        case 0: return "livre";
        case 1: return "aberta";
        case 2: return "cancelada";
        case 3: return "executada";
        default: return "desconhecido";
    }
}

static void menu(void) {
    puts("");
    puts("==== Sistema de Bolsa - Interface C ====");
    puts("1  - Cadastrar cliente");
    puts("2  - Cadastrar ativo");
    puts("3  - Depositar saldo");
    puts("4  - Retirar saldo");
    puts("5  - Creditar custodia");
    puts("6  - Abrir ordem de compra");
    puts("7  - Abrir ordem de venda");
    puts("8  - Executar casamento");
    puts("9  - Cancelar ordem");
    puts("10 - Consultar saldo");
    puts("11 - Consultar custodia");
    puts("12 - Consultar status da ordem");
    puts("13 - Mostrar roteiro de demonstracao");
    puts("0  - Sair");
}

static int ler_cliente(void) {
    int valor = 0;
    (void)ler_inteiro("Cliente", UI_MIN_CLIENTE, UI_MAX_CLIENTE, &valor);
    return valor;
}

static int ler_ativo(void) {
    int valor = 0;
    (void)ler_inteiro("Ativo", UI_MIN_ATIVO, UI_MAX_ATIVO, &valor);
    return valor;
}

static int ler_ordem(const char *rotulo) {
    int valor = 0;
    (void)ler_inteiro(rotulo, UI_MIN_ORDEM, UI_MAX_ORDEM, &valor);
    return valor;
}

static int ler_dinheiro(const char *rotulo) {
    int valor = 0;
    (void)ler_inteiro(rotulo, UI_MIN_VALOR, UI_MAX_DINHEIRO, &valor);
    return valor;
}

static int ler_quantidade(void) {
    int valor = 0;
    (void)ler_inteiro("Quantidade", UI_MIN_VALOR, UI_MAX_QUANTIDADE, &valor);
    return valor;
}

static int ler_preco(void) {
    int valor = 0;
    (void)ler_inteiro("Preco", UI_MIN_VALOR, UI_MAX_PRECO, &valor);
    return valor;
}

/*
 * Interface textual para o sistema de bolsa gerado pelo Atelier-B.
 *
 * Esta interface:
 * - inclui somente o header gerado pelo Atelier-B;
 * - nao acessa variaveis internas da maquina;
 * - nao inclui arquivos .c gerados;
 * - chama apenas operacoes publicadas no .h;
 * - trata entrada invalida no nivel de UI;
 * - deixa as regras de negocio dentro das operacoes B.
 *
 * Ajuste o nome do header abaixo caso o gerador tenha produzido outro nome.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "Bolsa_Valores_Imp.h"
#include "demo_bolsa.h"

#ifndef BOLSA_INIT
#define BOLSA_INIT() Bolsa_Valores_Imp__INITIALISATION()
#endif

#ifndef OP_CADASTRAR_CLIENTE
#define OP_CADASTRAR_CLIENTE(res, cliente) \
    Bolsa_Valores_Imp__cadastrarCliente(&(res), (cliente))
#endif

#ifndef OP_CADASTRAR_ATIVO
#define OP_CADASTRAR_ATIVO(res, ativo) \
    Bolsa_Valores_Imp__cadastrarAtivo(&(res), (ativo))
#endif

#ifndef OP_DEPOSITAR_SALDO
#define OP_DEPOSITAR_SALDO(res, cliente, valor) \
    Bolsa_Valores_Imp__depositarSaldo(&(res), (cliente), (valor))
#endif

#ifndef OP_RETIRAR_SALDO
#define OP_RETIRAR_SALDO(res, cliente, valor) \
    Bolsa_Valores_Imp__retirarSaldo(&(res), (cliente), (valor))
#endif

#ifndef OP_CREDITAR_CUSTODIA
#define OP_CREDITAR_CUSTODIA(res, cliente, ativo, quantidade) \
    Bolsa_Valores_Imp__creditarCustodia(&(res), (cliente), (ativo), (quantidade))
#endif

#ifndef OP_ABRIR_ORDEM_COMPRA
#define OP_ABRIR_ORDEM_COMPRA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemCompra(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

#ifndef OP_ABRIR_ORDEM_VENDA
#define OP_ABRIR_ORDEM_VENDA(res, ordem, cliente, ativo, quantidade, preco) \
    Bolsa_Valores_Imp__abrirOrdemVenda(&(res), (ordem), (cliente), (ativo), (quantidade), (preco))
#endif

#ifndef OP_EXECUTAR_CASAMENTO
#define OP_EXECUTAR_CASAMENTO(res, compra, venda, quantidade, preco) \
    Bolsa_Valores_Imp__executarCasamento(&(res), (compra), (venda), (quantidade), (preco))
#endif

#ifndef OP_CANCELAR_ORDEM
#define OP_CANCELAR_ORDEM(res, ordem) \
    Bolsa_Valores_Imp__cancelarOrdem(&(res), (ordem))
#endif

#ifndef OP_CONSULTAR_SALDO
#define OP_CONSULTAR_SALDO(disponivel, bloqueado, cliente) \
    Bolsa_Valores_Imp__consultarSaldo(&(disponivel), &(bloqueado), (cliente))
#endif

#ifndef OP_CONSULTAR_CUSTODIA
#define OP_CONSULTAR_CUSTODIA(disponivel, bloqueado, cliente, ativo) \
    Bolsa_Valores_Imp__consultarCustodia(&(disponivel), &(bloqueado), (cliente), (ativo))
#endif

#ifndef OP_CONSULTAR_STATUS_ORDEM
#define OP_CONSULTAR_STATUS_ORDEM(status, ordem) \
    Bolsa_Valores_Imp__consultarStatusOrdem(&(status), (ordem))
#endif

enum {
    UI_MIN_CLIENTE = 1,
    UI_MAX_CLIENTE = 20,
    UI_MIN_ATIVO = 1,
    UI_MAX_ATIVO = 10,
    UI_MIN_ORDEM = 1,
    UI_MAX_ORDEM = 50,
    UI_MIN_VALOR = 1,
    UI_MAX_DINHEIRO = 1000000,
    UI_MAX_QUANTIDADE = 1000,
    UI_MAX_PRECO = 1000,
    B_ERRO = 0,
    B_OK = 1
};

static void limpar_linha(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* descarta */
    }
}

static int ler_inteiro(const char *rotulo, int minimo, int maximo, int *saida) {
    char buffer[128];
    long valor;
    char *fim = NULL;

    if (saida == NULL) {
        return 0;
    }

    for (;;) {
        printf("%s [%d..%d]: ", rotulo, minimo, maximo);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }

        errno = 0;
        valor = strtol(buffer, &fim, 10);

        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            printf("Entrada fora do intervalo de inteiro.\n");
            continue;
        }

        while (*fim == ' ' || *fim == '\t') {
            fim++;
        }

        if (*fim != '\n' && *fim != '\0') {
            printf("Digite apenas numeros inteiros.\n");
            continue;
        }

        if (valor < minimo || valor > maximo) {
            printf("Valor invalido. Use um numero entre %d e %d.\n", minimo, maximo);
            continue;
        }

        *saida = (int)valor;
        return 1;
    }
}

static void mostrar_resultado(int resultado) {
    if (resultado == B_OK) {
        printf("Operacao realizada com sucesso.\n");
    } else if (resultado == B_ERRO) {
        printf("Operacao rejeitada pela maquina B.\n");
    } else {
        printf("Resultado inesperado retornado pela maquina: %d\n", resultado);
    }
}

static const char *descricao_status(int codigo) {
    switch (codigo) {
        case 0: return "livre";
        case 1: return "aberta";
        case 2: return "cancelada";
        case 3: return "executada";
        default: return "desconhecido";
    }
}

static void menu(void) {
    puts("");
    puts("==== Sistema de Bolsa - Interface C ====");
    puts("1  - Cadastrar cliente");
    puts("2  - Cadastrar ativo");
    puts("3  - Depositar saldo");
    puts("4  - Retirar saldo");
    puts("5  - Creditar custodia");
    puts("6  - Abrir ordem de compra");
    puts("7  - Abrir ordem de venda");
    puts("8  - Executar casamento");
    puts("9  - Cancelar ordem");
    puts("10 - Consultar saldo");
    puts("11 - Consultar custodia");
    puts("12 - Consultar status da ordem");
    puts("0  - Sair");
}

static int ler_cliente(void) {
    int valor = 0;
    (void)ler_inteiro("Cliente", UI_MIN_CLIENTE, UI_MAX_CLIENTE, &valor);
    return valor;
}

static int ler_ativo(void) {
    int valor = 0;
    (void)ler_inteiro("Ativo", UI_MIN_ATIVO, UI_MAX_ATIVO, &valor);
    return valor;
}

static int ler_ordem(const char *rotulo) {
    int valor = 0;
    (void)ler_inteiro(rotulo, UI_MIN_ORDEM, UI_MAX_ORDEM, &valor);
    return valor;
}

static int ler_dinheiro(const char *rotulo) {
    int valor = 0;
    (void)ler_inteiro(rotulo, UI_MIN_VALOR, UI_MAX_DINHEIRO, &valor);
    return valor;
}

static int ler_quantidade(void) {
    int valor = 0;
    (void)ler_inteiro("Quantidade", UI_MIN_VALOR, UI_MAX_QUANTIDADE, &valor);
    return valor;
}

static int ler_preco(void) {
    int valor = 0;
    (void)ler_inteiro("Preco", UI_MIN_VALOR, UI_MAX_PRECO, &valor);
    return valor;
}

int main(void) {
    int opcao = -1;
    int resultado = B_ERRO;

    BOLSA_INIT();

    puts("Sistema iniciado.");

    if (!demo_bolsa_popular()) {
        puts("Falha ao carregar dados de demonstracao.");
        return EXIT_FAILURE;
    }

    demo_bolsa_roteiro();

    while (opcao != 0) {
        menu();

        if (!ler_inteiro("Opcao", 0, 12, &opcao)) {
            puts("Falha de leitura. Encerrando.");
            return EXIT_FAILURE;
        }

        switch (opcao) {
            case 0:
                puts("Encerrando.");
                break;

            case 1: {
                int cliente = ler_cliente();
                OP_CADASTRAR_CLIENTE(resultado, cliente);
                mostrar_resultado(resultado);
                break;
            }

            case 2: {
                int ativo = ler_ativo();
                OP_CADASTRAR_ATIVO(resultado, ativo);
                mostrar_resultado(resultado);
                break;
            }

            case 3: {
                int cliente = ler_cliente();
                int valor = ler_dinheiro("Valor do deposito");
                OP_DEPOSITAR_SALDO(resultado, cliente, valor);
                mostrar_resultado(resultado);
                break;
            }

            case 4: {
                int cliente = ler_cliente();
                int valor = ler_dinheiro("Valor da retirada");
                OP_RETIRAR_SALDO(resultado, cliente, valor);
                mostrar_resultado(resultado);
                break;
            }

            case 5: {
                int cliente = ler_cliente();
                int ativo = ler_ativo();
                int quantidade = ler_quantidade();
                OP_CREDITAR_CUSTODIA(resultado, cliente, ativo, quantidade);
                mostrar_resultado(resultado);
                break;
            }

            case 6: {
                int ordem = ler_ordem("Ordem de compra");
                int cliente = ler_cliente();
                int ativo = ler_ativo();
                int quantidade = ler_quantidade();
                int preco = ler_preco();
                OP_ABRIR_ORDEM_COMPRA(resultado, ordem, cliente, ativo, quantidade, preco);
                mostrar_resultado(resultado);
                break;
            }

            case 7: {
                int ordem = ler_ordem("Ordem de venda");
                int cliente = ler_cliente();
                int ativo = ler_ativo();
                int quantidade = ler_quantidade();
                int preco = ler_preco();
                OP_ABRIR_ORDEM_VENDA(resultado, ordem, cliente, ativo, quantidade, preco);
                mostrar_resultado(resultado);
                break;
            }

            case 8: {
                int ordemCompra = ler_ordem("Ordem de compra");
                int ordemVenda = ler_ordem("Ordem de venda");
                int quantidade = ler_quantidade();
                int preco = ler_preco();
                OP_EXECUTAR_CASAMENTO(resultado, ordemCompra, ordemVenda, quantidade, preco);
                mostrar_resultado(resultado);
                break;
            }

            case 9: {
                int ordem = ler_ordem("Ordem");
                OP_CANCELAR_ORDEM(resultado, ordem);
                mostrar_resultado(resultado);
                break;
            }

            case 10: {
                int cliente = ler_cliente();
                int disponivel = 0;
                int bloqueado = 0;
                OP_CONSULTAR_SALDO(disponivel, bloqueado, cliente);
                printf("Saldo disponivel: %d\n", disponivel);
                printf("Saldo bloqueado : %d\n", bloqueado);
                break;
            }

            case 11: {
                int cliente = ler_cliente();
                int ativo = ler_ativo();
                int disponivel = 0;
                int bloqueado = 0;
                OP_CONSULTAR_CUSTODIA(disponivel, bloqueado, cliente, ativo);
                printf("Custodia disponivel: %d\n", disponivel);
                printf("Custodia bloqueada : %d\n", bloqueado);
                break;
            }

            case 12: {
                int ordem = ler_ordem("Ordem");
                int status = -1;
                OP_CONSULTAR_STATUS_ORDEM(status, ordem);
                printf("Status da ordem: %d (%s)\n", status, descricao_status(status));
                break;
            }

            case 13:
                demo_bolsa_roteiro();
                break;

            default:
                puts("Opcao invalida.");
                break;
        }
    }

    return EXIT_SUCCESS;
}
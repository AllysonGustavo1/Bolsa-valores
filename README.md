# 📈 Sistema Simplificado de Bolsa de Valores com Método B

Projeto de bolsa de valores simplificada desenvolvido com **Método B**, incluindo especificação abstrata, refinamento,
implementação B0, geração de código C pelo Atelier B e interface de linha de comando para demonstração.

O sistema modela clientes, ativos, saldos, custódias e ordens de compra/venda. A regra central é o bloqueio de recursos:
ordens de compra bloqueiam saldo, ordens de venda bloqueiam custódia, e uma negociação só é executada quando as ordens
são compatíveis.

---

## 📁 1. Estrutura do projeto

### Arquivos B

| Arquivo | Função |
|---|---|
| `Bolsa_Contexto.mch` | Define constantes, conjuntos enumerados e códigos de resultado |
| `Bolsa_Valores_Abstract.mch` | Especificação abstrata do sistema |
| `Bolsa_Valores_Ref.ref` | Refinamento com estruturas mais próximas da implementação |
| `Bolsa_Valores_Imp.imp` | Implementação B0 para geração de código C |

### Arquivos C manuais

| Arquivo | Função |
|---|---|
| `bolsa_api.h` | Camada de compatibilidade com o header gerado |
| `console_ui.h` | Formatação, cores e tabelas no terminal |
| `interface_bolsa_valores.c` | CLI principal |
| `demo_bolsa.c` | Carga de dados e cenários de demonstração |
| `demo_bolsa.h` | Header da demo |
| `test_scenarios.c` | Testes automatizados básicos |
| `Makefile` | Automatiza compilação, execução, testes e limpeza |

### Arquivos gerados pelo Atelier B

| Arquivo | Função |
|---|---|
| `Bolsa_Valores_Abstract.h` | Header exportado pelo código gerado |
| `Bolsa_Valores_Implementation.c` | Código C gerado a partir da implementação |
| `Bolsa_Contexto.h` | Header do contexto, quando gerado pela ferramenta |

---

## ⚙️ 2. Funcionalidades

| Operação | Descrição |
|---|---|
| `cadastrarCliente` | Cadastra um cliente |
| `cadastrarAtivo` | Cadastra/lista um ativo |
| `depositarSaldo` | Adiciona saldo disponível |
| `retirarSaldo` | Retira saldo disponível |
| `creditarCustodia` | Adiciona ativos à custódia disponível |
| `abrirOrdemCompra` | Abre ordem de compra e bloqueia saldo |
| `abrirOrdemVenda` | Abre ordem de venda e bloqueia custódia |
| `executarCasamento` | Executa negociação entre compra e venda compatíveis |
| `cancelarOrdem` | Cancela ordem aberta e devolve recurso bloqueado |
| `consultarSaldo` | Consulta saldo disponível/bloqueado |
| `consultarCustodia` | Consulta custódia disponível/bloqueada |
| `consultarStatusOrdem` | Consulta status da ordem |

---

## 🚦 3. Códigos de resultado

As operações retornam um inteiro chamado `resultado`. O código `OK = 1` indica sucesso; os demais códigos indicam
rejeição.

| Código | Nome | Significado |
|---:|---|---|
| 1 | `OK` | Operação realizada com sucesso |
| 0 | `ERRO` | Rejeição genérica |
| 2 | `ERRO_CLIENTE_JA_CADASTRADO` | Cliente já cadastrado |
| 3 | `ERRO_ATIVO_JA_CADASTRADO` | Ativo já cadastrado |
| 4 | `ERRO_DEPOSITO_INVALIDO` | Depósito inválido, cliente inexistente ou limite excedido |
| 5 | `ERRO_SALDO_INSUFICIENTE` | Saldo insuficiente ou cliente inexistente |
| 6 | `ERRO_CUSTODIA_INVALIDA` | Custódia inválida, cliente/ativo inexistente ou limite excedido |
| 7 | `ERRO_ABERTURA_COMPRA_INVALIDA` | Compra inválida, ordem ocupada, saldo insuficiente ou limite excedido |
| 8 | `ERRO_ABERTURA_VENDA_INVALIDA` | Venda inválida, ordem ocupada, custódia insuficiente ou limite excedido |
| 9 | `ERRO_CASAMENTO_INVALIDO` | Casamento inválido ou ordens incompatíveis |
| 10 | `ERRO_LIMITE_EXCEDIDO` | Limite interno excedido ou recurso bloqueado insuficiente |
| 11 | `ERRO_ORDEM_NAO_ABERTA` | Ordem não está aberta |

Os motivos de rejeição são simples e agrupados por operação. Mensagens mais específicas exigiriam a separação de cada
condição de erro em `IF`s mais detalhados na máquina B.

---

## 🧪 4. Verificações no Atelier B

Ordem recomendada dos componentes:

```text
1. Bolsa_Contexto.mch
2. Bolsa_Valores_Abstract.mch
3. Bolsa_Valores_Ref.ref
4. Bolsa_Valores_Imp.imp
```

Verificações principais:

| Etapa | Verificações |
|---|---|
| Contexto | `Type Check` |
| Máquina abstrata | `Type Check`, `PO Generate`, `Proof` |
| Refinamento | `Type Check`, `PO Generate`, `Proof` |
| Implementação | `Type Check`, `B0 Check`, `Code Generation` |

Pontos importantes:

- o contexto contém os códigos de resultado;
- após alterações nos arquivos B, o código C precisa ser gerado novamente;
- a implementação usa índices internos iniciando em zero;
- a CLI deve ser compilada usando os arquivos C gerados mais os arquivos C manuais.

---

## 🛠️ 5. Compilação

### Usando Makefile

Com `Makefile` no diretório do projeto:

```bash
make check-files
make
make run
```

Comandos úteis:

| Comando | Ação |
|---|---|
| `make` | Compila a CLI |
| `make run` | Executa a CLI interativa |
| `make demo` | Executa roteiro automático no terminal |
| `make cli-test` | Executa roteiro automático e salva `run_output_cli.txt` |
| `make test` | Compila e roda `test_scenarios.c`, quando presente |
| `make asan` | Compila com AddressSanitizer/UBSan |
| `make check-files` | Verifica arquivos esperados |
| `make clean` | Remove binários e logs |
| `make help` | Lista os comandos disponíveis |

### Compilação manual

```bash
gcc -std=c11 -Wall -Wextra -O2 \
  -include bolsa_api.h \
  interface_bolsa_valores.c demo_bolsa.c Bolsa_Valores_Implementation.c \
  -o bolsa_cli
```
---

## 💻 6. Execução da CLI

```bash
./bolsa_cli
```

Comandos principais:

| Comando | Ação |
|---|---|
| `estado` | Mostra saldos, custódias e status |
| `total` | Executa casamento total de alto valor |
| `parcial` | Executa casamento parcial |
| `invalida` | Testa rejeição por preço incompatível |
| `cancelar` | Cancela uma ordem da demonstração |
| `ativo2` | Executa negociação no segundo ativo |
| `reset` | Reinicia a máquina e recarrega a demo |
| `menu` | Mostra o menu |
| `sair` | Encerra o programa |

Desativação de cores:

```bash
NO_COLOR=1 ./bolsa_cli
```

---

## 🎬 7. Roteiro de demonstração

Roteiro sugerido:

```text
estado
total
estado
parcial
estado
invalida
cancelar
ativo2
estado
sair
```

O fluxo cobre:

| Etapa | O que demonstra |
|---|---|
| `estado` | Estado inicial com saldos, custódias e ordens |
| `total` | Execução total entre compra e venda compatíveis |
| `parcial` | Execução parcial mantendo ordens abertas |
| `invalida` | Rejeição por incompatibilidade de preço |
| `cancelar` | Cancelamento e devolução de recurso bloqueado |
| `ativo2` | Negociação envolvendo outro ativo |
| `estado` | Estado final após as operações |

---

## ✅ 8. Testes automatizados

Com Makefile:

```bash
make test
```

Compilação manual:

```bash
gcc -std=c11 -Wall -Wextra -O2 \
  -include bolsa_api.h \
  test_scenarios.c Bolsa_Valores_Implementation.c \
  -o bolsa_tests
```

Execução:

```bash
./bolsa_tests
```

Cenários cobertos:

- cadastro de cliente;
- cadastro duplicado;
- cadastro de ativo;
- depósito;
- abertura de compra;
- abertura de venda;
- tentativa de reutilizar ordem;
- casamento total;
- tentativa de cancelar ordem já executada.

---

## 🔍 9. Observações da demonstração

A demonstração usa valores altos para deixar os efeitos visíveis:

```text
Cliente 1: saldo 1.000.000
Ordem 1: compra de 900 unidades a preço limite 1000
Ordem 2: venda de 900 unidades a preço mínimo 920
Execução total: 900 unidades a preço 950
```

Na ordem 8, a rejeição esperada é:

```text
BOLSA_ERRO_ABERTURA_COMPRA_INVALIDA = 7
```

Esse retorno ocorre porque a operação agora informa um motivo de rejeição mais específico em vez de usar apenas
`ERRO = 0`.

---

## ⚠️ 10. Limitações

O sistema é uma simplificação acadêmica. Não foram implementados:

- book automático de ofertas;
- prioridade por preço e tempo;
- histórico completo de negociações;
- taxas e corretagem;
- autenticação de usuários;
- persistência em banco de dados;
- integração com mercado real.

O foco é demonstrar modelagem formal, invariantes, refinamento, implementação B0, geração de código e integração com C.

---

## 📌 11. Resumo

A versão atual inclui:

```text
Especificação abstrata
Refinamento
Implementação B0
Geração de código C
CLI formatada
Demonstração automática
Testes básicos
Makefile
Códigos de rejeição com motivo simples
```
# Sistema Simplificado de Bolsa de Valores com Método B

## Informações do trabalho

**Alunos:** Allyson Gustavo Silva do Carmo e Franklin Claudio Lopes de Oliveira Filho  
**Professor:** Marcel Vinicius Medeiros Oliveira  
**Disciplina:** DIM0516 - Métodos Formais de Engenharia de Software - T01 (2026.1)

---

## 1. 🎯 Objetivo do projeto

Este projeto propõe o desenvolvimento formal de um sistema simplificado de bolsa de valores usando o **Método B**. O
sistema permite cadastrar clientes e ativos, controlar saldo e custódia, abrir ordens de compra e venda, executar
negociações entre ordens compatíveis, cancelar ordens e consultar estados do sistema.

A escolha do tema se justifica porque sistemas financeiros são críticos: uma falha em regras de saldo, custódia ou
execução de ordens pode causar inconsistências e prejuízos. Com o Método B, essas regras são descritas formalmente por
meio de máquinas abstratas, invariantes, operações, refinamentos e implementação B0, permitindo validação no ProB, prova
no Atelier B e geração de código C.

---

## 2. 📈 Como uma bolsa de valores funciona

De forma simplificada, uma bolsa de valores é um ambiente onde compradores e vendedores negociam ativos. Cada
participante envia ordens ao sistema:

- uma **ordem de compra** informa o ativo desejado, a quantidade e o preço máximo que o comprador aceita pagar;
- uma **ordem de venda** informa o ativo oferecido, a quantidade e o preço mínimo que o vendedor aceita receber.

Uma negociação só pode ocorrer quando há compatibilidade entre compra e venda. Por exemplo:

```text
Compra: aceita pagar até 100
Venda: aceita vender por pelo menos 90
Resultado: pode haver negociação
```

Se a venda exigir 110, a negociação não deve ocorrer, pois o comprador só aceita pagar até 100.

Em bolsas reais, existe um *book de ofertas*, que organiza ordens por melhor preço e ordem de chegada. Neste projeto, a
implementação foi simplificada: o usuário informa explicitamente a ordem de compra, a ordem de venda, a quantidade e o
preço de execução. A máquina verifica se a execução é válida conforme as regras formais.

---

## 3. 🧩 Modelo implementado

O sistema trabalha com limites finitos definidos no contexto da máquina B:

```text
MAX_CLIENTES
MAX_ATIVOS
MAX_ORDENS
MAX_DINHEIRO
MAX_QUANTIDADE
MAX_PRECO
```

As principais entidades são:

| Entidade | Função no sistema |
|---|---|
| Cliente | Participante que pode comprar ou vender ativos |
| Ativo | Item negociável |
| Saldo disponível | Dinheiro livre para uso |
| Saldo bloqueado | Dinheiro reservado para ordens de compra |
| Custódia disponível | Ativos livres para venda |
| Custódia bloqueada | Ativos reservados para ordens de venda |
| Ordem | Intenção de compra ou venda |
| Status | Livre, aberta, cancelada ou executada |

A regra central é o **bloqueio de recursos**. Ao abrir uma ordem, o sistema reserva o saldo ou a custódia necessária.
Isso impede que o mesmo recurso seja usado em duas operações ao mesmo tempo.

---

## 4. 🔒 Por que o Método B é adequado

O Método B é útil neste projeto porque permite transformar regras críticas em propriedades formais. Em vez de apenas
descrever que “não pode vender sem custódia”, essa regra aparece na especificação e precisa ser preservada pelas
operações.

O uso do Método B ajuda a garantir:

- saldos e custódias nunca ficam negativos;
- uma compra só é aberta se houver saldo suficiente;
- uma venda só é aberta se houver custódia suficiente;
- ordens abertas possuem cliente, ativo, quantidade e preço válidos;
- ordens canceladas ou executadas ficam com quantidade zero;
- uma negociação só ocorre entre ordens compatíveis;
- a implementação respeita a especificação abstrata.

Essas garantias são importantes porque o sistema representa um domínio financeiro, no qual inconsistências podem afetar
diretamente valores monetários e posições de ativos.

---

## 5. ⚙️ Estrutura do desenvolvimento formal

O projeto foi organizado em três níveis.

### 5.1. Contexto

Define os conjuntos enumerados e constantes globais:

```text
TIPO_LADO = {lado_compra, lado_venda}
STATUS_ORDEM = {status_livre, status_aberta, status_cancelada, status_executada}
OK
ERRO
```

Também define os limites máximos do sistema, como quantidade de clientes, ativos, ordens e valores permitidos.

### 5.2. Máquina abstrata

Modela o sistema de forma matemática. Nesta etapa, a prioridade é descrever corretamente o comportamento esperado, sem
se preocupar com detalhes de código.

Exemplo conceitual de custódia:

```text
custodiaDisponivel(cliente |-> ativo)
```

Esse formato deixa claro que cada cliente possui uma quantidade associada a cada ativo.

### 5.3. Refinamento e implementação

No refinamento, as estruturas abstratas são aproximadas de estruturas implementáveis. Por exemplo:

- clientes cadastrados viram uma função booleana;
- ativos cadastrados viram uma função booleana;
- a custódia por par cliente/ativo é linearizada em um vetor.

A linearização segue a regra:

```text
indice = (cliente - 1) * MAX_ATIVOS + (ativo - 1)
```

Na implementação B0, os índices internos começam em zero, o que facilita a geração de código C.

---

## 6. 🧾 Funcionalidades implementadas

| Operação | Descrição |
|---|---|
| `cadastrarCliente` | Cadastra um cliente e inicializa seus saldos |
| `cadastrarAtivo` | Lista um ativo negociável |
| `depositarSaldo` | Adiciona saldo disponível a um cliente |
| `retirarSaldo` | Retira saldo disponível, se houver saldo suficiente |
| `creditarCustodia` | Adiciona ativos à custódia disponível |
| `abrirOrdemCompra` | Abre uma compra e bloqueia saldo |
| `abrirOrdemVenda` | Abre uma venda e bloqueia custódia |
| `executarCasamento` | Executa negociação entre compra e venda compatíveis |
| `cancelarOrdem` | Cancela ordem aberta e devolve recursos bloqueados |
| `consultarSaldo` | Consulta saldo disponível e bloqueado |
| `consultarCustodia` | Consulta custódia disponível e bloqueada |
| `consultarStatusOrdem` | Consulta o status de uma ordem |

As operações retornam `OK` ou `ERRO`, permitindo que a interface C mostre se a regra de negócio aceitou ou rejeitou a
ação.

---

## 7. 🔁 Fluxos principais

### Compra

```text
1. Cliente é cadastrado
2. Ativo é cadastrado
3. Cliente deposita saldo
4. Cliente abre ordem de compra
5. Sistema bloqueia o valor da ordem
6. Ordem fica aberta
```

### Venda

```text
1. Cliente é cadastrado
2. Ativo é cadastrado
3. Cliente recebe custódia
4. Cliente abre ordem de venda
5. Sistema bloqueia a quantidade de ativos
6. Ordem fica aberta
```

### Execução de negociação

```text
1. Existe uma ordem de compra aberta
2. Existe uma ordem de venda aberta
3. As ordens são do mesmo ativo
4. O preço da compra é compatível com o preço da venda
5. O comprador recebe ativos
6. O vendedor recebe saldo
7. As quantidades e os status são atualizados
```

### Cancelamento

```text
1. Ordem está aberta
2. Sistema identifica se é compra ou venda
3. Recurso bloqueado é devolvido
4. Quantidade vira zero
5. Status passa para cancelada
```

---

## 8. 🧪 Testes e validação

A validação foi feita em três níveis.

### 8.1. ProB

O ProB foi usado para animar a máquina abstrata e observar os estados após as operações. Os cenários principais incluem:

- cadastro de cliente e ativo;
- depósito de saldo;
- crédito de custódia;
- abertura de compra;
- abertura de venda;
- execução total;
- execução parcial;
- cancelamento;
- rejeição de operação inválida.

### 8.2. Atelier B

No Atelier B, foram realizadas verificações formais:

- Type Check do contexto;
- Type Check da máquina abstrata;
- geração de Proof Obligations;
- análise de Well-Definedness Proof Obligations;
- prova de preservação dos invariantes;
- Type Check do refinamento;
- Type Check da implementação;
- B0 Check;
- geração de código C.

As Proof Obligations verificam se as operações preservam os invariantes. As WD Proof Obligations verificam se expressões
como acessos a funções e arrays estão bem definidas.

### 8.3. Código C e CLI

Após a geração do código C, foi criada uma interface de linha de comando. Essa interface não acessa variáveis internas
da máquina; ela apenas chama as funções exportadas pelo header gerado.

Roteiro usado na demonstração:

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

Esse roteiro testa execução válida, execução parcial, rejeição por preço incompatível, cancelamento e negociação
envolvendo um segundo ativo.

---

## 9. 💻 Interface de demonstração

A interface em C permite interagir com o sistema usando comandos textuais:

| Comando | Ação |
|---|---|
| `estado` | Mostra saldos, custódias e status |
| `total` | Executa casamento total |
| `parcial` | Executa casamento parcial |
| `invalida` | Testa rejeição por preço incompatível |
| `cancelar` | Cancela uma ordem da demonstração |
| `ativo2` | Executa negociação com outro ativo |
| `reset` | Reinicia o estado da máquina |
| `menu` | Mostra os comandos |
| `sair` | Encerra a aplicação |

A interface também usa tabelas e cores no terminal para facilitar a leitura durante a apresentação.

---

## 10. ⚠️ Limitações

O sistema é propositalmente simplificado. Ele não implementa:

- book automático de ofertas;
- prioridade por preço e tempo;
- histórico de negociações;
- taxas, corretagem ou emolumentos;
- autenticação de usuários;
- persistência em banco de dados;
- integração com mercado real.

Essas limitações foram adotadas para manter o foco no objetivo da disciplina: especificação formal, prova, refinamento,
implementação B0 e geração de código.

---

## 11. ✅ Conclusão

O projeto mostra como o Método B pode ser aplicado em um domínio financeiro simplificado. A especificação formal permite
definir regras críticas com precisão, o refinamento aproxima o modelo da implementação, e a geração de código C
demonstra que o sistema pode ser executado fora da ferramenta formal.

A principal contribuição do trabalho é mostrar que regras de negócio sensíveis, como bloqueio de saldo, custódia,
cancelamento e execução de ordens, podem ser modeladas, verificadas e demonstradas de forma estruturada.
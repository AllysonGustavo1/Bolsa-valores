# Sistema Simplificado de Bolsa de Valores com Método B

## Informacoes do trabalho

**Alunos:**
ALLYSON GUSTAVO SILVA DO CARMO
FRANKLIN CLAUDIO LOPES DE OLIVEIRA FILHO

**Professor:**
MARCEL VINICIUS MEDEIROS OLIVEIRA

**Materia e turma:**
DIM0516 - METODOS FORMAIS DE ENGENHARIA DE SOFTWARE - T01 (2026.1)

## 1. Tema e objetivo

O projeto consiste no desenvolvimento formal de um sistema simplificado de bolsa de valores, utilizando o Método B para
especificação, refinamento, implementação e posterior geração de código. A proposta é modelar as regras centrais de uma
plataforma de negociação de ativos, garantindo que operações como cadastro, abertura de ordens, cancelamento e execução
de negócios respeitem invariantes formais de consistência.

O foco do sistema não é reproduzir toda a complexidade de uma bolsa real, mas sim criar um modelo suficientemente
completo para demonstrar o uso de máquinas abstratas, invariantes, operações, refinamentos, implementação B0 e
integração com uma interface externa em C.

## 2. Visão geral do sistema

O sistema representa uma bolsa de valores simplificada, na qual clientes podem ser cadastrados, ativos podem ser
listados e ordens de compra e venda podem ser abertas. Cada cliente possui saldo financeiro e custódia de ativos. Ao
abrir uma ordem de compra, parte do saldo do cliente é bloqueada. Ao abrir uma ordem de venda, parte da custódia do
cliente é bloqueada.

O sistema também permite executar o casamento entre uma ordem de compra e uma ordem de venda, desde que ambas estejam
abertas, sejam referentes ao mesmo ativo, tenham preços compatíveis e pertençam a clientes diferentes. Após a execução,
os saldos, custódias, quantidades restantes e status das ordens são atualizados.

## 3. Entidades principais

O sistema é baseado nas seguintes entidades:

**Clientes:** representam os usuários que podem comprar ou vender ativos. Cada cliente possui saldo disponível, saldo
bloqueado e custódia de ativos.

**Ativos:** representam os papéis ou instrumentos financeiros negociáveis na bolsa.

**Ordens:** representam intenções de compra ou venda. Cada ordem possui um dono, um ativo associado, um lado da
operação, uma quantidade, um preço limite e um status.

**Status da ordem:** uma ordem pode estar livre, aberta, cancelada ou executada.

**Tipo da ordem:** uma ordem pode ser de compra ou de venda.

## 4. Funcionalidades abstratas

A máquina abstrata modela as funcionalidades principais do sistema de forma matemática, sem se preocupar inicialmente
com detalhes de implementação. As principais funcionalidades abstratas são:

### Cadastro de clientes

Permite cadastrar novos clientes no sistema. Um cliente só pode ser cadastrado se ainda não estiver presente no conjunto
de clientes. Ao ser cadastrado, seu saldo disponível e saldo bloqueado são inicializados com zero.

### Cadastro de ativos

Permite listar ativos negociáveis. Um ativo só pode ser cadastrado se ainda não estiver listado.

### Depósito de saldo

Permite adicionar saldo disponível a um cliente já cadastrado. A operação deve preservar o limite máximo de dinheiro
definido no contexto do sistema.

### Retirada de saldo

Permite retirar saldo disponível de um cliente. A retirada só pode ocorrer se o cliente existir e possuir saldo
suficiente. O saldo bloqueado em ordens abertas não pode ser retirado.

### Crédito de custódia

Permite registrar uma quantidade de determinado ativo na custódia disponível de um cliente. Essa operação representa uma
posição inicial ou um ajuste administrativo de custódia.

### Abertura de ordem de compra

Permite abrir uma ordem de compra para determinado cliente e ativo. A ordem só pode ser aberta se o cliente existir, o
ativo estiver listado, a ordem estiver livre, a quantidade e o preço forem válidos e o cliente possuir saldo disponível
suficiente. Ao abrir a ordem, o valor necessário é transferido de saldo disponível para saldo bloqueado.

### Abertura de ordem de venda

Permite abrir uma ordem de venda para determinado cliente e ativo. A ordem só pode ser aberta se o cliente existir, o
ativo estiver listado, a ordem estiver livre e o cliente possuir custódia disponível suficiente. Ao abrir a ordem, a
quantidade vendida é transferida de custódia disponível para custódia bloqueada.

### Execução de casamento de ordens

Permite executar uma negociação entre uma ordem de compra e uma ordem de venda. Para que o casamento seja permitido, as
ordens devem estar abertas, ter lados opostos, negociar o mesmo ativo, pertencer a clientes diferentes e possuir preços
compatíveis. A quantidade executada não pode exceder a quantidade restante de nenhuma das ordens.

Após a execução, o comprador recebe a custódia do ativo, o vendedor recebe o valor financeiro da operação, o saldo
bloqueado do comprador é reduzido e a custódia bloqueada do vendedor também é reduzida. Caso a execução seja total, a
ordem passa para o status executada. Caso seja parcial, a ordem continua aberta com a quantidade restante.

### Cancelamento de ordem

Permite cancelar uma ordem aberta. Se a ordem for de compra, o saldo bloqueado restante é devolvido ao saldo disponível
do cliente. Se a ordem for de venda, a custódia bloqueada restante é devolvida à custódia disponível. Após o
cancelamento, a quantidade da ordem passa para zero e o status passa para cancelada.

### Consultas

O sistema também permite consultar saldo disponível e bloqueado, custódia disponível e bloqueada, além do status de uma
ordem.

## 5. Operações previstas na máquina

As principais operações do sistema são:

```text
cadastrarCliente(clienteId)
cadastrarAtivo(ativoId)
depositarSaldo(clienteId, valorDeposito)
retirarSaldo(clienteId, valorRetirada)
creditarCustodia(clienteId, ativoId, quantidade)
abrirOrdemCompra(ordemId, clienteId, ativoId, quantidade, precoLimite)
abrirOrdemVenda(ordemId, clienteId, ativoId, quantidade, precoLimite)
executarCasamento(ordemCompra, ordemVenda, quantidadeExecucao, precoExecucao)
cancelarOrdem(ordemId)
consultarSaldo(clienteId)
consultarCustodia(clienteId, ativoId)
consultarStatusOrdem(ordemId)
```

As operações retornam um código indicando sucesso ou erro. Dessa forma, a interface externa pode apenas chamar as
operações e exibir o resultado, sem precisar implementar as regras de negócio fora do modelo formal.

## 6. Invariantes principais

Os invariantes garantem que o sistema permaneça consistente após cada operação. Entre as propriedades principais, estão:

- clientes cadastrados pertencem ao intervalo válido de clientes;
- ativos cadastrados pertencem ao intervalo válido de ativos;
- saldos e custódias nunca assumem valores negativos;
- saldos e custódias respeitam os limites máximos definidos;
- ordens abertas sempre possuem cliente cadastrado, ativo cadastrado, quantidade positiva e preço positivo;
- ordens livres, canceladas ou executadas possuem quantidade igual a zero;
- uma ordem aberta sempre possui status coerente com seus dados;
- o sistema não permite compra sem saldo suficiente;
- o sistema não permite venda sem custódia suficiente;
- o casamento só ocorre entre ordens compatíveis.

## 7. Refinamento e implementação

Após a máquina abstrata, o projeto prevê um refinamento para aproximar a especificação da implementação. Nessa etapa,
conjuntos abstratos podem ser representados por estruturas mais concretas, como funções booleanas indicando se um
cliente ou ativo está cadastrado.

A custódia, inicialmente modelada como uma relação entre cliente e ativo, pode ser refinada para uma estrutura indexada
numericamente, facilitando a implementação em B0 e a geração de código.

Por fim, a implementação deve respeitar as restrições do subconjunto B0, usando estruturas finitas, operações mais
simples e atribuições compatíveis com a geração automática de código pelo Atelier-B.

## 8. Interface externa

A interface do sistema será feita em C e terá o papel de permitir a interação com o código gerado. Ela não deve acessar
diretamente variáveis internas da máquina. Sua função será apenas chamar as operações declaradas nos arquivos `.h`
gerados pelo Atelier-B, receber os resultados e apresentá-los ao usuário.

Essa interface permitirá demonstrar cenários como cadastro, abertura de ordens, execução total, execução parcial,
cancelamento e rejeição de operações inválidas.

MACHINE Bolsa_Valores_Abstract

SETS
  CONJUNTO_CLIENTES;
  CONJUNTO_ATIVOS;
  CONJUNTO_ORDENS;
  TIPO_LADO = {lado_compra, lado_venda};
  STATUS_ORDEM = {status_aberta, status_cancelada, status_executada}

VARIABLES
  conjuntoClientes,
  conjuntoAtivos,
  conjuntoOrdens,
  conjuntoOrdensAbertas,
  saldoClientes,
  custodiaClientes,
  donoOrdem,
  ativoOrdem,
  tipoLadoOrdem,
  quantidadeOrdem,
  precoOrdem,
  statusOrdem

INVARIANT
  conjuntoClientes <: CONJUNTO_CLIENTES &
  conjuntoAtivos <: CONJUNTO_ATIVOS &
  conjuntoOrdens <: CONJUNTO_ORDENS &
  conjuntoOrdensAbertas <: conjuntoOrdens &

  saldoClientes : conjuntoClientes --> NATURAL &
  custodiaClientes : (conjuntoClientes * conjuntoAtivos) +-> NATURAL &

  donoOrdem : conjuntoOrdens --> conjuntoClientes &
  ativoOrdem : conjuntoOrdens --> conjuntoAtivos &
  tipoLadoOrdem : conjuntoOrdens --> TIPO_LADO &
  quantidadeOrdem : conjuntoOrdens --> NATURAL1 &
  precoOrdem : conjuntoOrdens --> NATURAL1 &
  statusOrdem : conjuntoOrdens --> STATUS_ORDEM &
  conjuntoOrdensAbertas = (statusOrdem~)[{status_aberta}]

INITIALISATION
  conjuntoClientes := {} ||
  conjuntoAtivos := {} ||
  conjuntoOrdens := {} ||
  conjuntoOrdensAbertas := {} ||
  saldoClientes := {} ||
  custodiaClientes := {} ||
  donoOrdem := {} ||
  ativoOrdem := {} ||
  tipoLadoOrdem := {} ||
  quantidadeOrdem := {} ||
  precoOrdem := {} ||
  statusOrdem := {}

OPERATIONS

  cadastrarCliente(novoCliente) =
    PRE
      novoCliente : CONJUNTO_CLIENTES &
      novoCliente /: conjuntoClientes
    THEN
      conjuntoClientes := conjuntoClientes \/ {novoCliente} ||
      saldoClientes := saldoClientes \/ {novoCliente |-> 0}
    END;

  cadastrarAtivo(novoAtivo) =
    PRE
      novoAtivo : CONJUNTO_ATIVOS &
      novoAtivo /: conjuntoAtivos
    THEN
      conjuntoAtivos := conjuntoAtivos \/ {novoAtivo}
    END;

  depositarSaldo(cliente, valorDeposito) =
    PRE
      cliente : conjuntoClientes &
      valorDeposito : NATURAL1
    THEN
      saldoClientes := saldoClientes <+ 
        {cliente |-> saldoClientes(cliente) + valorDeposito}
    END;

  definirCustodia(cliente, ativo, quantidade) =
    PRE
      cliente : conjuntoClientes &
      ativo : conjuntoAtivos &
      quantidade : NATURAL
    THEN
      custodiaClientes := custodiaClientes <+ 
        {(cliente |-> ativo) |-> quantidade}
    END;

  abrirOrdemCompra(ordemId, cliente, ativo, quantidade, precoUnitario) =
    PRE
      ordemId : CONJUNTO_ORDENS &
      ordemId /: conjuntoOrdens &
      cliente : conjuntoClientes &
      ativo : conjuntoAtivos &
      quantidade : NATURAL1 &
      precoUnitario : NATURAL1 &
      saldoClientes(cliente) >= quantidade * precoUnitario
    THEN
      conjuntoOrdens := conjuntoOrdens \/ {ordemId} ||
      conjuntoOrdensAbertas := conjuntoOrdensAbertas \/ {ordemId} ||
      donoOrdem := donoOrdem \/ {ordemId |-> cliente} ||
      ativoOrdem := ativoOrdem \/ {ordemId |-> ativo} ||
      tipoLadoOrdem := tipoLadoOrdem \/ {ordemId |-> lado_compra} ||
      quantidadeOrdem := quantidadeOrdem \/ {ordemId |-> quantidade} ||
      precoOrdem := precoOrdem \/ {ordemId |-> precoUnitario} ||
      statusOrdem := statusOrdem \/ {ordemId |-> status_aberta}
    END;

  abrirOrdemVenda(ordemId, cliente, ativo, quantidade, precoUnitario) =
    PRE
      ordemId : CONJUNTO_ORDENS &
      ordemId /: conjuntoOrdens &
      cliente : conjuntoClientes &
      ativo : conjuntoAtivos &
      quantidade : NATURAL1 &
      precoUnitario : NATURAL1 &
      (cliente |-> ativo) : dom(custodiaClientes) &
      custodiaClientes(cliente |-> ativo) >= quantidade
    THEN
      conjuntoOrdens := conjuntoOrdens \/ {ordemId} ||
      conjuntoOrdensAbertas := conjuntoOrdensAbertas \/ {ordemId} ||
      donoOrdem := donoOrdem \/ {ordemId |-> cliente} ||
      ativoOrdem := ativoOrdem \/ {ordemId |-> ativo} ||
      tipoLadoOrdem := tipoLadoOrdem \/ {ordemId |-> lado_venda} ||
      quantidadeOrdem := quantidadeOrdem \/ {ordemId |-> quantidade} ||
      precoOrdem := precoOrdem \/ {ordemId |-> precoUnitario} ||
      statusOrdem := statusOrdem \/ {ordemId |-> status_aberta}
    END;

  cancelarOrdem(ordemId) =
    PRE
      ordemId : conjuntoOrdensAbertas
    THEN
      conjuntoOrdensAbertas := conjuntoOrdensAbertas - {ordemId} ||
      statusOrdem := statusOrdem <+ {ordemId |-> status_cancelada}
    END

END
MACHINE Bolsa_Valores_Abstract

SEES
  Bolsa_Valores_Ctx

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

  saldoClientes : conjuntoClientes --> NAT &
  custodiaClientes : (conjuntoClientes * conjuntoAtivos) +-> NAT &

  donoOrdem : conjuntoOrdens --> conjuntoClientes &
  ativoOrdem : conjuntoOrdens --> conjuntoAtivos &
  tipoLadoOrdem : conjuntoOrdens --> TIPO_LADO &
  quantidadeOrdem : conjuntoOrdens --> NAT1 &
  precoOrdem : conjuntoOrdens --> NAT1 &
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
      valorDeposito : NAT1
    THEN
      saldoClientes := saldoClientes <+ 
        {cliente |-> saldoClientes(cliente) + valorDeposito}
    END;

  definirCustodia(cliente, ativo, quantidade) =
    PRE
      cliente : conjuntoClientes &
      ativo : conjuntoAtivos &
      quantidade : NAT
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
      quantidade : NAT1 &
      precoUnitario : NAT1 &
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
      quantidade : NAT1 &
      precoUnitario : NAT1 &
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
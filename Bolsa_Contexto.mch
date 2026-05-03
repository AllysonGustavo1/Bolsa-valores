MACHINE Bolsa_Contexto

SETS
  TIPO_LADO = {lado_compra, lado_venda};
  STATUS_ORDEM = {status_livre, status_aberta, status_cancelada, status_executada}

CONCRETE_CONSTANTS
  MAX_CLIENTES,
  MAX_ATIVOS,
  MAX_ORDENS,
  MAX_DINHEIRO,
  MAX_QUANTIDADE,
  MAX_PRECO,
  ERRO,
  OK

PROPERTIES
  MAX_CLIENTES = 20 &
  MAX_ATIVOS = 10 &
  MAX_ORDENS = 50 &
  MAX_DINHEIRO = 1000000 &
  MAX_QUANTIDADE = 1000 &
  MAX_PRECO = 1000 &
  ERRO = 0 &
  OK = 1

END
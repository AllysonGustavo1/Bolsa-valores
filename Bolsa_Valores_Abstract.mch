MACHINE Bolsa_Valores_Abstract

DEFINITIONS
  CLIENTE_ID == (1..20);
  ATIVO_ID == (1..10);
  ORDEM_ID == (1..50);
  DINHEIRO == (0..1000000);
  VALOR_POSITIVO == (1..1000000);
  QUANTIDADE_TOTAL == (0..1000000);
  QUANTIDADE_VALIDA == (1..1000);
  PRECO_VALIDO == (1..1000);
  RESULTADO == (0..1);
  ERRO == 0;
  OK == 1

SETS
  TIPO_LADO = {lado_compra, lado_venda};
  STATUS_ORDEM = {status_livre, status_aberta, status_cancelada, status_executada}

VARIABLES
  clientes,
  ativos,
  saldoDisponivel,
  saldoBloqueado,
  custodiaDisponivel,
  custodiaBloqueada,
  donoOrdem,
  ativoOrdem,
  tipoLadoOrdem,
  quantidadeOrdem,
  precoLimiteOrdem,
  statusOrdem

INVARIANT
  clientes <: CLIENTE_ID &
  ativos <: ATIVO_ID &

  saldoDisponivel : CLIENTE_ID --> DINHEIRO &
  saldoBloqueado : CLIENTE_ID --> DINHEIRO &
  custodiaDisponivel : (CLIENTE_ID * ATIVO_ID) --> QUANTIDADE_TOTAL &
  custodiaBloqueada : (CLIENTE_ID * ATIVO_ID) --> QUANTIDADE_TOTAL &

  donoOrdem : ORDEM_ID --> CLIENTE_ID &
  ativoOrdem : ORDEM_ID --> ATIVO_ID &
  tipoLadoOrdem : ORDEM_ID --> TIPO_LADO &
  quantidadeOrdem : ORDEM_ID --> QUANTIDADE_TOTAL &
  precoLimiteOrdem : ORDEM_ID --> DINHEIRO &
  statusOrdem : ORDEM_ID --> STATUS_ORDEM &

  ((statusOrdem~)[{status_aberta}]) <: ((donoOrdem~)[clientes]) &
  ((statusOrdem~)[{status_aberta}]) <: ((ativoOrdem~)[ativos]) &
  ((statusOrdem~)[{status_aberta}]) <: ((quantidadeOrdem~)[QUANTIDADE_VALIDA]) &
  ((statusOrdem~)[{status_aberta}]) <: ((precoLimiteOrdem~)[PRECO_VALIDO]) &

  ((statusOrdem~)[{status_livre}]) <: ((quantidadeOrdem~)[{0}]) &
  ((statusOrdem~)[{status_livre}]) <: ((precoLimiteOrdem~)[{0}]) &
  ((statusOrdem~)[{status_cancelada}]) <: ((quantidadeOrdem~)[{0}]) &
  ((statusOrdem~)[{status_executada}]) <: ((quantidadeOrdem~)[{0}])

INITIALISATION
  clientes := {} ||
  ativos := {} ||
  saldoDisponivel := CLIENTE_ID * {0} ||
  saldoBloqueado := CLIENTE_ID * {0} ||
  custodiaDisponivel := (CLIENTE_ID * ATIVO_ID) * {0} ||
  custodiaBloqueada := (CLIENTE_ID * ATIVO_ID) * {0} ||
  donoOrdem := ORDEM_ID * {1} ||
  ativoOrdem := ORDEM_ID * {1} ||
  tipoLadoOrdem := ORDEM_ID * {lado_compra} ||
  quantidadeOrdem := ORDEM_ID * {0} ||
  precoLimiteOrdem := ORDEM_ID * {0} ||
  statusOrdem := ORDEM_ID * {status_livre}

OPERATIONS

  resultado <-- cadastrarCliente(clienteId) =
    PRE
      clienteId : CLIENTE_ID
    THEN
      IF clienteId /: clientes THEN
        clientes := clientes \/ {clienteId} ||
        saldoDisponivel := saldoDisponivel <+ {clienteId |-> 0} ||
        saldoBloqueado := saldoBloqueado <+ {clienteId |-> 0} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- cadastrarAtivo(ativoId) =
    PRE
      ativoId : ATIVO_ID
    THEN
      IF ativoId /: ativos THEN
        ativos := ativos \/ {ativoId} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- depositarSaldo(clienteId, valorDeposito) =
    PRE
      clienteId : CLIENTE_ID &
      valorDeposito : VALOR_POSITIVO
    THEN
      IF clienteId : clientes &
         saldoDisponivel(clienteId) + valorDeposito <= 1000000
      THEN
        saldoDisponivel := saldoDisponivel <+
          {clienteId |-> saldoDisponivel(clienteId) + valorDeposito} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- retirarSaldo(clienteId, valorRetirada) =
    PRE
      clienteId : CLIENTE_ID &
      valorRetirada : VALOR_POSITIVO
    THEN
      IF clienteId : clientes &
         saldoDisponivel(clienteId) >= valorRetirada
      THEN
        saldoDisponivel := saldoDisponivel <+
          {clienteId |-> saldoDisponivel(clienteId) - valorRetirada} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- creditarCustodia(clienteId, ativoId, quantidade) =
    PRE
      clienteId : CLIENTE_ID &
      ativoId : ATIVO_ID &
      quantidade : QUANTIDADE_VALIDA
    THEN
      IF clienteId : clientes &
         ativoId : ativos &
         custodiaDisponivel(clienteId |-> ativoId) + quantidade <= 1000000
      THEN
        custodiaDisponivel := custodiaDisponivel <+
          {(clienteId |-> ativoId) |-> custodiaDisponivel(clienteId |-> ativoId) + quantidade} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- abrirOrdemCompra(ordemId, clienteId, ativoId, quantidade, precoLimite) =
    PRE
      ordemId : ORDEM_ID &
      clienteId : CLIENTE_ID &
      ativoId : ATIVO_ID &
      quantidade : QUANTIDADE_VALIDA &
      precoLimite : PRECO_VALIDO
    THEN
      IF statusOrdem(ordemId) = status_livre &
         clienteId : clientes &
         ativoId : ativos &
         saldoDisponivel(clienteId) >= quantidade * precoLimite &
         saldoBloqueado(clienteId) + quantidade * precoLimite <= 1000000
      THEN
        saldoDisponivel := saldoDisponivel <+
          {clienteId |-> saldoDisponivel(clienteId) - quantidade * precoLimite} ||
        saldoBloqueado := saldoBloqueado <+
          {clienteId |-> saldoBloqueado(clienteId) + quantidade * precoLimite} ||
        donoOrdem := donoOrdem <+ {ordemId |-> clienteId} ||
        ativoOrdem := ativoOrdem <+ {ordemId |-> ativoId} ||
        tipoLadoOrdem := tipoLadoOrdem <+ {ordemId |-> lado_compra} ||
        quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> quantidade} ||
        precoLimiteOrdem := precoLimiteOrdem <+ {ordemId |-> precoLimite} ||
        statusOrdem := statusOrdem <+ {ordemId |-> status_aberta} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- abrirOrdemVenda(ordemId, clienteId, ativoId, quantidade, precoLimite) =
    PRE
      ordemId : ORDEM_ID &
      clienteId : CLIENTE_ID &
      ativoId : ATIVO_ID &
      quantidade : QUANTIDADE_VALIDA &
      precoLimite : PRECO_VALIDO
    THEN
      IF statusOrdem(ordemId) = status_livre &
         clienteId : clientes &
         ativoId : ativos &
         custodiaDisponivel(clienteId |-> ativoId) >= quantidade &
         custodiaBloqueada(clienteId |-> ativoId) + quantidade <= 1000000
      THEN
        custodiaDisponivel := custodiaDisponivel <+
          {(clienteId |-> ativoId) |-> custodiaDisponivel(clienteId |-> ativoId) - quantidade} ||
        custodiaBloqueada := custodiaBloqueada <+
          {(clienteId |-> ativoId) |-> custodiaBloqueada(clienteId |-> ativoId) + quantidade} ||
        donoOrdem := donoOrdem <+ {ordemId |-> clienteId} ||
        ativoOrdem := ativoOrdem <+ {ordemId |-> ativoId} ||
        tipoLadoOrdem := tipoLadoOrdem <+ {ordemId |-> lado_venda} ||
        quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> quantidade} ||
        precoLimiteOrdem := precoLimiteOrdem <+ {ordemId |-> precoLimite} ||
        statusOrdem := statusOrdem <+ {ordemId |-> status_aberta} ||
        resultado := OK
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- executarCasamento(ordemCompra, ordemVenda, quantidadeExecucao, precoExecucao) =
    PRE
      ordemCompra : ORDEM_ID &
      ordemVenda : ORDEM_ID &
      quantidadeExecucao : QUANTIDADE_VALIDA &
      precoExecucao : PRECO_VALIDO
    THEN
      IF ordemCompra /= ordemVenda &
         statusOrdem(ordemCompra) = status_aberta &
         statusOrdem(ordemVenda) = status_aberta &
         tipoLadoOrdem(ordemCompra) = lado_compra &
         tipoLadoOrdem(ordemVenda) = lado_venda &
         donoOrdem(ordemCompra) /= donoOrdem(ordemVenda) &
         ativoOrdem(ordemCompra) = ativoOrdem(ordemVenda) &
         precoLimiteOrdem(ordemCompra) >= precoLimiteOrdem(ordemVenda) &
         precoExecucao >= precoLimiteOrdem(ordemVenda) &
         precoExecucao <= precoLimiteOrdem(ordemCompra) &
         quantidadeExecucao <= quantidadeOrdem(ordemCompra) &
         quantidadeExecucao <= quantidadeOrdem(ordemVenda) &
         saldoBloqueado(donoOrdem(ordemCompra)) >= quantidadeExecucao * precoLimiteOrdem(ordemCompra) &
         custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) >= quantidadeExecucao &
         saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao) <= 1000000 &
         saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao <= 1000000 &
         custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao <= 1000000
      THEN
        IF quantidadeOrdem(ordemCompra) > quantidadeExecucao THEN
          IF quantidadeOrdem(ordemVenda) > quantidadeExecucao THEN
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemCompra) |->
                saldoBloqueado(donoOrdem(ordemCompra)) -
                quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||

            saldoDisponivel := saldoDisponivel <+
              {
                donoOrdem(ordemCompra) |->
                  saldoDisponivel(donoOrdem(ordemCompra)) +
                  quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao),
                donoOrdem(ordemVenda) |->
                  saldoDisponivel(donoOrdem(ordemVenda)) +
                  quantidadeExecucao * precoExecucao
              } ||

            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |->
                custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) -
                quantidadeExecucao} ||

            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |->
                custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) +
                quantidadeExecucao} ||

            quantidadeOrdem := quantidadeOrdem <+
              {
                ordemCompra |-> quantidadeOrdem(ordemCompra) - quantidadeExecucao,
                ordemVenda |-> quantidadeOrdem(ordemVenda) - quantidadeExecucao
              } ||

            resultado := OK
          ELSE
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemCompra) |->
                saldoBloqueado(donoOrdem(ordemCompra)) -
                quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||

            saldoDisponivel := saldoDisponivel <+
              {
                donoOrdem(ordemCompra) |->
                  saldoDisponivel(donoOrdem(ordemCompra)) +
                  quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao),
                donoOrdem(ordemVenda) |->
                  saldoDisponivel(donoOrdem(ordemVenda)) +
                  quantidadeExecucao * precoExecucao
              } ||

            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |->
                custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) -
                quantidadeExecucao} ||

            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |->
                custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) +
                quantidadeExecucao} ||

            quantidadeOrdem := quantidadeOrdem <+
              {
                ordemCompra |-> quantidadeOrdem(ordemCompra) - quantidadeExecucao,
                ordemVenda |-> 0
              } ||

            statusOrdem := statusOrdem <+ {ordemVenda |-> status_executada} ||

            resultado := OK
          END
        ELSE
          IF quantidadeOrdem(ordemVenda) > quantidadeExecucao THEN
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemCompra) |->
                saldoBloqueado(donoOrdem(ordemCompra)) -
                quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||

            saldoDisponivel := saldoDisponivel <+
              {
                donoOrdem(ordemCompra) |->
                  saldoDisponivel(donoOrdem(ordemCompra)) +
                  quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao),
                donoOrdem(ordemVenda) |->
                  saldoDisponivel(donoOrdem(ordemVenda)) +
                  quantidadeExecucao * precoExecucao
              } ||

            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |->
                custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) -
                quantidadeExecucao} ||

            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |->
                custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) +
                quantidadeExecucao} ||

            quantidadeOrdem := quantidadeOrdem <+
              {
                ordemCompra |-> 0,
                ordemVenda |-> quantidadeOrdem(ordemVenda) - quantidadeExecucao
              } ||

            statusOrdem := statusOrdem <+ {ordemCompra |-> status_executada} ||

            resultado := OK
          ELSE
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemCompra) |->
                saldoBloqueado(donoOrdem(ordemCompra)) -
                quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||

            saldoDisponivel := saldoDisponivel <+
              {
                donoOrdem(ordemCompra) |->
                  saldoDisponivel(donoOrdem(ordemCompra)) +
                  quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao),
                donoOrdem(ordemVenda) |->
                  saldoDisponivel(donoOrdem(ordemVenda)) +
                  quantidadeExecucao * precoExecucao
              } ||

            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |->
                custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) -
                quantidadeExecucao} ||

            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |->
                custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) +
                quantidadeExecucao} ||

            quantidadeOrdem := quantidadeOrdem <+
              {
                ordemCompra |-> 0,
                ordemVenda |-> 0
              } ||

            statusOrdem := statusOrdem <+
              {
                ordemCompra |-> status_executada,
                ordemVenda |-> status_executada
              } ||

            resultado := OK
          END
        END
      ELSE
        resultado := ERRO
      END
    END;

  resultado <-- cancelarOrdem(ordemId) =
    PRE
      ordemId : ORDEM_ID
    THEN
      IF statusOrdem(ordemId) = status_aberta THEN
        IF tipoLadoOrdem(ordemId) = lado_compra THEN
          IF saldoBloqueado(donoOrdem(ordemId)) >= quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) &
             saldoDisponivel(donoOrdem(ordemId)) + quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) <= 1000000
          THEN
            saldoDisponivel := saldoDisponivel <+
              {donoOrdem(ordemId) |->
                saldoDisponivel(donoOrdem(ordemId)) +
                quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemId) |->
                saldoBloqueado(donoOrdem(ordemId)) -
                quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemId |-> status_cancelada} ||
            resultado := OK
          ELSE
            resultado := ERRO
          END
        ELSE
          IF custodiaBloqueada(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) >= quantidadeOrdem(ordemId) &
             custodiaDisponivel(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) + quantidadeOrdem(ordemId) <= 1000000
          THEN
            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |->
                custodiaDisponivel(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) +
                quantidadeOrdem(ordemId)} ||
            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |->
                custodiaBloqueada(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) -
                quantidadeOrdem(ordemId)} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemId |-> status_cancelada} ||
            resultado := OK
          ELSE
            resultado := ERRO
          END
        END
      ELSE
        resultado := ERRO
      END
    END;

  valorDisponivel, valorBloqueado <-- consultarSaldo(clienteId) =
    PRE
      clienteId : CLIENTE_ID
    THEN
      valorDisponivel := saldoDisponivel(clienteId) ||
      valorBloqueado := saldoBloqueado(clienteId)
    END;

  quantidadeDisponivel, quantidadeBloqueada <-- consultarCustodia(clienteId, ativoId) =
    PRE
      clienteId : CLIENTE_ID &
      ativoId : ATIVO_ID
    THEN
      quantidadeDisponivel := custodiaDisponivel(clienteId |-> ativoId) ||
      quantidadeBloqueada := custodiaBloqueada(clienteId |-> ativoId)
    END;

  statusCodigo <-- consultarStatusOrdem(ordemId) =
    PRE
      ordemId : ORDEM_ID
    THEN
      IF statusOrdem(ordemId) = status_livre THEN
        statusCodigo := 0
      ELSE
        IF statusOrdem(ordemId) = status_aberta THEN
          statusCodigo := 1
        ELSE
          IF statusOrdem(ordemId) = status_cancelada THEN
            statusCodigo := 2
          ELSE
            statusCodigo := 3
          END
        END
      END
    END
END
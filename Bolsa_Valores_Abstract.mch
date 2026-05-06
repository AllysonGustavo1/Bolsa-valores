MACHINE Bolsa_Valores_Abstract

SEES
  Bolsa_Contexto

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
  clientes <: 1..MAX_CLIENTES &
  ativos <: 1..MAX_ATIVOS &

  saldoDisponivel : (1..MAX_CLIENTES) --> (0..MAX_DINHEIRO) &
  saldoBloqueado : (1..MAX_CLIENTES) --> (0..MAX_DINHEIRO) &

  custodiaDisponivel : ((1..MAX_CLIENTES) * (1..MAX_ATIVOS)) --> (0..MAX_DINHEIRO) &
  custodiaBloqueada : ((1..MAX_CLIENTES) * (1..MAX_ATIVOS)) --> (0..MAX_DINHEIRO) &

  donoOrdem : (1..MAX_ORDENS) --> (1..MAX_CLIENTES) &
  ativoOrdem : (1..MAX_ORDENS) --> (1..MAX_ATIVOS) &
  tipoLadoOrdem : (1..MAX_ORDENS) --> TIPO_LADO &
  quantidadeOrdem : (1..MAX_ORDENS) --> (0..MAX_QUANTIDADE) &
  precoLimiteOrdem : (1..MAX_ORDENS) --> (0..MAX_PRECO) &
  statusOrdem : (1..MAX_ORDENS) --> STATUS_ORDEM &

  donoOrdem[statusOrdem~[{status_aberta}]] <: clientes &
  ativoOrdem[statusOrdem~[{status_aberta}]] <: ativos &
  quantidadeOrdem[statusOrdem~[{status_aberta}]] <: 1..MAX_QUANTIDADE &
  precoLimiteOrdem[statusOrdem~[{status_aberta}]] <: 1..MAX_PRECO &
  quantidadeOrdem[(1..MAX_ORDENS) - statusOrdem~[{status_aberta}]] <: {0} &
  precoLimiteOrdem[statusOrdem~[{status_livre}]] <: {0}

INITIALISATION
  clientes := {} ||
  ativos := {} ||
  saldoDisponivel := (1..MAX_CLIENTES) * {0} ||
  saldoBloqueado := (1..MAX_CLIENTES) * {0} ||
  custodiaDisponivel := ((1..MAX_CLIENTES) * (1..MAX_ATIVOS)) * {0} ||
  custodiaBloqueada := ((1..MAX_CLIENTES) * (1..MAX_ATIVOS)) * {0} ||
  donoOrdem := (1..MAX_ORDENS) * {1} ||
  ativoOrdem := (1..MAX_ORDENS) * {1} ||
  tipoLadoOrdem := (1..MAX_ORDENS) * {lado_compra} ||
  quantidadeOrdem := (1..MAX_ORDENS) * {0} ||
  precoLimiteOrdem := (1..MAX_ORDENS) * {0} ||
  statusOrdem := (1..MAX_ORDENS) * {status_livre}

OPERATIONS

  resultado <-- cadastrarCliente(clienteId) =
    PRE
      clienteId : 1..MAX_CLIENTES
    THEN
      IF clienteId /: clientes THEN
        clientes := clientes \/ {clienteId} ||
        saldoDisponivel := saldoDisponivel <+ {clienteId |-> 0} ||
        saldoBloqueado := saldoBloqueado <+ {clienteId |-> 0} ||
        resultado := OK
      ELSE
        resultado := ERRO_CLIENTE_JA_CADASTRADO
      END
    END;

  resultado <-- cadastrarAtivo(ativoId) =
    PRE
      ativoId : 1..MAX_ATIVOS
    THEN
      IF ativoId /: ativos THEN
        ativos := ativos \/ {ativoId} ||
        resultado := OK
      ELSE
        resultado := ERRO_ATIVO_JA_CADASTRADO
      END
    END;

  resultado <-- depositarSaldo(clienteId, valorDeposito) =
    PRE
      clienteId : 1..MAX_CLIENTES &
      valorDeposito : 1..MAX_DINHEIRO
    THEN
      IF clienteId : clientes &
         saldoDisponivel(clienteId) + valorDeposito <= MAX_DINHEIRO
      THEN
        saldoDisponivel := saldoDisponivel <+
          {clienteId |-> saldoDisponivel(clienteId) + valorDeposito} ||
        resultado := OK
      ELSE
        resultado := ERRO_DEPOSITO_INVALIDO
      END
    END;

  resultado <-- retirarSaldo(clienteId, valorRetirada) =
    PRE
      clienteId : 1..MAX_CLIENTES &
      valorRetirada : 1..MAX_DINHEIRO
    THEN
      IF clienteId : clientes &
         saldoDisponivel(clienteId) >= valorRetirada
      THEN
        saldoDisponivel := saldoDisponivel <+
          {clienteId |-> saldoDisponivel(clienteId) - valorRetirada} ||
        resultado := OK
      ELSE
        resultado := ERRO_SALDO_INSUFICIENTE
      END
    END;

  resultado <-- creditarCustodia(clienteId, ativoId, quantidade) =
    PRE
      clienteId : 1..MAX_CLIENTES &
      ativoId : 1..MAX_ATIVOS &
      quantidade : 1..MAX_QUANTIDADE
    THEN
      IF clienteId : clientes &
         ativoId : ativos &
         custodiaDisponivel(clienteId |-> ativoId) + quantidade <= MAX_DINHEIRO
      THEN
        custodiaDisponivel := custodiaDisponivel <+
          {(clienteId |-> ativoId) |-> custodiaDisponivel(clienteId |-> ativoId) + quantidade} ||
        resultado := OK
      ELSE
        resultado := ERRO_CUSTODIA_INVALIDA
      END
    END;

  resultado <-- abrirOrdemCompra(ordemId, clienteId, ativoId, quantidade, precoLimite) =
    PRE
      ordemId : 1..MAX_ORDENS &
      clienteId : 1..MAX_CLIENTES &
      ativoId : 1..MAX_ATIVOS &
      quantidade : 1..MAX_QUANTIDADE &
      precoLimite : 1..MAX_PRECO
    THEN
      IF statusOrdem(ordemId) = status_livre &
         clienteId : clientes &
         ativoId : ativos &
         saldoDisponivel(clienteId) >= quantidade * precoLimite &
         saldoBloqueado(clienteId) + quantidade * precoLimite <= MAX_DINHEIRO
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
        resultado := ERRO_ABERTURA_COMPRA_INVALIDA
      END
    END;

  resultado <-- abrirOrdemVenda(ordemId, clienteId, ativoId, quantidade, precoLimite) =
    PRE
      ordemId : 1..MAX_ORDENS &
      clienteId : 1..MAX_CLIENTES &
      ativoId : 1..MAX_ATIVOS &
      quantidade : 1..MAX_QUANTIDADE &
      precoLimite : 1..MAX_PRECO
    THEN
      IF statusOrdem(ordemId) = status_livre &
         clienteId : clientes &
         ativoId : ativos &
         custodiaDisponivel(clienteId |-> ativoId) >= quantidade &
         custodiaBloqueada(clienteId |-> ativoId) + quantidade <= MAX_DINHEIRO
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
        resultado := ERRO_ABERTURA_VENDA_INVALIDA
      END
    END;

  resultado <-- executarCasamento(ordemCompra, ordemVenda, quantidadeExecucao, precoExecucao) =
    PRE
      ordemCompra : 1..MAX_ORDENS &
      ordemVenda : 1..MAX_ORDENS &
      quantidadeExecucao : 1..MAX_QUANTIDADE &
      precoExecucao : 1..MAX_PRECO
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
         saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao) <= MAX_DINHEIRO &
         saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao <= MAX_DINHEIRO &
         custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao <= MAX_DINHEIRO
      THEN
        ANY novaQuantidadeCompra, novaQuantidadeVenda, novoStatusCompra, novoStatusVenda
        WHERE
          novaQuantidadeCompra = quantidadeOrdem(ordemCompra) - quantidadeExecucao &
          novaQuantidadeVenda = quantidadeOrdem(ordemVenda) - quantidadeExecucao &
          novaQuantidadeCompra : 0..MAX_QUANTIDADE &
          novaQuantidadeVenda : 0..MAX_QUANTIDADE &
          novoStatusCompra : STATUS_ORDEM &
          novoStatusVenda : STATUS_ORDEM &
          (novaQuantidadeCompra = 0 => novoStatusCompra = status_executada) &
          (novaQuantidadeCompra > 0 => novoStatusCompra = status_aberta) &
          (novaQuantidadeVenda = 0 => novoStatusVenda = status_executada) &
          (novaQuantidadeVenda > 0 => novoStatusVenda = status_aberta)
        THEN
          saldoBloqueado := saldoBloqueado <+
            {donoOrdem(ordemCompra) |->
              saldoBloqueado(donoOrdem(ordemCompra)) -
              quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||

          saldoDisponivel := saldoDisponivel <+
            {donoOrdem(ordemCompra) |->
              saldoDisponivel(donoOrdem(ordemCompra)) +
              quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao),
             donoOrdem(ordemVenda) |->
              saldoDisponivel(donoOrdem(ordemVenda)) +
              quantidadeExecucao * precoExecucao} ||

          custodiaBloqueada := custodiaBloqueada <+
            {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |->
              custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) -
              quantidadeExecucao} ||

          custodiaDisponivel := custodiaDisponivel <+
            {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |->
              custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) +
              quantidadeExecucao} ||

          quantidadeOrdem := quantidadeOrdem <+
            {ordemCompra |-> novaQuantidadeCompra,
             ordemVenda |-> novaQuantidadeVenda} ||

          statusOrdem := statusOrdem <+
            {ordemCompra |-> novoStatusCompra,
             ordemVenda |-> novoStatusVenda} ||

          resultado := OK
        END
      ELSE
        resultado := ERRO_CASAMENTO_INVALIDO
      END
    END;

  resultado <-- cancelarOrdem(ordemId) =
    PRE
      ordemId : 1..MAX_ORDENS
    THEN
      IF statusOrdem(ordemId) = status_aberta THEN
        IF tipoLadoOrdem(ordemId) = lado_compra THEN
          IF saldoBloqueado(donoOrdem(ordemId)) >= quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) &
             saldoDisponivel(donoOrdem(ordemId)) + quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) <= MAX_DINHEIRO
          THEN
            saldoDisponivel := saldoDisponivel <+
              {donoOrdem(ordemId) |-> saldoDisponivel(donoOrdem(ordemId)) + quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
            saldoBloqueado := saldoBloqueado <+
              {donoOrdem(ordemId) |-> saldoBloqueado(donoOrdem(ordemId)) - quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemId |-> status_cancelada} ||
            resultado := OK
          ELSE
            resultado := ERRO_LIMITE_EXCEDIDO
          END
        ELSE
          IF custodiaBloqueada(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) >= quantidadeOrdem(ordemId) &
             custodiaDisponivel(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) + quantidadeOrdem(ordemId) <= MAX_DINHEIRO
          THEN
            custodiaDisponivel := custodiaDisponivel <+
              {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |->
                custodiaDisponivel(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) + quantidadeOrdem(ordemId)} ||
            custodiaBloqueada := custodiaBloqueada <+
              {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |->
                custodiaBloqueada(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) - quantidadeOrdem(ordemId)} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemId |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemId |-> status_cancelada} ||
            resultado := OK
          ELSE
            resultado := ERRO_LIMITE_EXCEDIDO
          END
        END
      ELSE
        resultado := ERRO_ORDEM_NAO_ABERTA
      END
    END;

  valorDisponivel, valorBloqueado <-- consultarSaldo(clienteId) =
    PRE
      clienteId : 1..MAX_CLIENTES
    THEN
      valorDisponivel := saldoDisponivel(clienteId) ||
      valorBloqueado := saldoBloqueado(clienteId)
    END;

  quantidadeDisponivel, quantidadeBloqueada <-- consultarCustodia(clienteId, ativoId) =
    PRE
      clienteId : 1..MAX_CLIENTES &
      ativoId : 1..MAX_ATIVOS
    THEN
      quantidadeDisponivel := custodiaDisponivel(clienteId |-> ativoId) ||
      quantidadeBloqueada := custodiaBloqueada(clienteId |-> ativoId)
    END;

  statusCodigo <-- consultarStatusOrdem(ordemId) =
    PRE
      ordemId : 1..MAX_ORDENS
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
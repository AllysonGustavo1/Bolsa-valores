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

  !(ordemId).(
    ordemId : (1..MAX_ORDENS) & statusOrdem(ordemId) = status_aberta =>
      donoOrdem(ordemId) : clientes &
      ativoOrdem(ordemId) : ativos &
      quantidadeOrdem(ordemId) : (1..MAX_QUANTIDADE) &
      precoLimiteOrdem(ordemId) : (1..MAX_PRECO)
  ) &

  !(ordemId).(
    ordemId : (1..MAX_ORDENS) & statusOrdem(ordemId) /= status_aberta =>
      quantidadeOrdem(ordemId) = 0
  ) &

  !(ordemId).(
    ordemId : (1..MAX_ORDENS) & statusOrdem(ordemId) = status_livre =>
      precoLimiteOrdem(ordemId) = 0
  )

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

  /* 
  * Cadastra um novo cliente no sistema. 
  * Verifica se o identificador fornecido ainda não pertence ao conjunto de clientes e, caso não pertença, 
  * o adiciona, inicializando seu saldo disponível e saldo bloqueado com zero. 
   */
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

  /* 
  * Cadastra um novo ativo na bolsa. 
  * Verifica se o identificador do ativo ainda não está no conjunto de ativos listados e o insere, 
  * permitindo que ele seja negociado. 
   */
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

  /* 
  * Realiza o depósito de um valor no saldo de um cliente. 
  * Valida se o cliente já existe e garante que a soma do saldo atual com o valor do depósito 
  * não ultrapasse o limite máximo estabelecido no contexto (MAX_DINHEIRO). 
   */
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

  /* 
  * Efetua a retirada de um valor do saldo disponível de um cliente. 
  * Verifica se o cliente está cadastrado e se ele possui saldo disponível suficiente para o saque, 
  * garantindo que o saldo bloqueado em ordens não seja afetado. 
   */
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

  /* 
  * Adiciona uma quantidade de um determinado ativo à custódia do cliente. 
  * Funciona como uma injeção de posição inicial para que o cliente possa criar ordens de venda futuramente. 
   */
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

  /* 
  * Registra uma intenção de compra de um ativo. 
  * Valida se a ordem está livre e se o cliente tem saldo disponível suficiente para arcar com o custo total (quantidade * preco). 
  * Se válido, o valor necessário sai do saldo disponível e vai para o saldo bloqueado, e a ordem é gravada como aberta. 
   */
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

  /* 
  * Registra uma intenção de venda de um ativo. 
  * Verifica se a ordem está livre e se o cliente tem custódia (quantidade do ativo) disponível suficiente. 
  * Transfere os ativos da custódia disponível para a custódia bloqueada e grava os detalhes da ordem. 
   */
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

  /* 
  * Processa a negociação combinando uma ordem de compra e uma de venda abertas. 
  * Valida inúmeras regras: as ordens devem ter lados opostos, donos diferentes, negociar o mesmo ativo, 
  * preços compatíveis e quantidades válidas. 
  * Após a validação, reduz o saldo bloqueado do comprador e credita a ele a custódia, ao mesmo tempo que debita 
  * a custódia bloqueada do vendedor e lhe repassa o valor financeiro da venda. 
  * Lida com os cenários de execução total ou parcial, marcando a ordem como executada caso a quantidade chegue a zero. 
   */
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
        IF quantidadeOrdem(ordemCompra) > quantidadeExecucao THEN
          IF quantidadeOrdem(ordemVenda) > quantidadeExecucao THEN
            saldoBloqueado := saldoBloqueado <+ {donoOrdem(ordemCompra) |-> saldoBloqueado(donoOrdem(ordemCompra)) - quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||
            saldoDisponivel := saldoDisponivel <+ {donoOrdem(ordemCompra) |-> saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao), donoOrdem(ordemVenda) |-> saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao} ||
            custodiaBloqueada := custodiaBloqueada <+ {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |-> custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) - quantidadeExecucao} ||
            custodiaDisponivel := custodiaDisponivel <+ {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |-> custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemCompra |-> quantidadeOrdem(ordemCompra) - quantidadeExecucao, ordemVenda |-> quantidadeOrdem(ordemVenda) - quantidadeExecucao} ||
            resultado := OK
          ELSE
            saldoBloqueado := saldoBloqueado <+ {donoOrdem(ordemCompra) |-> saldoBloqueado(donoOrdem(ordemCompra)) - quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||
            saldoDisponivel := saldoDisponivel <+ {donoOrdem(ordemCompra) |-> saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao), donoOrdem(ordemVenda) |-> saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao} ||
            custodiaBloqueada := custodiaBloqueada <+ {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |-> custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) - quantidadeExecucao} ||
            custodiaDisponivel := custodiaDisponivel <+ {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |-> custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemCompra |-> quantidadeOrdem(ordemCompra) - quantidadeExecucao, ordemVenda |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemVenda |-> status_executada} ||
            resultado := OK
          END
        ELSE
          IF quantidadeOrdem(ordemVenda) > quantidadeExecucao THEN
            saldoBloqueado := saldoBloqueado <+ {donoOrdem(ordemCompra) |-> saldoBloqueado(donoOrdem(ordemCompra)) - quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||
            saldoDisponivel := saldoDisponivel <+ {donoOrdem(ordemCompra) |-> saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao), donoOrdem(ordemVenda) |-> saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao} ||
            custodiaBloqueada := custodiaBloqueada <+ {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |-> custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) - quantidadeExecucao} ||
            custodiaDisponivel := custodiaDisponivel <+ {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |-> custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemCompra |-> 0, ordemVenda |-> quantidadeOrdem(ordemVenda) - quantidadeExecucao} ||
            statusOrdem := statusOrdem <+ {ordemCompra |-> status_executada} ||
            resultado := OK
          ELSE
            saldoBloqueado := saldoBloqueado <+ {donoOrdem(ordemCompra) |-> saldoBloqueado(donoOrdem(ordemCompra)) - quantidadeExecucao * precoLimiteOrdem(ordemCompra)} ||
            saldoDisponivel := saldoDisponivel <+ {donoOrdem(ordemCompra) |-> saldoDisponivel(donoOrdem(ordemCompra)) + quantidadeExecucao * (precoLimiteOrdem(ordemCompra) - precoExecucao), donoOrdem(ordemVenda) |-> saldoDisponivel(donoOrdem(ordemVenda)) + quantidadeExecucao * precoExecucao} ||
            custodiaBloqueada := custodiaBloqueada <+ {(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) |-> custodiaBloqueada(donoOrdem(ordemVenda) |-> ativoOrdem(ordemVenda)) - quantidadeExecucao} ||
            custodiaDisponivel := custodiaDisponivel <+ {(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) |-> custodiaDisponivel(donoOrdem(ordemCompra) |-> ativoOrdem(ordemCompra)) + quantidadeExecucao} ||
            quantidadeOrdem := quantidadeOrdem <+ {ordemCompra |-> 0, ordemVenda |-> 0} ||
            statusOrdem := statusOrdem <+ {ordemCompra |-> status_executada, ordemVenda |-> status_executada} ||
            resultado := OK
          END
        END
      ELSE
        resultado := ERRO_CASAMENTO_INVALIDO
      END
    END;

  /* 
  * Cancela uma ordem que ainda não foi totalmente executada. 
  * Se for uma ordem de compra, remove o saldo retido do saldo bloqueado e o devolve para o disponível. 
  * Se for de venda, estorna a quantidade retida na custódia bloqueada de volta para a custódia disponível do ativo. 
  * Zera a quantidade restante e muda o status da ordem para cancelada. 
   */
  resultado <-- cancelarOrdem(ordemId) =
    PRE
      ordemId : 1..MAX_ORDENS
    THEN
      IF statusOrdem(ordemId) = status_aberta THEN
        IF tipoLadoOrdem(ordemId) = lado_compra THEN
          IF saldoBloqueado(donoOrdem(ordemId)) >= quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) &
             saldoDisponivel(donoOrdem(ordemId)) + quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId) <= MAX_DINHEIRO
          THEN
            saldoDisponivel := saldoDisponivel <+ {donoOrdem(ordemId) |-> saldoDisponivel(donoOrdem(ordemId)) + quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
            saldoBloqueado := saldoBloqueado <+ {donoOrdem(ordemId) |-> saldoBloqueado(donoOrdem(ordemId)) - quantidadeOrdem(ordemId) * precoLimiteOrdem(ordemId)} ||
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
            custodiaDisponivel := custodiaDisponivel <+ {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |-> custodiaDisponivel(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) + quantidadeOrdem(ordemId)} ||
            custodiaBloqueada := custodiaBloqueada <+ {(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) |-> custodiaBloqueada(donoOrdem(ordemId) |-> ativoOrdem(ordemId)) - quantidadeOrdem(ordemId)} ||
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

  /* 
  * Recupera o saldo atual de um cliente. 
  * Retorna os valores presentes nos vetores de saldo disponível e bloqueado para o cliente fornecido. 
   */
  valorDisponivel, valorBloqueado <-- consultarSaldo(clienteId) =
    PRE
      clienteId : 1..MAX_CLIENTES
    THEN
      valorDisponivel := saldoDisponivel(clienteId) ||
      valorBloqueado := saldoBloqueado(clienteId)
    END;

  /* 
  * Recupera as posições de custódia de um cliente em um ativo específico. 
  * Retorna as quantidades atualmente disponíveis (livres para negociação) e bloqueadas (retidas em ordens de venda). 
   */
  quantidadeDisponivel, quantidadeBloqueada <-- consultarCustodia(clienteId, ativoId) =
    PRE
      clienteId : 1..MAX_CLIENTES &
      ativoId : 1..MAX_ATIVOS
    THEN
      quantidadeDisponivel := custodiaDisponivel(clienteId |-> ativoId) ||
      quantidadeBloqueada := custodiaBloqueada(clienteId |-> ativoId)
    END;

  /* 
  * Consulta a situação atual de uma ordem no sistema. 
  * Retorna um código numérico de acordo com o status (0: livre, 1: aberta, 2: cancelada, 3: executada) para a interface externa lidar facilmente. 
   */
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
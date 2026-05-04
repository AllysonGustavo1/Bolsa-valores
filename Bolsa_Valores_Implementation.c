/* WARNING if type checker is not performed, translation could contain errors ! */

#include "Bolsa_Valores_Abstract.h"

/* Clause SEES */
#include "Bolsa_Contexto.h"

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */

/* Array and record constants */
/* Clause CONCRETE_VARIABLES */

static bool Bolsa_Valores_Abstract__clienteAtivoI[20];
static bool Bolsa_Valores_Abstract__ativoListadoI[10];
static int32_t Bolsa_Valores_Abstract__saldoDisponivelI[20];
static int32_t Bolsa_Valores_Abstract__saldoBloqueadoI[20];
static int32_t Bolsa_Valores_Abstract__custodiaDisponivelI[200];
static int32_t Bolsa_Valores_Abstract__custodiaBloqueadaI[200];
static int32_t Bolsa_Valores_Abstract__donoOrdemI[50];
static int32_t Bolsa_Valores_Abstract__ativoOrdemI[50];
static Bolsa_Contexto__TIPO_LADO Bolsa_Valores_Abstract__tipoLadoOrdemI[50];
static int32_t Bolsa_Valores_Abstract__quantidadeOrdemI[50];
static int32_t Bolsa_Valores_Abstract__precoLimiteOrdemI[50];
static Bolsa_Contexto__STATUS_ORDEM Bolsa_Valores_Abstract__statusOrdemI[50];
/* Clause INITIALISATION */
void Bolsa_Valores_Abstract__INITIALISATION(void)
{
    
    unsigned int i = 0;
    for(i = 0; i <= 19;i++)
    {
        Bolsa_Valores_Abstract__clienteAtivoI[i] = false;
    }
    for(i = 0; i <= 9;i++)
    {
        Bolsa_Valores_Abstract__ativoListadoI[i] = false;
    }
    for(i = 0; i <= 19;i++)
    {
        Bolsa_Valores_Abstract__saldoDisponivelI[i] = 0;
    }
    for(i = 0; i <= 19;i++)
    {
        Bolsa_Valores_Abstract__saldoBloqueadoI[i] = 0;
    }
    for(i = 0; i <= 199;i++)
    {
        Bolsa_Valores_Abstract__custodiaDisponivelI[i] = 0;
    }
    for(i = 0; i <= 199;i++)
    {
        Bolsa_Valores_Abstract__custodiaBloqueadaI[i] = 0;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__donoOrdemI[i] = 1;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__ativoOrdemI[i] = 1;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__tipoLadoOrdemI[i] = Bolsa_Contexto__lado_compra;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__quantidadeOrdemI[i] = 0;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__precoLimiteOrdemI[i] = 0;
    }
    for(i = 0; i <= 49;i++)
    {
        Bolsa_Valores_Abstract__statusOrdemI[i] = Bolsa_Contexto__status_livre;
    }
}

/* Clause OPERATIONS */

void Bolsa_Valores_Abstract__cadastrarCliente(int32_t clienteId, int32_t *resultado)
{
    {
        int32_t idxCliente;
        bool cadastroAtual;
        
        idxCliente = clienteId-1;
        cadastroAtual = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        if(cadastroAtual == false)
        {
            Bolsa_Valores_Abstract__clienteAtivoI[idxCliente] = true;
            Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente] = 0;
            Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente] = 0;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_CLIENTE_JA_CADASTRADO;
        }
    }
}

void Bolsa_Valores_Abstract__cadastrarAtivo(int32_t ativoId, int32_t *resultado)
{
    {
        int32_t idxAtivo;
        bool cadastroAtual;
        
        idxAtivo = ativoId-1;
        cadastroAtual = Bolsa_Valores_Abstract__ativoListadoI[idxAtivo];
        if(cadastroAtual == false)
        {
            Bolsa_Valores_Abstract__ativoListadoI[idxAtivo] = true;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_ATIVO_JA_CADASTRADO;
        }
    }
}

void Bolsa_Valores_Abstract__depositarSaldo(int32_t clienteId, int32_t valorDeposito, int32_t *resultado)
{
    {
        int32_t idxCliente;
        int32_t saldoAtual;
        int32_t novoSaldo;
        bool clienteCadastrado;
        
        idxCliente = clienteId-1;
        clienteCadastrado = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        saldoAtual = Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente];
        novoSaldo = saldoAtual+valorDeposito;
        if((clienteCadastrado == true) &&
        ((novoSaldo) <= (1000000)))
        {
            Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente] = novoSaldo;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_DEPOSITO_INVALIDO;
        }
    }
}

void Bolsa_Valores_Abstract__retirarSaldo(int32_t clienteId, int32_t valorRetirada, int32_t *resultado)
{
    {
        int32_t idxCliente;
        int32_t saldoAtual;
        int32_t novoSaldo;
        bool clienteCadastrado;
        
        idxCliente = clienteId-1;
        clienteCadastrado = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        saldoAtual = Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente];
        if((clienteCadastrado == true) &&
        ((saldoAtual) >= (valorRetirada)))
        {
            novoSaldo = saldoAtual-valorRetirada;
            Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente] = novoSaldo;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_SALDO_INSUFICIENTE;
        }
    }
}

void Bolsa_Valores_Abstract__creditarCustodia(int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t *resultado)
{
    {
        int32_t idxCliente;
        int32_t idxAtivo;
        int32_t idxCustodia;
        bool clienteCadastrado;
        bool ativoCadastrado;
        int32_t custodiaAtual;
        int32_t novaCustodia;
        
        idxCliente = clienteId-1;
        idxAtivo = ativoId-1;
        idxCustodia = (idxCliente * 10)+idxAtivo;
        clienteCadastrado = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        ativoCadastrado = Bolsa_Valores_Abstract__ativoListadoI[idxAtivo];
        custodiaAtual = Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia];
        novaCustodia = custodiaAtual+quantidade;
        if(((clienteCadastrado == true) &&
            (ativoCadastrado == true)) &&
        ((novaCustodia) <= (1000000)))
        {
            Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia] = novaCustodia;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_CUSTODIA_INVALIDA;
        }
    }
}

void Bolsa_Valores_Abstract__abrirOrdemCompra(int32_t ordemId, int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t precoLimite, int32_t *resultado)
{
    {
        int32_t idxOrdem;
        int32_t idxCliente;
        int32_t idxAtivo;
        bool clienteCadastrado;
        bool ativoCadastrado;
        Bolsa_Contexto__STATUS_ORDEM statusAtual;
        int32_t saldoAtual;
        int32_t bloqueadoAtual;
        int32_t valorOrdem;
        int32_t novoSaldoDisponivel;
        int32_t novoSaldoBloqueado;
        
        idxOrdem = ordemId-1;
        idxCliente = clienteId-1;
        idxAtivo = ativoId-1;
        clienteCadastrado = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        ativoCadastrado = Bolsa_Valores_Abstract__ativoListadoI[idxAtivo];
        statusAtual = Bolsa_Valores_Abstract__statusOrdemI[idxOrdem];
        saldoAtual = Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente];
        bloqueadoAtual = Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente];
        valorOrdem = quantidade * precoLimite;
        novoSaldoBloqueado = bloqueadoAtual+valorOrdem;
        if(((((statusAtual == Bolsa_Contexto__status_livre) &&
                    (clienteCadastrado == true)) &&
                (ativoCadastrado == true)) &&
            ((saldoAtual) >= (valorOrdem))) &&
        ((novoSaldoBloqueado) <= (1000000)))
        {
            novoSaldoDisponivel = saldoAtual-valorOrdem;
            Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente] = novoSaldoDisponivel;
            Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente] = novoSaldoBloqueado;
            Bolsa_Valores_Abstract__donoOrdemI[idxOrdem] = clienteId;
            Bolsa_Valores_Abstract__ativoOrdemI[idxOrdem] = ativoId;
            Bolsa_Valores_Abstract__tipoLadoOrdemI[idxOrdem] = Bolsa_Contexto__lado_compra;
            Bolsa_Valores_Abstract__quantidadeOrdemI[idxOrdem] = quantidade;
            Bolsa_Valores_Abstract__precoLimiteOrdemI[idxOrdem] = precoLimite;
            Bolsa_Valores_Abstract__statusOrdemI[idxOrdem] = Bolsa_Contexto__status_aberta;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_ABERTURA_COMPRA_INVALIDA;
        }
    }
}

void Bolsa_Valores_Abstract__abrirOrdemVenda(int32_t ordemId, int32_t clienteId, int32_t ativoId, int32_t quantidade, int32_t precoLimite, int32_t *resultado)
{
    {
        int32_t idxOrdem;
        int32_t idxCliente;
        int32_t idxAtivo;
        int32_t idxCustodia;
        bool clienteCadastrado;
        bool ativoCadastrado;
        Bolsa_Contexto__STATUS_ORDEM statusAtual;
        int32_t custodiaAtual;
        int32_t custodiaBloqueadaAtual;
        int32_t novaCustodiaDisponivel;
        int32_t novaCustodiaBloqueada;
        
        idxOrdem = ordemId-1;
        idxCliente = clienteId-1;
        idxAtivo = ativoId-1;
        idxCustodia = (idxCliente * 10)+idxAtivo;
        clienteCadastrado = Bolsa_Valores_Abstract__clienteAtivoI[idxCliente];
        ativoCadastrado = Bolsa_Valores_Abstract__ativoListadoI[idxAtivo];
        statusAtual = Bolsa_Valores_Abstract__statusOrdemI[idxOrdem];
        custodiaAtual = Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia];
        custodiaBloqueadaAtual = Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodia];
        novaCustodiaBloqueada = custodiaBloqueadaAtual+quantidade;
        if(((((statusAtual == Bolsa_Contexto__status_livre) &&
                    (clienteCadastrado == true)) &&
                (ativoCadastrado == true)) &&
            ((custodiaAtual) >= (quantidade))) &&
        ((novaCustodiaBloqueada) <= (1000000)))
        {
            novaCustodiaDisponivel = custodiaAtual-quantidade;
            Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia] = novaCustodiaDisponivel;
            Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodia] = novaCustodiaBloqueada;
            Bolsa_Valores_Abstract__donoOrdemI[idxOrdem] = clienteId;
            Bolsa_Valores_Abstract__ativoOrdemI[idxOrdem] = ativoId;
            Bolsa_Valores_Abstract__tipoLadoOrdemI[idxOrdem] = Bolsa_Contexto__lado_venda;
            Bolsa_Valores_Abstract__quantidadeOrdemI[idxOrdem] = quantidade;
            Bolsa_Valores_Abstract__precoLimiteOrdemI[idxOrdem] = precoLimite;
            Bolsa_Valores_Abstract__statusOrdemI[idxOrdem] = Bolsa_Contexto__status_aberta;
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_ABERTURA_VENDA_INVALIDA;
        }
    }
}

void Bolsa_Valores_Abstract__executarCasamento(int32_t ordemCompra, int32_t ordemVenda, int32_t quantidadeExecucao, int32_t precoExecucao, int32_t *resultado)
{
    {
        int32_t idxCompra;
        int32_t idxVenda;
        int32_t comprador;
        int32_t vendedor;
        int32_t ativoCompra;
        int32_t ativoVenda;
        int32_t idxComprador;
        int32_t idxVendedor;
        Bolsa_Contexto__STATUS_ORDEM statusCompra;
        Bolsa_Contexto__STATUS_ORDEM statusVenda;
        Bolsa_Contexto__TIPO_LADO ladoCompra;
        Bolsa_Contexto__TIPO_LADO ladoVenda;
        int32_t precoCompra;
        int32_t precoVenda;
        int32_t qtdCompra;
        int32_t qtdVenda;
        int32_t saldoBloqueadoComprador;
        int32_t saldoDispComprador;
        int32_t saldoDispVendedor;
        int32_t idxCustodiaComprador;
        int32_t idxCustodiaVendedor;
        int32_t custBloqVendedor;
        int32_t custDispComprador;
        int32_t valorBloqueadoCompra;
        int32_t valorExecutado;
        int32_t valorDevolvido;
        int32_t novoSaldoBloqueadoComprador;
        int32_t novoSaldoDisponivelComprador;
        int32_t novoSaldoDisponivelVendedor;
        int32_t novaCustodiaBloqueadaVendedor;
        int32_t novaCustodiaDisponivelComprador;
        int32_t novaQuantidadeCompra;
        int32_t novaQuantidadeVenda;
        
        idxCompra = ordemCompra-1;
        idxVenda = ordemVenda-1;
        comprador = Bolsa_Valores_Abstract__donoOrdemI[idxCompra];
        vendedor = Bolsa_Valores_Abstract__donoOrdemI[idxVenda];
        ativoCompra = Bolsa_Valores_Abstract__ativoOrdemI[idxCompra];
        ativoVenda = Bolsa_Valores_Abstract__ativoOrdemI[idxVenda];
        idxComprador = comprador-1;
        idxVendedor = vendedor-1;
        statusCompra = Bolsa_Valores_Abstract__statusOrdemI[idxCompra];
        statusVenda = Bolsa_Valores_Abstract__statusOrdemI[idxVenda];
        ladoCompra = Bolsa_Valores_Abstract__tipoLadoOrdemI[idxCompra];
        ladoVenda = Bolsa_Valores_Abstract__tipoLadoOrdemI[idxVenda];
        precoCompra = Bolsa_Valores_Abstract__precoLimiteOrdemI[idxCompra];
        precoVenda = Bolsa_Valores_Abstract__precoLimiteOrdemI[idxVenda];
        qtdCompra = Bolsa_Valores_Abstract__quantidadeOrdemI[idxCompra];
        qtdVenda = Bolsa_Valores_Abstract__quantidadeOrdemI[idxVenda];
        idxCustodiaComprador = (idxComprador * 10)+(ativoCompra-1);
        idxCustodiaVendedor = (idxVendedor * 10)+(ativoCompra-1);
        saldoBloqueadoComprador = Bolsa_Valores_Abstract__saldoBloqueadoI[idxComprador];
        saldoDispComprador = Bolsa_Valores_Abstract__saldoDisponivelI[idxComprador];
        saldoDispVendedor = Bolsa_Valores_Abstract__saldoDisponivelI[idxVendedor];
        custBloqVendedor = Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodiaVendedor];
        custDispComprador = Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodiaComprador];
        valorBloqueadoCompra = quantidadeExecucao * precoCompra;
        valorExecutado = quantidadeExecucao * precoExecucao;
        valorDevolvido = quantidadeExecucao * (precoCompra-precoExecucao);
        novoSaldoDisponivelComprador = saldoDispComprador+valorDevolvido;
        novoSaldoDisponivelVendedor = saldoDispVendedor+valorExecutado;
        novaCustodiaDisponivelComprador = custDispComprador+quantidadeExecucao;
        if((((((((((((((((((idxCompra) != (idxVenda)) &&
                                                                    (statusCompra == Bolsa_Contexto__status_aberta)) &&
                                                                (statusVenda == Bolsa_Contexto__status_aberta)) &&
                                                            (ladoCompra == Bolsa_Contexto__lado_compra)) &&
                                                        (ladoVenda == Bolsa_Contexto__lado_venda)) &&
                                                    ((comprador) != (vendedor))) &&
                                                (ativoCompra == ativoVenda)) &&
                                            ((precoCompra) >= (precoVenda))) &&
                                        ((precoExecucao) >= (precoVenda))) &&
                                    ((precoExecucao) <= (precoCompra))) &&
                                ((quantidadeExecucao) <= (qtdCompra))) &&
                            ((quantidadeExecucao) <= (qtdVenda))) &&
                        ((saldoBloqueadoComprador) >= (valorBloqueadoCompra))) &&
                    ((custBloqVendedor) >= (quantidadeExecucao))) &&
                ((novoSaldoDisponivelComprador) <= (1000000))) &&
            ((novoSaldoDisponivelVendedor) <= (1000000))) &&
        ((novaCustodiaDisponivelComprador) <= (1000000)))
        {
            novoSaldoBloqueadoComprador = saldoBloqueadoComprador-valorBloqueadoCompra;
            novaCustodiaBloqueadaVendedor = custBloqVendedor-quantidadeExecucao;
            novaQuantidadeCompra = qtdCompra-quantidadeExecucao;
            novaQuantidadeVenda = qtdVenda-quantidadeExecucao;
            Bolsa_Valores_Abstract__saldoBloqueadoI[idxComprador] = novoSaldoBloqueadoComprador;
            Bolsa_Valores_Abstract__saldoDisponivelI[idxComprador] = novoSaldoDisponivelComprador;
            Bolsa_Valores_Abstract__saldoDisponivelI[idxVendedor] = novoSaldoDisponivelVendedor;
            Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodiaVendedor] = novaCustodiaBloqueadaVendedor;
            Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodiaComprador] = novaCustodiaDisponivelComprador;
            Bolsa_Valores_Abstract__quantidadeOrdemI[idxCompra] = novaQuantidadeCompra;
            Bolsa_Valores_Abstract__quantidadeOrdemI[idxVenda] = novaQuantidadeVenda;
            if(novaQuantidadeCompra == 0)
            {
                Bolsa_Valores_Abstract__statusOrdemI[idxCompra] = Bolsa_Contexto__status_executada;
            }
            else
            {
                Bolsa_Valores_Abstract__statusOrdemI[idxCompra] = Bolsa_Contexto__status_aberta;
            }
            if(novaQuantidadeVenda == 0)
            {
                Bolsa_Valores_Abstract__statusOrdemI[idxVenda] = Bolsa_Contexto__status_executada;
            }
            else
            {
                Bolsa_Valores_Abstract__statusOrdemI[idxVenda] = Bolsa_Contexto__status_aberta;
            }
            (*resultado) = Bolsa_Contexto__OK;
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_CASAMENTO_INVALIDO;
        }
    }
}

void Bolsa_Valores_Abstract__cancelarOrdem(int32_t ordemId, int32_t *resultado)
{
    {
        int32_t idxOrdem;
        int32_t clienteId;
        int32_t ativoId;
        int32_t idxCliente;
        Bolsa_Contexto__TIPO_LADO tipoOrdem;
        Bolsa_Contexto__STATUS_ORDEM statusAtual;
        int32_t quantidadeAtual;
        int32_t precoAtual;
        int32_t idxCustodia;
        int32_t valorDevolvido;
        int32_t saldoAtual;
        int32_t saldoBloqueadoAtual;
        int32_t novoSaldoDisponivel;
        int32_t novoSaldoBloqueado;
        int32_t custodiaAtual;
        int32_t custodiaBloqueadaAtual;
        int32_t novaCustodiaDisponivel;
        int32_t novaCustodiaBloqueada;
        
        idxOrdem = ordemId-1;
        clienteId = Bolsa_Valores_Abstract__donoOrdemI[idxOrdem];
        idxCliente = clienteId-1;
        ativoId = Bolsa_Valores_Abstract__ativoOrdemI[idxOrdem];
        tipoOrdem = Bolsa_Valores_Abstract__tipoLadoOrdemI[idxOrdem];
        statusAtual = Bolsa_Valores_Abstract__statusOrdemI[idxOrdem];
        quantidadeAtual = Bolsa_Valores_Abstract__quantidadeOrdemI[idxOrdem];
        precoAtual = Bolsa_Valores_Abstract__precoLimiteOrdemI[idxOrdem];
        idxCustodia = (idxCliente * 10)+(ativoId-1);
        valorDevolvido = quantidadeAtual * precoAtual;
        saldoAtual = Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente];
        saldoBloqueadoAtual = Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente];
        novoSaldoDisponivel = saldoAtual+valorDevolvido;
        custodiaAtual = Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia];
        custodiaBloqueadaAtual = Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodia];
        novaCustodiaDisponivel = custodiaAtual+quantidadeAtual;
        if(statusAtual == Bolsa_Contexto__status_aberta)
        {
            if(tipoOrdem == Bolsa_Contexto__lado_compra)
            {
                if(((saldoBloqueadoAtual) >= (valorDevolvido)) &&
                ((novoSaldoDisponivel) <= (1000000)))
                {
                    novoSaldoBloqueado = saldoBloqueadoAtual-valorDevolvido;
                    Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente] = novoSaldoDisponivel;
                    Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente] = novoSaldoBloqueado;
                    Bolsa_Valores_Abstract__quantidadeOrdemI[idxOrdem] = 0;
                    Bolsa_Valores_Abstract__statusOrdemI[idxOrdem] = Bolsa_Contexto__status_cancelada;
                    (*resultado) = Bolsa_Contexto__OK;
                }
                else
                {
                    (*resultado) = Bolsa_Contexto__ERRO_LIMITE_EXCEDIDO;
                }
            }
            else
            {
                if(((custodiaBloqueadaAtual) >= (quantidadeAtual)) &&
                ((novaCustodiaDisponivel) <= (1000000)))
                {
                    novaCustodiaBloqueada = custodiaBloqueadaAtual-quantidadeAtual;
                    Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia] = novaCustodiaDisponivel;
                    Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodia] = novaCustodiaBloqueada;
                    Bolsa_Valores_Abstract__quantidadeOrdemI[idxOrdem] = 0;
                    Bolsa_Valores_Abstract__statusOrdemI[idxOrdem] = Bolsa_Contexto__status_cancelada;
                    (*resultado) = Bolsa_Contexto__OK;
                }
                else
                {
                    (*resultado) = Bolsa_Contexto__ERRO_LIMITE_EXCEDIDO;
                }
            }
        }
        else
        {
            (*resultado) = Bolsa_Contexto__ERRO_ORDEM_NAO_ABERTA;
        }
    }
}

void Bolsa_Valores_Abstract__consultarSaldo(int32_t clienteId, int32_t *valorDisponivel, int32_t *valorBloqueado)
{
    {
        int32_t idxCliente;
        
        idxCliente = clienteId-1;
        (*valorDisponivel) = Bolsa_Valores_Abstract__saldoDisponivelI[idxCliente];
        (*valorBloqueado) = Bolsa_Valores_Abstract__saldoBloqueadoI[idxCliente];
    }
}

void Bolsa_Valores_Abstract__consultarCustodia(int32_t clienteId, int32_t ativoId, int32_t *quantidadeDisponivel, int32_t *quantidadeBloqueada)
{
    {
        int32_t idxCliente;
        int32_t idxAtivo;
        int32_t idxCustodia;
        
        idxCliente = clienteId-1;
        idxAtivo = ativoId-1;
        idxCustodia = (idxCliente * 10)+idxAtivo;
        (*quantidadeDisponivel) = Bolsa_Valores_Abstract__custodiaDisponivelI[idxCustodia];
        (*quantidadeBloqueada) = Bolsa_Valores_Abstract__custodiaBloqueadaI[idxCustodia];
    }
}

void Bolsa_Valores_Abstract__consultarStatusOrdem(int32_t ordemId, int32_t *statusCodigo)
{
    {
        int32_t idxOrdem;
        Bolsa_Contexto__STATUS_ORDEM statusAtual;
        
        idxOrdem = ordemId-1;
        statusAtual = Bolsa_Valores_Abstract__statusOrdemI[idxOrdem];
        if(statusAtual == Bolsa_Contexto__status_livre)
        {
            (*statusCodigo) = 0;
        }
        else
        {
            if(statusAtual == Bolsa_Contexto__status_aberta)
            {
                (*statusCodigo) = 1;
            }
            else
            {
                if(statusAtual == Bolsa_Contexto__status_cancelada)
                {
                    (*statusCodigo) = 2;
                }
                else
                {
                    (*statusCodigo) = 3;
                }
            }
        }
    }
}


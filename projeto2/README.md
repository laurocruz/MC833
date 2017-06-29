## Lauro Cruz e Souza - 156175
## Pedro Emílio Machado de Brito - 137264

# Projeto 2

# Arquivos

### server.c

O "backend" do servidor foi implementado usando uma thread para cada tipo de
conexão (TCP ou UDP) e tipo de dados (segurança ou entretenimento/conforto),
para um total de 4 threads.

Para as threads lidando com TCP, há um `select()` para multiplexar entre as
conexões abertas.

### demo_server.c

Esse arquivo implementa a lógica de detecção de colisões. Também implementa os
atrasos vazios para simular carga no canal de entretenimento e conforto.

Optamos por usar uma heurística simples e pessimista para alertar sobre colisões
iminentes, o que simplifica a implementação, apesar de causar frenagens mais do
que o necessário.

A detecção da colisão em si foi implementada de maneira precisa.

### client.c

Nesse arquivo fica o "backend" do cliente, onde fazemos a comunicação com o
servidor, e se trata em sua maior parte de adaptações de códidgo de ativivdades
anteriores.

### carro.c

Nesse arquivo fica a lógica para movimentar o carro, a partir do estado atual e
das mensagens enviadas pelo servidor, e também as esperas da comunicação de
entretenimento e conforto.

### demo_carro.c

Nesse arquivo fica o código de inicialização do carro e inicialização de gerador
de números aleatórios.


## Rodando

Compile usando `make`. Rode o servidor com, por exemplo:

```
./demo_server 2000 2000 2001 2001
```

Em outro terminal, use o script:

```
./run-sim simconfig1.in simdata1.in
```

Os resultados para cada instância do carro serão coletados em `results.out`,
indicando o número de mensagens recebidas, também como um breakdown por tipo de
mensagem.

O servidor mostra atualizações na saída padrão, para salvá-la, basta
redirecionar para um arquivo.


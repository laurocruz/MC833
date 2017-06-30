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


## Sistema anti-colisão

Optamos por usar uma heurística simples e pessimista para alertar sobre colisões
iminentes, calculando intervalos de tempo fracionários em que cada carro vai
estar no cruzamento, e vendo se há intersecção, o que simplifica a
implementação, apesar de causar frenagens mais do que o necessário.

A detecção da colisão em si foi implementada de maneira precisa.


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

# Formato:

O arquivo do tipo `simconfig` tem uma linha, com, respectivamente:

* hostname do servidor
* porta do servidor para pacotes de segurança
* porta do servidor para pacotes de conforto e entretenimento
* 1 para usar TCP na segurança, 0 pra UDP
* 1 para usar TCP no conforto, 0 pra UDP
* 1 para usar TCP no entretenimento, 0 pra UDP

O arquivo do tipo `simdata` tem várias linhas, cada linha será usada para lançar
um cliente, com o seguinte formato:

* Velocidade inicial do carro (em direção ao cruzamento)
* Direção do carro: 0 = para cima, 1 = para direita, 2 = para baixo, 3 = para esquerda
* Comprimento do carro
* Aceleração a ser aplicada quando o servidor pedir para aumentar a velocidade
* Aceleração a ser aplicada quando o servidor pedir para abaixar a velocidade
* Velocidade máxima do carro
* Distância inicial do carro em relação a origem.

O arquivo `results.out` tem informações sobre o número de mensagens enviadas, e
separando por cada tipo de mensagem, inclusive, mostrando se o carro acabou
colidindo, e tendo que ser chamada a ambulância. Além disso, tem informações
sobre a latência média dos pedidos.

Exemplo de uma saída com `simconfig1.in` e `simdata1.in`:

```
avg. TCP latency = 10
messages 10 empty 9 brake 1 throttle 0 ambulance 0
avg. TCP latency = 11
messages 10 empty 9 brake 1 throttle 0 ambulance 0
avg. TCP latency = 14
messages 10 empty 9 brake 1 throttle 0 ambulance 0
avg. TCP latency = 10
messages 11 empty 10 brake 1 throttle 0 ambulance 0
avg. TCP latency = 12
messages 10 empty 9 brake 1 throttle 0 ambulance 0
avg. TCP latency = 129
messages 9 empty 8 brake 1 throttle 0 ambulance 0
avg. TCP latency = 139
messages 9 empty 7 brake 1 throttle 1 ambulance 0
avg. TCP latency = 26
messages 10 empty 10 brake 0 throttle 0 ambulance 0
avg. TCP latency = 146
messages 9 empty 7 brake 1 throttle 1 ambulance 0
avg. TCP latency = 104
messages 11 empty 10 brake 1 throttle 0 ambulance 0
avg. TCP latency = 18
messages 13 empty 11 brake 2 throttle 0 ambulance 0
avg. TCP latency = 20
messages 12 empty 10 brake 2 throttle 0 ambulance 0
avg. TCP latency = 16
messages 13 empty 11 brake 2 throttle 0 ambulance 0
avg. TCP latency = 21
messages 13 empty 11 brake 2 throttle 0 ambulance 0
avg. TCP latency = 25
messages 13 empty 12 brake 1 throttle 0 ambulance 0
avg. TCP latency = 114
messages 12 empty 10 brake 2 throttle 0 ambulance 0
avg. TCP latency = 16
messages 15 empty 11 brake 2 throttle 2 ambulance 0
avg. TCP latency = 95
messages 13 empty 12 brake 1 throttle 0 ambulance 0
avg. TCP latency = 12
messages 16 empty 9 brake 3 throttle 4 ambulance 0
avg. TCP latency = 18
messages 17 empty 10 brake 5 throttle 2 ambulance 0
avg. TCP latency = 21
messages 16 empty 11 brake 3 throttle 2 ambulance 0
avg. TCP latency = 13
messages 18 empty 15 brake 3 throttle 0 ambulance 0
avg. TCP latency = 87
messages 16 empty 10 brake 4 throttle 2 ambulance 0
avg. TCP latency = 17
messages 19 empty 15 brake 4 throttle 0 ambulance 0
avg. TCP latency = 17
messages 19 empty 13 brake 5 throttle 1 ambulance 0
avg. TCP latency = 12
messages 82 empty 0 brake 81 throttle 0 ambulance 1
avg. TCP latency = 12
messages 82 empty 0 brake 81 throttle 0 ambulance 1
avg. TCP latency = 105236
messages 1 empty 1 brake 0 throttle 0 ambulance 0
avg. TCP latency = 105247
messages 1 empty 0 brake 1 throttle 0 ambulance 0
```

Alguns clientes apresentam latências médias enormes, o que provavelmente se deve
a alguma contenção no processamento devido ao uso de atrasos para simular
processamento.


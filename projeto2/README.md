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

## Sistema anti-colisão

Optamos por usar uma heurística simples e pessimista para alertar sobre colisões
iminentes, calculando intervalos de tempo fracinários em que cada carro vai
estar no cruzamento, e vendo se há intersecção. o que simplifica a implementação, apesar
de causar frenagens mais do que o necessário.

A detecção da colisão em si foi implementada de maneira precisa.


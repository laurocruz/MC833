# Atividade 1


## (a)

(a) Envie 10 pacotes que verificam o tempo de ida e volta a 5 endereços web de sua escolha (10 pacotes para cada endereço). Inclua no relatório os comandos utilizados e as saídas obtidas, além de uma análise sobre as informações observadas.

### `$ ping www.ic.unicamp.br -c 10`

```
PING atibaia.ic.unicamp.br (143.106.7.54) 56(84) bytes of data.
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=1 ttl=55 time=78.4 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=2 ttl=55 time=74.3 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=3 ttl=55 time=29.2 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=4 ttl=55 time=264 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=5 ttl=55 time=27.8 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=6 ttl=55 time=29.7 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=7 ttl=55 time=60.1 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=8 ttl=55 time=20.4 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=9 ttl=55 time=16.1 ms
64 bytes from atibaia.ic.unicamp.br (143.106.7.54): icmp_seq=10 ttl=55 time=25.8 ms

--- atibaia.ic.unicamp.br ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9013ms
rtt min/avg/max/mdev = 16.171/62.691/264.532/70.604 ms
```

### `$ ping www.usp.br -c 10`

```
PING rubus.uspnet.usp.br (200.144.248.41) 56(84) bytes of data.
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=1 ttl=248 time=17.2 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=2 ttl=248 time=82.2 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=3 ttl=248 time=60.8 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=4 ttl=248 time=10.5 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=5 ttl=248 time=51.6 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=6 ttl=248 time=41.3 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=7 ttl=248 time=48.1 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=8 ttl=248 time=53.3 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=9 ttl=248 time=30.3 ms
64 bytes from webhost.uspdigital.usp.br (200.144.248.41): icmp_seq=10 ttl=248 time=13.2 ms

--- rubus.uspnet.usp.br ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9012ms
rtt min/avg/max/mdev = 10.501/40.885/82.204/21.876 ms
```

### `$ ping www.unesp.br -c 10`

```
PING yoda.unesp.br (200.145.6.90) 56(84) bytes of data.
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=1 ttl=58 time=45.7 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=2 ttl=58 time=102 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=3 ttl=58 time=43.1 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=4 ttl=58 time=31.0 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=5 ttl=58 time=61.7 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=6 ttl=58 time=58.5 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=7 ttl=58 time=93.2 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=8 ttl=58 time=159 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=9 ttl=58 time=47.9 ms
64 bytes from yoda.unesp.br (200.145.6.90): icmp_seq=10 ttl=58 time=88.0 ms

--- yoda.unesp.br ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9010ms
rtt min/avg/max/mdev = 31.070/73.224/159.952/36.670 ms
```

### `$ ping www.cs.man.ac.uk -c 10`

```
PING waldorf.cs.man.ac.uk (130.88.199.17) 56(84) bytes of data.
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=1 ttl=46 time=263 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=2 ttl=46 time=409 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=3 ttl=46 time=303 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=4 ttl=46 time=346 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=5 ttl=46 time=241 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=6 ttl=46 time=427 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=7 ttl=46 time=244 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=8 ttl=46 time=234 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=9 ttl=46 time=272 ms
64 bytes from waldorf.cs.man.ac.uk (130.88.199.17): icmp_seq=10 ttl=46 time=238 ms

--- waldorf.cs.man.ac.uk ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9113ms
rtt min/avg/max/mdev = 234.999/298.399/427.285/68.451 ms
```

### `$ ping www.u-tokyo.ac.jp -c 10`

```
PING www.u-tokyo.ac.jp (210.152.135.178) 56(84) bytes of data.
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=1 ttl=42 time=313 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=2 ttl=42 time=354 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=3 ttl=42 time=357 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=4 ttl=42 time=337 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=5 ttl=42 time=314 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=6 ttl=42 time=417 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=7 ttl=42 time=1094 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=8 ttl=42 time=354 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=9 ttl=42 time=407 ms
64 bytes from 210.152.135.178 (210.152.135.178): icmp_seq=10 ttl=42 time=318 ms

--- www.u-tokyo.ac.jp ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9575ms
rtt min/avg/max/mdev = 313.578/426.984/1094.769/225.190 ms, pipe 2
```

A principal informação que podemos tirar da saída do `$ ping` são os valores de RTT (Round-Trip Time) para os diferentes servidores. RTT é o tempo necessário para um pacote sair do host (nosso computador), ir ao servidor e retornar. Sendo assim, para servidores mais próximos do host o RTT deve ser menor, e para servidores mais distantes deve ser maior.

Isso pode ser visto facilmente nos resultados acima, pois a lista de pings é listada em ordem crescente de distância do host para o servidor, e analisando o RTT médio, é possível ver que os RTT's aumentam (62.691)

(b) Verifique se o host www.lrc.ic.unicamp.br é alcançável utilizando a ferramenta ping. Depois, acesse a URL no seu browser. Explique.


(c) Mostre passo a passo e explique: Descubra o endereço IP da sua máquina utilizando alguma ferramenta vista em sala. Identifique qual interface de rede está sendo utilizada para transmitir dados entre o seu computador e a Internet. Identifique a interface de "loopback" e utilize uma das ferramentas para alterar o número de pacotes enviados e recebidos.


(d) Identifique a rota padrão na sua máquina.


(e) Qual é o servidor DNS configurado na sua estação? Obtenha os endereços IP de www.ic.unicamp.br, www.unicamp.br, www.usp.br, www.unesp.br, www.cs.man.ac.uk, www.u-tokyo.ac.jp.


(f) Quais e quantos são os roteadores identificáveis nos caminhos para os endereços do item (e)? Identifique enlaces de longa distância para o endereço fora do Brasil e explique como fez para identificá-los.


(g) Identifique 5 conexões TCP do seu computador, bem como as portas de origem e destino. Acesse o site www.unicamp.br e repita o comando que utilizou para identificar as conexões. Apresente as informações que encontrou sobre a conexão com www.unicamp.br.


(h) Utilize telnet para receber o arquivo html da página http://www.ic.unicamp.br/~reltech/ e apresente no relatório os passos utilizados, explicando-os. Tente, de forma similar, realizar uma conexão telnet com localhost. O que ocorre? Por quê?

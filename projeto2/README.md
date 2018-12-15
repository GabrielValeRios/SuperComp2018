# WebCrawler
Este documento tem por objetivo auxiliar o uso do algoritmo de **crawling**, com exemplos que foram utilizados nos testes iniciais. O algoritmo irá funcionar para o site da varejista _MagazineLuiza_, onde as informações dos produtos de uma certa categoria serão baixada e processadas, sequencialmente ou paralelamente.
## Pré-requisitos
Para realizar os testes, é necessário possuir:
### Obrigatório
- g++
- python3 ou versões acima
- cpr, para baixar o html de uma página
- boost, para regex
### Não obrigatório para o programa funcionar, mas sim para testes de uso de memória
- [mprof](https://pypi.org/project/memory_profiler/)

### Instalações, como root
- Para instalar g++:
  - `apt install g++`
- Para instalar python:
  - `apt install python3`
- Para instalar cpr, apenas siga as orientações clicando [aqui](https://github.com/whoshuu/cpr)
- Para instalar boost:
  - `apt install libboost-all-dev`
  
## Testes
Para relizar os testes, clone este repositório usando o comando do git: 
- `git clone https://github.com/GabrielValeRios/superComp2018/`

Abra um terminal e navegue até a pasta **projeto2**, pois os comandos para iniciar a aplicação serão realizados em modo texto
Com o terminal aberto no diretório indicado acima, execute o primeiro comando:
- `python3 build_files.py`, para gerar 2 executáveis através de [CMake](https://cmake.org/) e [Make](https://pt.wikipedia.org/wiki/Makefile)

A partir deste comando, dois executáveis foram gerados: **crawler_par** e **crawler_seq**. Abaixo, será explicado como executar cada um deles, de modo individual, usando python3

### Sequencial
No terminal, digite:
- `python3 sequentialRun.py`

O executável irá pedir um _link_ da categoria do produto. Entre aspas duplas (" "), forneça o link teste abaixo...

- "https://www.magazineluiza.com.br/bateria-e-percussao/instrumentos-musicais/s/im/baoa/" , aspas duplas devem estar inclusas!

... e aperte a tecla enter.

**_IMPORTANTE_** : O link acima é um exemplo, outros links de outras categorias de produto podem ser fornecidos. Para saber como chegar a um link válido para usar na simulação, leia o relatório em PDF que se encontra em :
- ${path do diretório onde o clone do repositório foi feito}/superComp2018/projeto2/analises/relatorio/webCrawler-Analises.pdf

### Paralelo
No terminal, digite:
- `python3 parallelRun.py`

O executável irá pedir o número de _threads_ para iniciar a simulação. O número de threads mínimo é 2, caso seja dado um número menor o programa irá travar ou emitir um erro de sistema. Na seção **_Melhorias e Bugs_**, tais error e limitações serão melhor comentados.

Além disso, analogamente ao sequencial, a execução também irá requisitar um link, devendo ser o mesmo já citado para comparação de resultados:

- "https://www.magazineluiza.com.br/bateria-e-percussao/instrumentos-musicais/s/im/baoa/" , aspas duplas devem estar inclusas!

Após forecer as informações necessárias, basta apertar o botão enter.

## Analisando os Resultados
Ao terminar os passos anteriores, durante a execução, serão mostrados em formato [JSON](https://www.json.org/) as informações dos produtos da categoria do link fornecido. Cada produto será mostrado da seguinte forma, no terminal:
- `{

      "nome": nome do produto,

      "descricao": descrição do produto,

      "foto": url da foto do produto,

      "preco": preço a vista do produto,

      "preco_parcelado": preço parcelado do produto,

      "categoria": categoria a qual o produto pertence,

      "url": url de acesso ao produto

  }`
Ao fim da execução, o executável _sequencial_ irá gerar 3 arquivos textos, no mesmo diretório do executável:

- sequentialTimes.txt, tempo de cada produto na simulação
- WaitTimeSequential.txt, tempo ocioso total na simulação
- averageTimePerProductSequential.txt, tempo total médio por produto da simulação

Caso o executável tenha sido o _paralelo_, ele também irá gerar 3 arquivos textos:

- parallelTimes.txt, tempo de cada produto na simulação
- WaitTimeParallel.txt, tempo ocioso total na simulação
- averageTimePerProductParallel.txt, tempo total médio por produto da simulação

**_IMPORTANTE_** : Na versão __paralela__, para diferentes tempos com um número variado de threads, é necessário executar o código múltiplas vezes, mudando a variável relacionada ao número de threads quando se executa o algoritmo. Assim, a cada simulação, deve-se anotar o tempo dado pelos arquivos, pois na próxima simulação eles serão apagados.

Com os tempos anotados, podem ser feitos gráficos de análises comparativas entre os tempos gerados. Abaixo seguem exemplos de comparação:

![Tempo Ocioso](https://github.com/GabrielValeRios/superComp2018/blob/master/projeto2/analises/img/WaitTime.JPG)

![Tempo médio por produto](https://github.com/GabrielValeRios/superComp2018/blob/master/projeto2/analises/img/Average%20time%20per%20product.JPG)

## Usando Mprof

Para utilizar o mprof, para a versão sequencial ou paralela, basta executar o seguinte comando:

- `mprof run sequentialRun.py`

ou

- `mprof run parallelRun.py`

Um novo arquivo será criado no diretório do executável, com os resultados do uso de memória. Para visualiza-los, basta executar o comando:

- `mprof plot`

E como resultado, uma nova aba irá se abrir, como o exemplo abaixo demonstra:

![Exemplo Mprof](https://github.com/GabrielValeRios/superComp2018/blob/master/projeto2/analises/img/MprofSequential.png)

## Melhorias e bugs

O projeto possui algumas limitações. São elas:

- Não lidar com números inválidos de threads. Ex. NUM_THREADS < 1
- Não conseguir fazer o crawling de categorias com poucos produtos, podendo gerar um erro as vezes.
- Medições de memória imprecisas, há dúvida quanto as medições pela maneira como foi feita (rodar mprof nos arquivos sequentialRun.py e parallelRun.py é correto?) ou pelo número máximo de threads usado nos testes (será que poucas threads foram utilizadas?). Sobre esse último ponto, foi testado um crawling para uma mesma página com 2 números diferentes de threads: 10 e 100. Mesmo assim, não houve mudança no uso de memória, o que torna as medições feitas duvidosas, necessitando assim uma reavaliação.
- Alguns produtos estão indisponíveis, dependendo da categoria. Porém, o programa não diferencia para o usuário de modo claro esta informação, jogando um JSON vazio no terminal ou incompleto.

Os bugs serão revistos e conforme forem sendo solucionados, sua _box_ abaixo será marcada com um 'X'.

- [ ] Número invalido de threads 
- [ ] Crawling de categorias com poucas páginas
- [ ] Uso de memória dos algoritmos
- [ ] JSON incompleto no terminal devido a produtos indisponíveis

## Autor

- Gabriel do vale rios - Estudante de Engenharia de Computação do [Insper](https://www.insper.edu.br/)

## Licença

Este projeto é licenciado de acordo com GNU General Public License(GPL) - mais detalhes clique [aqui](https://www.gnu.org/licenses/gpl.html)

## Considerações finais

Cumprimentos ao professor [Igor](https://github.com/igordsm?tab=repositories)

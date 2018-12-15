# WebCrawler
Este documento tem por objetivo auxiliar o uso do algoritmo de **crawling**, com exemplos que foram utilizados nos testes iniciais. 
O algoritmo irá funcionar para o site da varejista _MagazineLuiza_, onde as informações dos produtos de uma certa categoria serão baixadas e processadas em um cluster.
## Pré-requisitos
Para realizar os testes, é necessário possuir:
### Obrigatório
- g++
- python3 ou versões acima
- cpr, para baixar o html de uma página
- boost, para regex e MPI

### Instalações, como root
- Para instalar g++:
  - `apt install g++`
- Para instalar python:
  - `apt install python3`
- Para instalar cpr, apenas siga as orientações clicando [aqui](https://github.com/whoshuu/cpr)
- Para instalar boost:
  - `apt install libboost-all-dev`
  - Para instalar MPI:
  - `apt install libboost-mpi-dev`
  
## Testes
Para relizar os testes, clone este repositório usando o comando do git: 
- `git clone https://github.com/GabrielValeRios/superComp2018/`

Abra um terminal e navegue até a pasta **projeto3**, pois os comandos para iniciar a aplicação serão realizados em modo texto
Com o terminal aberto no diretório indicado acima,o comando:
- `python3 build_files.py`, para gerar um executável através de [CMake](https://cmake.org/) e [Make](https://pt.wikipedia.org/wiki/Makefile)

pode ser executado. Não é necessário que se execute este comando, pois o binário do executável já está no repositório, para ambiente Linux. Caso não exista mais o executável por algum motivo, ai sim o comando acima deve ser executado.

### Rodando o código

Para rodar o código, é necessário fornecer um hostfile. Um hostfile é um arquivo onde estão os ip's das máquinas a serem utilizadas no cluster.
Caso esteja testando o código localmente, basta fornecer o arquivo "hostfile1", que se encontra na mesma pasta do executável.
Se estiver testando em um cluster, basta fornecer os IP's das máquinas em um arquivo chamado "hostfiles<numero de maquinas>".
Os arquivos no repositório não irão funcionar pois possuem os IP's das máquinas de um cluster privado. Para mais detalhes, cheque **Rodando em um Cluster**.

No terminal, digite:
- `python3 runMPI.py -f <hostfile desejado>`

A execução irá requisitar um link de categoria de produto, que como exemplo foi usado o link abaixo:

- https://www.magazineluiza.com.br/bateria-e-percussao/instrumentos-musicais/s/im/baoa/

Para saber como conseguir um link, basta acessar o relatório que se encontra na pasta /resultados/relatorio

Após forecer as informações necessárias, basta apertar o botão enter.

## Analisando os Resultados
Ao terminar os passos anteriores, após a execução, serão mostrados em formato [JSON](https://www.json.org/) as informações dos produtos da categoria do link fornecido. Cada produto será mostrado da seguinte forma, no terminal:
- `{

      "nome": nome do produto,

      "descricao": descrição do produto,

      "foto": url da foto do produto,

      "preco": preço a vista do produto,

      "preco_parcelado": preço parcelado do produto,

      "categoria": categoria a qual o produto pertence,

      "url": url de acesso ao produto

  }`
Ao fim da execução, o executável  irá gerar 1 arquivo texto, no mesmo diretório do executável:

- testes.txt, que possui o tempo de cada produto, o tempo Total da execução e o tempo total Ocioso do algoritmo.

Com os tempos anotados, foi realizado um gráfico dentre alguns possíveis para analise, que foi a comparação de desempenho de acordo com a quantidade de máquinas utilizada:

![Desempenho](https://github.com/GabrielValeRios/superComp2018/blob/master/projeto3/resultados/img/totalTime.JPG)

## Rodando em um Cluster

Caso possua um Cluster, é necessário que:
- Todas as máquinas possuam acesso SSH entre si. Para isso, gere chaves públicas e privadas com o comando _ssh-keygen_. Guarde a chave pública gerada para está máquina nas outras máquinas do Cluster, no arquivo _~/.ssh/authorized_keys_, e repita isso para todas as outras máquinas. Ou ainda, para não repetir este mesmo passo diversas vezes, faça para uma máquina e replique ela N vezes, sendo N a quantidade de máquinas que gostaria que seu CLuster tivesse.

- Para configurar o cluster, pode-se seguir este [tutorial](http://mpitutorial.com/tutorials/running-an-mpi-cluster-within-a-lan/), por Dwaraka Nath.

Assim, para rodar o código, basta estar logado na máquina _master_ e escolher um dos arquivos _hostfiles<numero>_ para rodar:
  
`python3 runMPI.py -f hostfiles<numero>`

- **PS:** Lembre-se de clonar o repositório e executar o comando acima na pasta compartilhada criada no tutorial acima.

## Melhorias e bugs

O projeto possui algumas limitações. São elas:

- Não conseguir fazer o crawling de categorias com poucos produtos, podendo gerar um erro as vezes.
- Alguns produtos estão indisponíveis, dependendo da categoria. Porém, o programa não diferencia para o usuário de modo claro esta informação, jogando um JSON vazio no terminal ou incompleto.

Os bugs serão revistos e conforme forem sendo solucionados, sua _box_ abaixo será marcada com um 'X'.

- [ ] Crawling de categorias com poucas páginas
- [ ] JSON incompleto no terminal devido a produtos indisponíveis

## Autor

- Gabriel do vale rios - Estudante de Engenharia de Computação do [Insper](https://www.insper.edu.br/)

## Licença

Este projeto é licenciado de acordo com GNU General Public License(GPL) - mais detalhes clique [aqui](https://www.gnu.org/licenses/gpl.html)

## Considerações finais

Cumprimentos ao professor [Igor](https://github.com/igordsm?tab=repositories), por fornecer o material necessário e ajuda para executar o projeto.

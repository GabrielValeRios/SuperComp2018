## Pré-requisitos
Para realizar os testes, é necessário possuir:
- g++
- python3 ou versões acima
- cuda toolkit
- placa de vídeo dedicada Nvidia GeForce. 

### Instalações, como root
- Para instalar g++:
  - `apt install g++`
- Para instalar python:
  - `apt install python3`
  - Para instalar Cuda toolkit:
  - Executar código `installCuda.py`
  
## Testes
Para relizar os testes, clone este repositório usando o comando do git: 
- `git clone https://github.com/GabrielValeRios/superComp2018/`

Abra um terminal e navegue até a pasta **projeto4**, pois os comandos para iniciar a aplicação serão realizados em modo texto
Com o terminal aberto no diretório indicado acima,o comando:
- `python3 installCuda.py`, para baixar as dependências do Cuda

### Rodando o código

### Sequencial

No terminal, digite:
- `python3 runSEQ_SEQ.py`

A execução irá requisitar uma imagem .pmg. Forneça a imagem entre aspas duplas " imagem.pgm". Segue exemplo abaixo:

- "bolas.pgm"

Após forecer as informações necessárias, será pedido o número de seeds f_g (foreground) e seeds b_g (backgroud), e o número de cada uma delas. Segue exemplo abaixo:

- num_seeds_f_g num_seeds_b_g ( Ex.: 1 1)
- posX_f_g posY_f_g (Ex.: 50 50). Está posição se refere a um pixel na imagem.
- posX_b_g posY_b_g (Ex.: 30 30). Está posição se refere a um pixel na imagem.

Aperte ENTER e aguarde. Os resultados dos tempos serão gerados no terminal, e a imagem resultante irá ser criada no repositório.

### GPU

Para gerar o filtro de bordas (não obrigatório), digite no terminal:
- `python3 runBorderFile.py`

Será exigido uma imagem .pgm. Exemplo:
 - bolas.pgm
 
 A saída deste código é uma imagem com filtro de bordas, criada no próprio repositório, com o nome "saida_bf.pgm"
 
 Para gerar a imagem segmentada, digite:
- `python3 runGPU_SEG.py`

Analogamente ao sequencial, serão pedidos dados de seeds, assim como suas posições. A formatação é idêntica à mostrada na seção Sequencial.

Os resultados gerados serão os tempos e a imagem segmentada, de nome saida_sssp.pgm.

## Analisando os Resultados
Ao terminar os passos anteriores, os tempos para o processamento sequencial e em GPU serão gerados, e pode ser utilizados para comparações.

Exemplo de comparação podem ser vistos na pasta "img", em **projeto4**.

## Melhorias e bugs

O projeto possui algumas limitações. São elas:

- Necessitar que as imagens estejam em formato .pgm, as vezes não funcionando para algumas imagens com maior qualidade para o cálculo de SSSP e Filtro de Borda.

Os bugs serão revistos e conforme forem sendo solucionados, sua _box_ abaixo será marcada com um 'X'.

- [x] Qualidade alta de imagem consegue ser processada pelos algoritmos deste projeto

A correção deste item pode ser vista em ´border_filter.cu´

## Autor

- Gabriel do vale rios - Estudante de Engenharia de Computação do [Insper](https://www.insper.edu.br/)

## Licença

Este projeto é licenciado de acordo com GNU General Public License(GPL) - mais detalhes clique [aqui](https://www.gnu.org/licenses/gpl.html)

## Considerações finais

Cumprimentos ao professor [Igor](https://github.com/igordsm?tab=repositories), por fornecer o material necessário e ajuda para executar o projeto.

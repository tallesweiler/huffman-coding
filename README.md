# Huffman-Coding

## Introdução:

Huffman-Coding é um trabalho da disciplina de Estrutura de Dados I da Universidade Federal do Espírito Santo.
Todas as especificações do trabalho estão presentes no pdf, mas em resumo, seu objetivo é de construir o algoritmo de compactação de huffman, que consiste em compactar textos e outros arquivos em função da frequência de seus caracteres, através de TADs, listas encadeadas e árvores binárias.
Desenvolvido por: João Victor Morais e Talles Weiler

## Guia de utilização do programa:

O programa consiste em duas pastas principais:

- <b>include/</b> -> possui os headers de todas as bibliotecas implementadas
- <b>src/</b> -> possui os arquivos-fonte de todas as bibliotecas implementadas e o arquivo principal (main.c)

Toda a utilização do programa é feita através de um makefile, onde seus principais comandos são:

- <b>"make compacta file=x"</b> -> A string 'x' representa o caminho até o arquivo que será compactado (ex: "make compacta file=tests/bible.txt" efetua a compactação do arquivo tests/bible.txt). Este comando apaga os arquivos temporários anteriores e cria as pastas temporárias obj/ (onde ficam os arquivos-objeto) e bin/ (onde fica o arquivo executável). Após isso, compila o programa, roda e gera o arquivo compactado.
- <b>"make descompacta file=x"</b> -> A string 'x' representa o caminho até o arquivo que será descompactado (ex: "make descompacta file=tests/bible.comp" efetua a descompactação do arquivo tests/bible.comp). Este comando segue o mesmo padrão do comando anterior.
- <b>"make valgrind mode=x file=y"</b> -> A string 'x' representa o modo e 'y' a string até o arquivo que será compactado/descompactado (ex: make valgrind mode=-encoder file=tests/bible.txt). Este comando executa o programa em mode de compactação/descompactação utilizando o valgrind, para verificar possíveis vazamentos de memória.
- <b>"make clean"</b> -> exclui todos os arquivos temporários

## Funcionamento do programa:

Na construção do programa foram utilizados seis bibliotecas próprias:

- Os arquivos "huffman-encoder.h" e "huffman-decoder.h" são os únicos no qual o cliente tem acesso, são eles que recebem os arquivos que devem compactar/descompactar e utilizam as outras bibliotecas implementadas para conseguir isso.
- O TAD "dictionary.h" constrói uma lista de árvores binárias com todos os caracteres encontrados no arquivo de entrada, após isso realiza o algoritmo de huffman, que consiste em organizar a lista em ordem crescente e unir os dois elementos menos frequentas em uma árvore, até que reste somente uma na lista, também chamada de árvore perfeita.
- O TAD "bitmap.h" armazena bits incompletos e é utilizado o tempo todo para armazenar os caminhos do topo da árvore até determinado char, o tamanho do caminho, o tamanho do dicionário, etc.
- O TAD "binaryTree.h" possui todas as funções de uma árvore binária básica, com a adição dos bits (caminho) do topo da árvore até cada uma das folhas.
- O TAD "list-binaryTree.h" possui todas as funções de uma lista simplesmente encadeada básica, com a adição de uma função de organizar os elementos da lista atráves do valor de cada árvore binária.

No caso da compactação, o programa primeiramente obtém a árvore perfeita, aplicando o algoritmo de huffman, depois traduz as informações dela para um dicionário. Então, as informação de reconstrução desse dicionário são escritas no início do arquivo compactado e depois, para cara char do arquivo original, sua versão do dicionário é adicionada no arquivo compactado.

No caso da descompactação, o programa primeiramente reconstrói o dicionário e, consequentemente, a árvore perfeita, e começa a caminhar na árvore através dos bits lidos. Assim, sempre que essa árvore chega a uma folha, o char desta folha é escrita no novo arquivo original.

## Implementação do programa e conclusão:

Como forma de autoavaliação, percebemos que o código utilizou muita memória desnecessária, através dos excessivos mallocs, porém infelizmente não tivemos tempo para refazer a lógica do programa. Mas, desconsiderando esse fato, o programa consegue compactar e descompactar arquivos sem perder nenhuma informação.

A maior dificuldade do trabalho foi a manipulação dos binários, mas felizmente o bitmap cedido pelo professor João Paulo auxiliou no processo.

## Bibliografia:

Para o desenvolvimento do projeto foram utilizados principalmente os materiais fornecidos pela professora Patricia Dockhorn, o livro "Introdução a Estrutura de Dados" de Waldermar Celes e o site "stackoverflow.com".
# huffman-coding

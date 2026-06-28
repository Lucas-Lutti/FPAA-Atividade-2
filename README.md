# Resolvedor de Tango

Projeto desenvolvido para o Trabalho Pratico 2 da disciplina de Fundamentos de Projeto e Analise de Algoritmos.

O programa resolve tabuleiros do jogo Tango no terminal usando duas estrategias:

- forca bruta;
- backtracking com podas.

O objetivo e comparar uma busca exaustiva simples com uma busca que usa as regras do jogo para descartar estados invalidos mais cedo.

## Sobre o jogo

O Tango e jogado em uma grade quadrada. Cada celula deve receber um dos dois simbolos:

- `S`: Sol;
- `L`: Lua.

As regras usadas pelo programa sao:

- toda celula deve ser preenchida;
- nao pode haver tres simbolos iguais seguidos na horizontal ou na vertical;
- cada linha deve ter a mesma quantidade de Sois e Luas;
- cada coluna deve ter a mesma quantidade de Sois e Luas;
- celulas ligadas por `=` devem ter o mesmo simbolo;
- celulas ligadas por `x` devem ter simbolos opostos.

## Funcionalidades

- leitura de tabuleiros a partir de arquivo `.txt`;
- suporte a tabuleiros pares, como `4x4`, `6x6` e `8x8`;
- representacao de restricoes horizontais e verticais;
- impressao do tabuleiro inicial e do tabuleiro resolvido;
- execucao por forca bruta;
- execucao por backtracking;
- exibicao de estatisticas simples da busca.

## Estrutura do projeto

```txt
.
|-- src/
|   |-- main.cpp
|   |-- Tabuleiro.h
|   |-- Tabuleiro.cpp
|   |-- Validador.h
|   |-- Validador.cpp
|   |-- Resolvedor.h
|   `-- Resolvedor.cpp
|-- exemplos/
|   |-- exemplo_4x4.txt
|   |-- exemplo_facil_6x6.txt
|   |-- exemplo_print_6x6.txt
|   `-- tabuleiro_print_134337.txt
|-- docs/
|   |-- documentacao.md
|   |-- documentacao.pdf
|   `-- assets/
|-- Makefile
`-- README.md
```

## Organizacao do codigo

O codigo foi separado por responsabilidade:

- `Tabuleiro`: leitura do arquivo, armazenamento das celulas, restricoes e impressao no terminal;
- `Validador`: regras do jogo e validacoes usadas nas podas;
- `Resolvedor`: algoritmos de forca bruta e backtracking;
- `main.cpp`: leitura dos argumentos, escolha do algoritmo e exibicao dos resultados.

## Requisitos

Para compilar, e necessario ter um compilador C++ com suporte a C++17.

No macOS ou Linux, basta usar:

```bash
make
```

## Compilacao

Na raiz do projeto, execute:

```bash
make
```

O executavel sera criado em:

```bash
build/tango
```

Para limpar os arquivos gerados:

```bash
make clean
```

## Execucao

O formato geral e:

```bash
build/tango arquivo_de_entrada.txt algoritmo
```

Os algoritmos disponiveis sao:

- `backtracking`;
- `forca-bruta`;
- `ambos`.

Exemplos:

```bash
build/tango exemplos/exemplo_facil_6x6.txt backtracking
build/tango exemplos/exemplo_facil_6x6.txt forca-bruta
build/tango exemplos/exemplo_facil_6x6.txt ambos
```

Se nenhum algoritmo for informado, o programa usa `backtracking` por padrao:

```bash
build/tango exemplos/exemplo_facil_6x6.txt
```

## Formato da entrada

O arquivo de entrada tem tres partes:

1. tamanho do tabuleiro;
2. matriz inicial de celulas;
3. restricoes horizontais e verticais.

Para as celulas:

- `S`: Sol;
- `L`: Lua;
- `.`: vazio.

Para as restricoes:

- `=`: as duas celulas devem ser iguais;
- `x`: as duas celulas devem ser opostas;
- `.`: sem restricao.

Em um tabuleiro `6x6`:

- a matriz de celulas tem `6` linhas e `6` colunas;
- a matriz horizontal tem `6` linhas e `5` colunas;
- a matriz vertical tem `5` linhas e `6` colunas.

Exemplo:

```txt
6
. S S . . S
. . . . . .
. L . S . L
. . S . S .
S . . . . L
. L . . . .

H
. . . . .
. . . . .
. . . . .
. . . . .
. . . . .
. . . . .

V
. . . . . x
. . . . . .
. . . . x .
. . . . . .
. . . . . .
```

Linhas iniciadas por `#` podem ser usadas como comentario.

## Exemplo de saida

```txt
Algoritmo: backtracking
Chamadas recursivas: 26
Tabuleiros completos testados: 1
Tempo: 0.000043s
Solucao encontrada:
 L   S   S   L   L   S
 L   S   L   S   S   L
 S   L   S   S   L   L
 L   L   S   L   S   S
 S   S   L   L   S   L
 S   L   L   S   L   S
```

## Documentacao

A documentacao tecnica do trabalho esta em:

- `docs/documentacao.md`;
- `docs/documentacao.pdf`.

Ela contem a modelagem do problema, a explicacao da forca bruta, a explicacao do backtracking, exemplos de execucao e analise de complexidade.

## Observacao

A forca bruta esta implementada para fins de comparacao. Em tabuleiros com muitas casas vazias, ela pode demorar bastante, pois testa combinacoes completas antes de validar a solucao.

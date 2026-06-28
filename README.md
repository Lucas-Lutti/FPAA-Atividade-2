# Resolvedor de Tango

Projeto para resolver tabuleiros do jogo Tango usando duas estrategias:

- forca bruta;
- backtracking com podas.

## Compilacao

```bash
make
```

O executavel sera gerado em:

```bash
build/tango
```

## Execucao

```bash
build/tango exemplos/exemplo_facil_6x6.txt backtracking
build/tango exemplos/exemplo_facil_6x6.txt forca-bruta
build/tango exemplos/exemplo_facil_6x6.txt ambos
```

O modo padrao, quando nenhum algoritmo e informado, e `backtracking`.

## Organizacao do codigo

O codigo fica em `src/` e foi separado por responsabilidade:

- `Tabuleiro`: leitura do arquivo, armazenamento das celulas e impressao no terminal.
- `Validador`: regras do jogo e podas usadas durante a busca.
- `Resolvedor`: algoritmos de forca bruta e backtracking.
- `main.cpp`: entrada do programa e escolha do modo de execucao.

## Formato da entrada

O arquivo comeca com o tamanho do tabuleiro. Depois vem a matriz inicial, seguida das restricoes horizontais e verticais.

Para as celulas:

- `S`: Sol
- `L`: Lua
- `.`: vazio

Para as restricoes:

- `=`: as duas celulas devem ser iguais
- `x`: as duas celulas devem ser opostas
- `.`: sem restricao

Em um tabuleiro `6 x 6`, a matriz horizontal tem `6` linhas e `5` colunas. A matriz vertical tem `5` linhas e `6` colunas.

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

Linhas iniciadas por `#` podem ser usadas como comentario nos arquivos de entrada.

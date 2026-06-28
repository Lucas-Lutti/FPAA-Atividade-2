#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

enum ValorCelula {
    VAZIO = -1,
    LUA = 0,
    SOL = 1
};

enum ValorRestricao {
    OPOSTO = -1,
    SEM_RESTRICAO = 0,
    IGUAL = 1
};

struct Tabuleiro {
    int tamanho = 0;
    std::vector<std::vector<int>> celulas;
    std::vector<std::vector<int>> restricoesHorizontais;
    std::vector<std::vector<int>> restricoesVerticais;

    static Tabuleiro lerDeArquivo(const std::string& caminho);
    std::vector<std::pair<int, int>> posicoesVazias() const;
    void imprimir(std::ostream& saida) const;
};

char caractereDaCelula(int valor);
char caractereDaRestricao(int valor);

#endif

#ifndef VALIDADOR_H
#define VALIDADOR_H

#include "Tabuleiro.h"

class Validador {
public:
    static bool estaCompleto(const Tabuleiro& tabuleiro);
    static bool solucaoValida(const Tabuleiro& tabuleiro);
    static bool estadoParcialValido(const Tabuleiro& tabuleiro, int linha = -1, int coluna = -1);

private:
    static bool respeitaAdjacencia(const Tabuleiro& tabuleiro);
    static bool respeitaEquilibrio(const Tabuleiro& tabuleiro, bool exigirCompleto);
    static bool respeitaRestricoes(const Tabuleiro& tabuleiro);
};

#endif

#include "Validador.h"

bool Validador::estaCompleto(const Tabuleiro& tabuleiro) {
    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            if (tabuleiro.celulas[linha][coluna] == VAZIO) {
                return false;
            }
        }
    }
    return true;
}

bool Validador::solucaoValida(const Tabuleiro& tabuleiro) {
    return estaCompleto(tabuleiro)
        && respeitaAdjacencia(tabuleiro)
        && respeitaEquilibrio(tabuleiro, true)
        && respeitaRestricoes(tabuleiro);
}

bool Validador::estadoParcialValido(const Tabuleiro& tabuleiro, int linha, int coluna) {
    (void)linha;
    (void)coluna;
    return respeitaAdjacencia(tabuleiro)
        && respeitaEquilibrio(tabuleiro, false)
        && respeitaRestricoes(tabuleiro);
}

bool Validador::respeitaAdjacencia(const Tabuleiro& tabuleiro) {
    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna + 2 < tabuleiro.tamanho; coluna++) {
            int primeira = tabuleiro.celulas[linha][coluna];
            int segunda = tabuleiro.celulas[linha][coluna + 1];
            int terceira = tabuleiro.celulas[linha][coluna + 2];
            if (primeira != VAZIO && primeira == segunda && segunda == terceira) {
                return false;
            }
        }
    }

    for (int linha = 0; linha + 2 < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            int primeira = tabuleiro.celulas[linha][coluna];
            int segunda = tabuleiro.celulas[linha + 1][coluna];
            int terceira = tabuleiro.celulas[linha + 2][coluna];
            if (primeira != VAZIO && primeira == segunda && segunda == terceira) {
                return false;
            }
        }
    }

    return true;
}

bool Validador::respeitaEquilibrio(const Tabuleiro& tabuleiro, bool exigirCompleto) {
    int metade = tabuleiro.tamanho / 2;

    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        int sois = 0;
        int luas = 0;
        int vazias = 0;
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            if (tabuleiro.celulas[linha][coluna] == SOL) {
                sois++;
            } else if (tabuleiro.celulas[linha][coluna] == LUA) {
                luas++;
            } else {
                vazias++;
            }
        }

        if (sois > metade || luas > metade) {
            return false;
        }
        if ((exigirCompleto || vazias == 0) && (sois != metade || luas != metade)) {
            return false;
        }
    }

    for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
        int sois = 0;
        int luas = 0;
        int vazias = 0;
        for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
            if (tabuleiro.celulas[linha][coluna] == SOL) {
                sois++;
            } else if (tabuleiro.celulas[linha][coluna] == LUA) {
                luas++;
            } else {
                vazias++;
            }
        }

        if (sois > metade || luas > metade) {
            return false;
        }
        if ((exigirCompleto || vazias == 0) && (sois != metade || luas != metade)) {
            return false;
        }
    }

    return true;
}

bool Validador::respeitaRestricoes(const Tabuleiro& tabuleiro) {
    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho - 1; coluna++) {
            int restricao = tabuleiro.restricoesHorizontais[linha][coluna];
            if (restricao == SEM_RESTRICAO) {
                continue;
            }

            int esquerda = tabuleiro.celulas[linha][coluna];
            int direita = tabuleiro.celulas[linha][coluna + 1];
            if (esquerda == VAZIO || direita == VAZIO) {
                continue;
            }

            if (restricao == IGUAL && esquerda != direita) {
                return false;
            }
            if (restricao == OPOSTO && esquerda == direita) {
                return false;
            }
        }
    }

    for (int linha = 0; linha < tabuleiro.tamanho - 1; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            int restricao = tabuleiro.restricoesVerticais[linha][coluna];
            if (restricao == SEM_RESTRICAO) {
                continue;
            }

            int cima = tabuleiro.celulas[linha][coluna];
            int baixo = tabuleiro.celulas[linha + 1][coluna];
            if (cima == VAZIO || baixo == VAZIO) {
                continue;
            }

            if (restricao == IGUAL && cima != baixo) {
                return false;
            }
            if (restricao == OPOSTO && cima == baixo) {
                return false;
            }
        }
    }

    return true;
}

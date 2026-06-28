#ifndef RESOLVEDOR_H
#define RESOLVEDOR_H

#include "Tabuleiro.h"

#include <vector>

struct EstatisticasResolucao {
    long long chamadasRecursivas = 0;
    long long tabuleirosCompletos = 0;
    double tempoSegundos = 0.0;
};

class Resolvedor {
public:
    static bool resolverForcaBruta(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas);
    static bool resolverBacktracking(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas);

private:
    static bool forcaBrutaRecursiva(
        Tabuleiro& tabuleiro,
        const std::vector<std::pair<int, int>>& casasVazias,
        int indice,
        EstatisticasResolucao& estatisticas
    );

    static bool backtrackingRecursivo(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas);
};

#endif

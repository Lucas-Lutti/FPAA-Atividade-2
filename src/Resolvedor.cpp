#include "Resolvedor.h"

#include "Validador.h"

#include <chrono>

namespace {

struct EscolhaCelula {
    int linha = -1;
    int coluna = -1;
    bool tabuleiroCompleto = false;
    bool semOpcaoValida = false;
    std::vector<int> valoresPossiveis;
};

EscolhaCelula escolherCelulaComMenosOpcoes(Tabuleiro& tabuleiro) {
    EscolhaCelula melhorEscolha;
    int menorQuantidadeOpcoes = 3;

    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            if (tabuleiro.celulas[linha][coluna] != VAZIO) {
                continue;
            }

            std::vector<int> valoresPossiveis;
            for (int valor : {LUA, SOL}) {
                tabuleiro.celulas[linha][coluna] = valor;
                if (Validador::estadoParcialValido(tabuleiro, linha, coluna)) {
                    valoresPossiveis.push_back(valor);
                }
                tabuleiro.celulas[linha][coluna] = VAZIO;
            }

            if (valoresPossiveis.empty()) {
                melhorEscolha.semOpcaoValida = true;
                return melhorEscolha;
            }

            if (static_cast<int>(valoresPossiveis.size()) < menorQuantidadeOpcoes) {
                melhorEscolha.linha = linha;
                melhorEscolha.coluna = coluna;
                melhorEscolha.valoresPossiveis = valoresPossiveis;
                menorQuantidadeOpcoes = static_cast<int>(valoresPossiveis.size());

                if (menorQuantidadeOpcoes == 1) {
                    return melhorEscolha;
                }
            }
        }
    }

    if (melhorEscolha.linha == -1) {
        melhorEscolha.tabuleiroCompleto = true;
    }
    return melhorEscolha;
}

} // namespace

bool Resolvedor::resolverForcaBruta(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas) {
    estatisticas = EstatisticasResolucao{};
    std::vector<std::pair<int, int>> casasVazias = tabuleiro.posicoesVazias();

    auto inicio = std::chrono::steady_clock::now();
    bool encontrouSolucao = forcaBrutaRecursiva(tabuleiro, casasVazias, 0, estatisticas);
    auto fim = std::chrono::steady_clock::now();

    estatisticas.tempoSegundos = std::chrono::duration<double>(fim - inicio).count();
    return encontrouSolucao;
}

bool Resolvedor::resolverBacktracking(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas) {
    estatisticas = EstatisticasResolucao{};

    auto inicio = std::chrono::steady_clock::now();
    bool encontrouSolucao = backtrackingRecursivo(tabuleiro, estatisticas);
    auto fim = std::chrono::steady_clock::now();

    estatisticas.tempoSegundos = std::chrono::duration<double>(fim - inicio).count();
    return encontrouSolucao;
}

bool Resolvedor::forcaBrutaRecursiva(
    Tabuleiro& tabuleiro,
    const std::vector<std::pair<int, int>>& casasVazias,
    int indice,
    EstatisticasResolucao& estatisticas
) {
    estatisticas.chamadasRecursivas++;

    if (indice == static_cast<int>(casasVazias.size())) {
        estatisticas.tabuleirosCompletos++;
        return Validador::solucaoValida(tabuleiro);
    }

    int linha = casasVazias[indice].first;
    int coluna = casasVazias[indice].second;

    for (int valor : {LUA, SOL}) {
        tabuleiro.celulas[linha][coluna] = valor;
        if (forcaBrutaRecursiva(tabuleiro, casasVazias, indice + 1, estatisticas)) {
            return true;
        }
    }

    tabuleiro.celulas[linha][coluna] = VAZIO;
    return false;
}

bool Resolvedor::backtrackingRecursivo(Tabuleiro& tabuleiro, EstatisticasResolucao& estatisticas) {
    estatisticas.chamadasRecursivas++;

    EscolhaCelula escolha = escolherCelulaComMenosOpcoes(tabuleiro);
    if (escolha.semOpcaoValida) {
        return false;
    }

    if (escolha.tabuleiroCompleto) {
        estatisticas.tabuleirosCompletos++;
        return Validador::solucaoValida(tabuleiro);
    }

    for (int valor : escolha.valoresPossiveis) {
        tabuleiro.celulas[escolha.linha][escolha.coluna] = valor;
        if (backtrackingRecursivo(tabuleiro, estatisticas)) {
            return true;
        }
    }

    tabuleiro.celulas[escolha.linha][escolha.coluna] = VAZIO;
    return false;
}

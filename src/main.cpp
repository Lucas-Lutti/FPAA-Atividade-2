#include "Resolvedor.h"
#include "Tabuleiro.h"
#include "Validador.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using std::string;

namespace {

string minusculoAscii(string texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char caractere) {
        return static_cast<char>(std::tolower(caractere));
    });
    return texto;
}

void mostrarUso(const char* nomePrograma) {
    std::cout
        << "Uso:\n"
        << "  " << nomePrograma << " arquivo.txt [backtracking|forca-bruta|ambos]\n\n"
        << "Exemplos:\n"
        << "  " << nomePrograma << " exemplos/exemplo_facil_6x6.txt backtracking\n"
        << "  " << nomePrograma << " exemplos/exemplo_facil_6x6.txt forca-bruta\n";
}

void mostrarEstatisticas(const EstatisticasResolucao& estatisticas) {
    std::cout << "Chamadas recursivas: " << estatisticas.chamadasRecursivas << '\n';
    std::cout << "Tabuleiros completos testados: " << estatisticas.tabuleirosCompletos << '\n';
    std::cout << "Tempo: " << std::fixed << std::setprecision(6) << estatisticas.tempoSegundos << "s\n";
}

void executarAlgoritmo(const Tabuleiro& original, const string& algoritmo) {
    Tabuleiro tabuleiro = original;
    EstatisticasResolucao estatisticas;

    std::cout << "\nAlgoritmo: " << algoritmo << '\n';

    bool encontrouSolucao = false;
    if (algoritmo == "backtracking") {
        encontrouSolucao = Resolvedor::resolverBacktracking(tabuleiro, estatisticas);
    } else {
        encontrouSolucao = Resolvedor::resolverForcaBruta(tabuleiro, estatisticas);
    }

    mostrarEstatisticas(estatisticas);

    if (encontrouSolucao) {
        std::cout << "Solucao encontrada:\n";
        tabuleiro.imprimir(std::cout);
    } else {
        std::cout << "Nenhuma solucao valida foi encontrada.\n";
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        mostrarUso(argv[0]);
        return 1;
    }

    string caminho = argv[1];
    string modo = argc == 3 ? minusculoAscii(argv[2]) : "backtracking";

    if (modo == "bruta" || modo == "bruteforce") {
        modo = "forca-bruta";
    }

    if (modo != "backtracking" && modo != "forca-bruta" && modo != "ambos") {
        mostrarUso(argv[0]);
        return 1;
    }

    try {
        Tabuleiro tabuleiro = Tabuleiro::lerDeArquivo(caminho);

        std::cout << "Arquivo: " << caminho << '\n';
        std::cout << "Tamanho: " << tabuleiro.tamanho << " x " << tabuleiro.tamanho << "\n\n";
        std::cout << "Tabuleiro inicial:\n";
        tabuleiro.imprimir(std::cout);

        if (!Validador::estadoParcialValido(tabuleiro)) {
            std::cout << "\nA entrada ja viola alguma regra do jogo.\n";
            return 2;
        }

        if (modo == "ambos") {
            executarAlgoritmo(tabuleiro, "forca-bruta");
            executarAlgoritmo(tabuleiro, "backtracking");
        } else {
            executarAlgoritmo(tabuleiro, modo);
        }
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 2;
    }

    return 0;
}

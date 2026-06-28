#include "Tabuleiro.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

using std::string;
using std::vector;

namespace {

string maiusculoAscii(string texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char caractere) {
        return static_cast<char>(std::toupper(caractere));
    });
    return texto;
}

vector<string> lerTokensDoArquivo(const string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);
    }

    vector<string> tokens;
    string linhaTexto;
    while (std::getline(arquivo, linhaTexto)) {
        size_t posicaoComentario = linhaTexto.find('#');
        if (posicaoComentario != string::npos) {
            linhaTexto = linhaTexto.substr(0, posicaoComentario);
        }

        std::istringstream fluxo(linhaTexto);
        string token;
        while (fluxo >> token) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int converterInteiro(const string& token) {
    size_t caracteresLidos = 0;
    int valor = std::stoi(token, &caracteresLidos);
    if (caracteresLidos != token.size()) {
        throw std::runtime_error("Numero invalido na entrada: " + token);
    }
    return valor;
}

int lerCelula(const string& token) {
    string valor = maiusculoAscii(token);
    if (valor == "." || valor == "-" || valor == "_" || valor == "?") {
        return VAZIO;
    }
    if (valor == "S" || valor == "SOL" || valor == "1") {
        return SOL;
    }
    if (valor == "L" || valor == "LUA" || valor == "0") {
        return LUA;
    }
    throw std::runtime_error("Celula invalida na entrada: " + token);
}

int lerRestricao(const string& token) {
    string valor = maiusculoAscii(token);
    if (valor == "." || valor == "-" || valor == "_" || valor == "0") {
        return SEM_RESTRICAO;
    }
    if (valor == "=") {
        return IGUAL;
    }
    if (valor == "X" || valor == "*" || token == "\xC3\x97") {
        return OPOSTO;
    }
    throw std::runtime_error("Restricao invalida na entrada: " + token);
}

void esperarMarcador(const vector<string>& tokens, size_t& posicaoAtual, const string& marcador) {
    if (posicaoAtual >= tokens.size() || maiusculoAscii(tokens[posicaoAtual]) != marcador) {
        throw std::runtime_error("Era esperado o marcador " + marcador + " na entrada.");
    }
    posicaoAtual++;
}

string mensagemPosicao(const string& secao, int linha, int coluna) {
    std::ostringstream mensagem;
    mensagem << "Faltou valor em " << secao << " na posicao ("
             << linha + 1 << ", " << coluna + 1 << ").";
    return mensagem.str();
}

} // namespace

Tabuleiro Tabuleiro::lerDeArquivo(const string& caminho) {
    vector<string> tokens = lerTokensDoArquivo(caminho);
    if (tokens.empty()) {
        throw std::runtime_error("Arquivo de entrada vazio.");
    }

    size_t posicaoAtual = 0;
    Tabuleiro tabuleiro;
    tabuleiro.tamanho = converterInteiro(tokens[posicaoAtual++]);

    if (tabuleiro.tamanho <= 0 || tabuleiro.tamanho % 2 != 0) {
        throw std::runtime_error("O tamanho do tabuleiro precisa ser um numero par positivo.");
    }

    tabuleiro.celulas.assign(tabuleiro.tamanho, vector<int>(tabuleiro.tamanho, VAZIO));
    tabuleiro.restricoesHorizontais.assign(
        tabuleiro.tamanho,
        vector<int>(tabuleiro.tamanho - 1, SEM_RESTRICAO)
    );
    tabuleiro.restricoesVerticais.assign(
        tabuleiro.tamanho - 1,
        vector<int>(tabuleiro.tamanho, SEM_RESTRICAO)
    );

    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            if (posicaoAtual >= tokens.size()) {
                throw std::runtime_error(mensagemPosicao("tabuleiro", linha, coluna));
            }
            tabuleiro.celulas[linha][coluna] = lerCelula(tokens[posicaoAtual++]);
        }
    }

    esperarMarcador(tokens, posicaoAtual, "H");
    for (int linha = 0; linha < tabuleiro.tamanho; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho - 1; coluna++) {
            if (posicaoAtual >= tokens.size()) {
                throw std::runtime_error(mensagemPosicao("restricoes horizontais", linha, coluna));
            }
            tabuleiro.restricoesHorizontais[linha][coluna] = lerRestricao(tokens[posicaoAtual++]);
        }
    }

    esperarMarcador(tokens, posicaoAtual, "V");
    for (int linha = 0; linha < tabuleiro.tamanho - 1; linha++) {
        for (int coluna = 0; coluna < tabuleiro.tamanho; coluna++) {
            if (posicaoAtual >= tokens.size()) {
                throw std::runtime_error(mensagemPosicao("restricoes verticais", linha, coluna));
            }
            tabuleiro.restricoesVerticais[linha][coluna] = lerRestricao(tokens[posicaoAtual++]);
        }
    }

    if (posicaoAtual != tokens.size()) {
        throw std::runtime_error("Ha informacoes sobrando no final do arquivo de entrada.");
    }

    return tabuleiro;
}

vector<std::pair<int, int>> Tabuleiro::posicoesVazias() const {
    vector<std::pair<int, int>> posicoes;
    for (int linha = 0; linha < tamanho; linha++) {
        for (int coluna = 0; coluna < tamanho; coluna++) {
            if (celulas[linha][coluna] == VAZIO) {
                posicoes.push_back({linha, coluna});
            }
        }
    }
    return posicoes;
}

void Tabuleiro::imprimir(std::ostream& saida) const {
    for (int linha = 0; linha < tamanho; linha++) {
        saida << " ";
        for (int coluna = 0; coluna < tamanho; coluna++) {
            saida << caractereDaCelula(celulas[linha][coluna]);
            if (coluna < tamanho - 1) {
                saida << " " << caractereDaRestricao(restricoesHorizontais[linha][coluna]) << " ";
            }
        }
        saida << '\n';

        if (linha < tamanho - 1) {
            saida << " ";
            for (int coluna = 0; coluna < tamanho; coluna++) {
                saida << caractereDaRestricao(restricoesVerticais[linha][coluna]);
                if (coluna < tamanho - 1) {
                    saida << "   ";
                }
            }
            saida << '\n';
        }
    }
}

char caractereDaCelula(int valor) {
    if (valor == SOL) {
        return 'S';
    }
    if (valor == LUA) {
        return 'L';
    }
    return '.';
}

char caractereDaRestricao(int valor) {
    if (valor == IGUAL) {
        return '=';
    }
    if (valor == OPOSTO) {
        return 'x';
    }
    return ' ';
}

// main.cpp - le o arquivo de entrada e despacha os comandos.
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lista_ordenada.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "uso: " << argv[0] << " arquivo_de_entrada\n";
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cerr << "nao foi possivel abrir " << argv[1] << "\n";
        return 1;
    }

    ListaOrdenada lista;
    std::string linha;
    while (std::getline(entrada, linha)) {
        // tolera entrada com fim de linha do Windows (CRLF)
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();

        std::istringstream ss(linha);
        std::string cmd;
        ss >> cmd;

        if (cmd == "INC") {
            int n;
            if (ss >> n) lista.incluir(n);
        } else if (cmd == "REM") {
            int n;
            if (ss >> n) lista.remover(n);
        } else if (cmd == "SUC") {
            int n, m;
            if (ss >> n >> m) {
                std::cout << linha << "\n";
                const auto s = lista.sucessor(n, m);
                if (s) std::cout << *s << "\n";
                else std::cout << "inf\n";
            }
        } else if (cmd == "IMP") {
            int m;
            if (ss >> m) {
                std::cout << linha << "\n";
                std::cout << lista.imprimir(m) << "\n";
            }
        }
    }
    return 0;
}

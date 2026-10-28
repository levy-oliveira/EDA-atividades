// main.cpp - le o arquivo de entrada e despacha os comandos.
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    std::string linha;
    while (std::getline(entrada, linha)) {
        // tolera entrada com fim de linha do Windows (CRLF)
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();

        std::istringstream ss(linha);
        std::string cmd;
        ss >> cmd;

        if (cmd == "INC") {
            int n;
            ss >> n;
            // TODO: incluir n e criar nova versao
        } else if (cmd == "REM") {
            int n;
            ss >> n;
            // TODO: remover n (se existir) e criar nova versao
        } else if (cmd == "SUC") {
            int n, m;
            ss >> n >> m;
            std::cout << linha << "\n";
            // TODO: imprimir o sucessor de n na versao m
        } else if (cmd == "IMP") {
            int m;
            ss >> m;
            std::cout << linha << "\n";
            // TODO: imprimir a estrutura na versao m
        }
    }
    return 0;
}

// lista_ordenada.h - lista encadeada ordenada com persistencia parcial.
#pragma once

#include <string>
#include <vector>

#include "persistencia.h"

class ListaOrdenada {
public:
    ListaOrdenada();  // versao 0: lista vazia

    int versao_atual() const;   // ultimo id de versao (0 = vazia)
    int total_nos() const;

    void incluir(int chave);    // INC: cria uma versao
    void remover(int chave);    // REM: cria uma versao (mesmo se nao achar)

    // SUC: menor chave estritamente maior que `chave` na versao `versao`.
    // Retorna -1 se nao existir tal chave.
    int sucessor(int chave, int versao) const;

    // IMP: elementos da versao `versao` em ordem crescente, separados por ", ".
    std::string imprimir(int versao) const;

private:
    int normalizar(int versao) const;  // versao inexistente -> ultima versao

    Persistencia mec_;           // declarado antes de raizes_ (ordem de inicializacao)
    std::vector<No*> raizes_;    // raizes_[v] = raiz da versao v
};

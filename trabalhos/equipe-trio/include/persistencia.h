// persistencia.h - mecanismo de persistencia parcial (copia de nos).
//
// Esta classe NAO sabe nada sobre listas ordenadas: so sabe ler e escrever o
// campo `next` de um no em uma versao. A lista ordenada e construida por cima.
#pragma once

#include <vector>

#include "no.h"

class Persistencia {
public:
    // Cria o no sentinela (raiz da versao 0). Ele nao guarda dado e nunca e apontado.
    Persistencia();

    // Raiz (sentinela) da ULTIMA versao. Muda quando a raiz e copiada.
    No* raiz() const;

    // Total de nos alocados (inclui as copias): usado para conferir o espaco O(v).
    int total_nos() const;

    // Aloca um no novo com `new` (next = nullptr, sem mods).
    No* criar_no(int chave);

    // LER: valor de n->next na versao `versao`.
    static No* ler(const No* n, int versao);

    // Grava o campo original `next` de um no criado NESTA versao (nao precisa de mod).
    void inicializar_next(No* n, No* alvo);

    // ESCREVER: faz n->next = novo, criando a versao `versao`.
    // Se os mods de n estiverem cheios, copia n e propaga a escrita pelos ponteiros de retorno.
    void escrever(No* n, No* novo, int versao);

private:
    static No* proximo_atual(const No* n);  // valor de n->next na ultima versao

    std::vector<No*> pool_;  // ponteiros para nos alocados com new; endereco nunca muda
    No* raiz_;
};

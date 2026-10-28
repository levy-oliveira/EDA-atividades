// persistencia.hpp - mecanismo de persistencia parcial (copia de nos).
//
// Esta classe NAO sabe nada sobre listas ordenadas: so sabe ler e escrever o
// campo `next` de um no em uma versao. A lista ordenada e construida por cima.
#pragma once

#include <cstddef>
#include <deque>

#include "no.hpp"

class Persistencia {
public:
    // Cria o no sentinela (raiz da versao 0). Ele nao guarda dado e nunca e apontado.
    Persistencia();
    Persistencia(const Persistencia&) = delete;
    Persistencia& operator=(const Persistencia&) = delete;

    // Raiz (sentinela) da ULTIMA versao. Muda quando a raiz e copiada.
    No* raiz() const { return raiz_; }

    // Total de nos alocados (inclui as copias): usado para conferir o espaco O(v).
    std::size_t total_nos() const { return pool_.size(); }

    // Aloca um no novo (next = nullptr, sem mods). Os enderecos sao estaveis.
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

    std::deque<No> pool_;  // deque: push_back nao invalida enderecos
    No* raiz_;
};

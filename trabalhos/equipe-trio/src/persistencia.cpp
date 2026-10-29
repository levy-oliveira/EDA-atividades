// persistencia.cpp - implementacao de LER e ESCREVER (ver persistencia.hpp).
#include "../include/persistencia.h"

#include <cassert>
#include <cstddef>

No* Persistencia::raiz() const {
    return raiz_;
}

std::size_t Persistencia::total_nos() const {
    return pool_.size();
}

Persistencia::Persistencia() : raiz_(nullptr) {
    raiz_ = criar_no(-1);  // sentinela: chave -1 nunca colide com chaves >= 0
}

No* Persistencia::criar_no(int chave) {
    pool_.push_back(No{chave, nullptr, nullptr, 0, {}});
    return &pool_.back();
}

No* Persistencia::proximo_atual(const No* n) {
    return n->nmods > 0 ? n->mods[n->nmods - 1].valor : n->next;
}

// Comeca no valor original e aplica, em ordem, as mods com versao <= `versao`.
// No maximo 2p mods: custo O(1).
No* Persistencia::ler(const No* n, int versao) {
    No* valor = n->next;
    for (int i = 0; i < n->nmods; ++i) {
        if (n->mods[i].versao <= versao) valor = n->mods[i].valor;
    }
    return valor;
}

void Persistencia::inicializar_next(No* n, No* alvo) {
    assert(n->nmods == 0 && n->next == nullptr);
    n->next = alvo;
    if (alvo != nullptr) {
        assert(alvo->retorno == nullptr);
        alvo->retorno = n;
    }
}

void Persistencia::escrever(No* n, No* novo, int versao) {
    // n deixa de apontar para o alvo atual: ele perde o ponteiro de retorno.
    No* antigo = proximo_atual(n);
    if (antigo != nullptr) {
        assert(antigo->retorno == n);
        antigo->retorno = nullptr;
    }

    if (n->nmods < kMaxMods) {
        // Caso 1: ha espaco nos mods. Basta anotar (versao, novo valor).
        n->mods[n->nmods++] = Mod{versao, novo};
        if (novo != nullptr) novo->retorno = n;
        return;
    }

    // Caso 2: mods cheios. Cria a copia, que ja nasce com o estado mais recente
    // (o unico campo mutavel e `next`, que agora e `novo`) e com mods vazios.
    No* copia = criar_no(n->chave);
    copia->next = novo;
    if (novo != nullptr) novo->retorno = copia;

    if (n == raiz_) {
        // n era a raiz: a copia e a raiz desta versao em diante.
        raiz_ = copia;
    } else {
        // O predecessor de n tem que passar a apontar para a copia. Isso e uma
        // escrita como outra qualquer e pode, por sua vez, copiar o predecessor.
        assert(n->retorno != nullptr);
        escrever(n->retorno, copia, versao);
    }
}

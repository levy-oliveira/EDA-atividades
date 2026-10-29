// lista_ordenada.cpp - o algoritmo da lista ordenada, escrito como na versao
// efemera, mas lendo com Persistencia::ler e escrevendo com Persistencia::escrever.
#include "../include/lista_ordenada.h"
#include <cstddef>

ListaOrdenada::ListaOrdenada() {
    raizes_.push_back(mec_.raiz());
}

int ListaOrdenada::versao_atual() const {
    return static_cast<int>(raizes_.size()) - 1;
}

std::size_t ListaOrdenada::total_nos() const {
    return mec_.total_nos();
}

int ListaOrdenada::normalizar(int versao) const {
    if (versao < 0 || versao > versao_atual()) return versao_atual();
    return versao;
}

void ListaOrdenada::incluir(int chave) {
    const int atual = versao_atual();
    const int nova = atual + 1;

    // anterior = ultimo no com chave < `chave`; seguinte = o que vem depois dele.
    No* anterior = mec_.raiz();
    No* seguinte = Persistencia::ler(anterior, atual);
    while (seguinte != nullptr && seguinte->chave < chave) {
        anterior = seguinte;
        seguinte = Persistencia::ler(seguinte, atual);
    }

    // Ordem importa: primeiro anterior->next = novo, depois novo->next = seguinte.
    // Assim nenhum no chega a ter dois ponteiros de entrada (p = 1 sempre vale).
    No* novo = mec_.criar_no(chave);
    mec_.escrever(anterior, novo, nova);
    mec_.inicializar_next(novo, seguinte);

    raizes_.push_back(mec_.raiz());
}

void ListaOrdenada::remover(int chave) {
    const int atual = versao_atual();
    const int nova = atual + 1;

    No* anterior = mec_.raiz();
    No* alvo = Persistencia::ler(anterior, atual);
    while (alvo != nullptr && alvo->chave < chave) {
        anterior = alvo;
        alvo = Persistencia::ler(alvo, atual);
    }

    if (alvo != nullptr && alvo->chave == chave) {
        No* seguinte = Persistencia::ler(alvo, atual);
        mec_.escrever(anterior, seguinte, nova);  // anterior->next = seguinte
    }
    // Mesmo sem remover nada, a versao existe (a raiz nao mudou).
    raizes_.push_back(mec_.raiz());
}

std::optional<int> ListaOrdenada::sucessor(int chave, int versao) const {
    const int v = normalizar(versao);
    // A lista e ordenada: o primeiro nome > chave e o sucessor.
    for (const No* n = Persistencia::ler(raizes_[v], v); n != nullptr; n = Persistencia::ler(n, v)) {
        if (n->chave > chave) return n->chave;
    }
    return std::nullopt;
}

std::string ListaOrdenada::imprimir(int versao) const {
    const int v = normalizar(versao);
    std::string saida;
    for (const No* n = Persistencia::ler(raizes_[v], v); n != nullptr; n = Persistencia::ler(n, v)) {
        if (!saida.empty()) saida += ", ";
        saida += std::to_string(n->chave);
    }
    return saida;
}

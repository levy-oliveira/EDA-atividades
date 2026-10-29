// no.hpp - o no da lista persistente (modelo "maquina de ponteiros").
#pragma once

// Hipotese do teorema de persistencia parcial: no maximo p ponteiros apontam
// para cada no. Numa lista encadeada simples so o no anterior aponta para ele.
constexpr int kP = 1;

// Cada no guarda ate 2p modificacoes antes de precisar ser copiado.
constexpr int kMaxMods = 2 * kP;

struct No;

// Uma modificacao do campo `next`: "a partir da versao `versao`, next = valor".
// (Com mais de um campo ponteiro, seria preciso guardar tambem qual campo.)
struct Mod {
    int versao;
    No* valor;
};

struct No {
    int chave;        // imutavel: nunca e reescrita
    No* next;         // valor ORIGINAL do campo next (vale ate a primeira mod)
    No* retorno;      // quem aponta para este no na ULTIMA versao (no maximo p = 1)
    int nmods;        // quantas posicoes de mods estao em uso
    Mod mods[kMaxMods];  // em ordem crescente de versao
};

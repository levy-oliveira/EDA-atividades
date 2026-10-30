// no.h - o no da lista persistente (modelo "maquina de ponteiros").
#pragma once

// Numa lista encadeada simples, cada no tem no maximo 1 ponteiro apontando
// para ele. Por isso, cada no guarda ate 2 modificacoes antes de ser copiado.
const int MAX_MODS = 2;

struct No;

// Uma modificacao do campo `next`: "na versao `versao`, next passou a ser `valor`".
struct Mod {
    int versao;
    No* valor;
};

struct No {
    int chave;            // nunca e alterado apos a criacao do no
    No* next;             // valor original de next (vale ate a primeira mod)
    No* retorno;          // quem aponta para este no na versao mais recente
    int nmods;            // quantas entradas de mods estao em uso
    Mod mods[MAX_MODS];   // historico de modificacoes, em ordem crescente de versao
};

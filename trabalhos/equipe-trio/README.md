# Estruturas de Dados Avançadas - Trabalho 01

## Equipe

Luiz Gustavo (matrícula 537585), Levy Oliveira (matrícula 541800) e Anna Beatriz (matrícula 415152)

## Descrição

- **Linguagem de programação utilizada:** C++17
- **Estruturas:**
  - **`Mod`** — definida em `include/node.h`
    - Registra uma modificação do campo `next`
    - `version`: versão em que a modificação ocorreu
    - `value`: novo valor de `next` a partir dessa versão

  - **`Node`** — definida em `include/node.h`
    - Nó da lista encadeada persistente
    - `key`: valor inteiro armazenado (nunca alterado após a criação)
    - `next`: valor original do ponteiro para o próximo nó
    - `back`: ponteiro para o nó que aponta para este na versão mais recente (permite propagar cópias)
    - `mod_count`: quantidade de modificações registradas em `mods`
    - `mods[MAX_MODS]`: histórico de até 2 modificações do campo `next`, em ordem crescente de versão

  - **`Persistence`** — definida em `include/persistence.h`, implementada em `src/persistence.cpp`
    - Implementa a persistência parcial pelo método da cópia de nós (máquina de ponteiros)
    - Não conhece a lógica da lista ordenada: apenas lê e escreve o campo `next` de um nó em uma versão
    - `create_node(key)`: aloca um novo nó e o registra no pool (liberado pelo destrutor)
    - `read(n, version)`: retorna o valor de `n->next` na versão `version`
    - `init_next(n, target)`: define o `next` original de um nó recém-criado sem registrar modificação
    - `write(n, value, version)`: faz `n->next = value` na versão `version`; se o nó já tiver `MAX_MODS` modificações, cria uma cópia e propaga a escrita recursivamente pelos ponteiros `back`
    - `root()`: retorna o nó sentinela da versão mais recente
    - `node_count()`: retorna o total de nós alocados, incluindo cópias

  - **`SortedList`** — definida em `include/sorted_list.h`, implementada em `src/sorted_list.cpp`
    - Lista encadeada ordenada com persistência parcial, construída sobre `Persistence`
    - `insert(key)`: insere `key` na posição correta e cria uma nova versão
    - `remove(key)`: remove uma ocorrência de `key` e cria uma nova versão (mesmo se não encontrar)
    - `successor(key, version)`: retorna o menor valor estritamente maior que `key` na versão `version`, ou `-1` se não existir
    - `print(version)`: retorna os elementos da versão `version` em ordem crescente, separados por `", "`
    - `current_version()`: retorna o índice da versão mais recente
    - `normalize(version)`: converte uma versão inexistente na versão mais recente

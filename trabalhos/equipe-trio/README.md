# Estruturas de Dados Avançadas - Trabalho 01

## Equipe

Fulano (matrícula 123456) e Cicrano (matrícula 654321)

## Descrição

- **Linguagem de programação utilizada:** C++17
- **Estruturas:**
  - **`Mod`** — definida em `include/no.h`
    - Registra uma modificação do campo `next` de um nó
    - `versao`: versão em que a modificação ocorreu
    - `valor`: novo valor de `next` a partir dessa versão

  - **`No`** — definida em `include/no.h`
    - Nó da lista encadeada persistente
    - `chave`: valor inteiro armazenado (nunca alterado após a criação)
    - `next`: valor original do ponteiro para o próximo nó
    - `retorno`: ponteiro para o nó que aponta para este na versão mais recente (permite propagar cópias)
    - `nmods`: quantidade de modificações registradas em `mods`
    - `mods[MAX_MODS]`: histórico de até 2 modificações do campo `next`, em ordem crescente de versão

  - **`Persistencia`** — definida em `include/persistencia.h`, implementada em `src/persistencia.cpp`
    - Implementa o mecanismo de persistência parcial pelo método da máquina de ponteiros
    - Não conhece a lógica de lista ordenada: apenas lê e escreve o campo `next` de um nó em uma versão
    - `criar_no(chave)`: aloca um novo nó com `new` e o registra no pool
    - `ler(n, versao)`: retorna o valor de `n->next` na versão `versao`, consultando o histórico de modificações
    - `inicializar_next(n, alvo)`: define o `next` original de um nó recém-criado sem registrar modificação
    - `escrever(n, novo, versao)`: faz `n->next = novo` na versão `versao`; se o nó já tiver `MAX_MODS` modificações, cria uma cópia e propaga a escrita recursivamente pelos ponteiros de retorno
    - `raiz()`: retorna o nó sentinela da versão mais recente
    - `total_nos()`: retorna o total de nós alocados, incluindo cópias

  - **`ListaOrdenada`** — definida em `include/lista_ordenada.h`, implementada em `src/lista_ordenada.cpp`
    - Lista encadeada ordenada com persistência parcial, construída sobre `Persistencia`
    - `incluir(chave)`: insere `chave` na posição correta e cria uma nova versão
    - `remover(chave)`: remove a primeira ocorrência de `chave` e cria uma nova versão (mesmo se não encontrar)
    - `sucessor(chave, versao)`: retorna o menor valor estritamente maior que `chave` na versão `versao`, ou `-1` se não existir
    - `imprimir(versao)`: retorna os elementos da versão `versao` em ordem crescente, separados por `", "`
    - `versao_atual()`: retorna o índice da versão mais recente
    - `normalizar(versao)`: converte uma versão inexistente na versão mais recente
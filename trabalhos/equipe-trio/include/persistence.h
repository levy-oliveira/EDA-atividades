#pragma once

#include <vector>

#include "node.h"

class Persistence {
public:
    Persistence();
    ~Persistence();
    Persistence(const Persistence&) = delete;
    Persistence& operator=(const Persistence&) = delete;

    Node* root() const;
    int node_count() const;

    Node* create_node(int key);

    static Node* read(const Node* n, int version);

    void init_next(Node* n, Node* target);
    void write(Node* n, Node* value, int version);

private:
    std::vector<Node*> pool_;
    Node* root_;
};

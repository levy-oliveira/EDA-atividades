#include "../include/persistence.h"

Persistence::Persistence() : root_(create_node(-1)) {}

Persistence::~Persistence() {
    for (Node* n : pool_) delete n;
}

Node* Persistence::root() const {
    return root_;
}

int Persistence::node_count() const {
    return (int)pool_.size();
}

Node* Persistence::create_node(int key) {
    Node* node = new Node{key, nullptr, nullptr, 0, {}};
    pool_.push_back(node);
    return node;
}

Node* Persistence::read(const Node* n, int version) {
    Node* value = n->next;
    for (int i = 0; i < n->mod_count; ++i) {
        if (n->mods[i].version <= version) value = n->mods[i].value;
    }
    return value;
}

void Persistence::init_next(Node* n, Node* target) {
    n->next = target;
    if (target != nullptr) target->back = n;
}

void Persistence::write(Node* n, Node* value, int version) {
    Node* old = n->mod_count > 0 ? n->mods[n->mod_count - 1].value : n->next;
    if (old != nullptr) old->back = nullptr;

    if (n->mod_count < MAX_MODS) {
        n->mods[n->mod_count++] = Mod{version, value};
        if (value != nullptr) value->back = n;
        return;
    }

    Node* copy = create_node(n->key);
    copy->next = value;
    if (value != nullptr) value->back = copy;

    if (n == root_) root_ = copy;
    else write(n->back, copy, version);
}

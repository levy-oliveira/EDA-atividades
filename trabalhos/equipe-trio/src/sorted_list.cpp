#include "../include/sorted_list.h"

SortedList::SortedList() {
    roots_.push_back(persistence_.root());
}

int SortedList::current_version() const {
    return (int)roots_.size() - 1;
}

int SortedList::node_count() const {
    return persistence_.node_count();
}

int SortedList::normalize(int version) const {
    if (version < 0 || version > current_version()) return current_version();
    return version;
}

void SortedList::insert(int key) {
    const int current = current_version();

    Node* prev = persistence_.root();
    Node* next = Persistence::read(prev, current);
    while (next != nullptr && next->key < key) {
        prev = next;
        next = Persistence::read(next, current);
    }

    Node* node = persistence_.create_node(key);
    persistence_.write(prev, node, current + 1);
    persistence_.init_next(node, next);

    roots_.push_back(persistence_.root());
}

void SortedList::remove(int key) {
    const int current = current_version();

    Node* prev = persistence_.root();
    Node* target = Persistence::read(prev, current);
    while (target != nullptr && target->key < key) {
        prev = target;
        target = Persistence::read(target, current);
    }

    if (target != nullptr && target->key == key) {
        persistence_.write(prev, Persistence::read(target, current), current + 1);
    }

    roots_.push_back(persistence_.root());
}

int SortedList::successor(int key, int version) const {
    const int v = normalize(version);
    for (const Node* n = Persistence::read(roots_[v], v); n != nullptr; n = Persistence::read(n, v)) {
        if (n->key > key) return n->key;
    }
    return -1;
}

std::string SortedList::print(int version) const {
    const int v = normalize(version);
    std::string out;
    for (const Node* n = Persistence::read(roots_[v], v); n != nullptr; n = Persistence::read(n, v)) {
        if (!out.empty()) out += ", ";
        out += std::to_string(n->key);
    }
    return out;
}

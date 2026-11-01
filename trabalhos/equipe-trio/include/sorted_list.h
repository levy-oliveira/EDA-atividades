#pragma once

#include <string>
#include <vector>

#include "persistence.h"

class SortedList {
public:
    SortedList();

    int current_version() const;
    int node_count() const;

    void insert(int key);
    void remove(int key);

    int successor(int key, int version) const;
    std::string print(int version) const;

private:
    int normalize(int version) const;

    Persistence persistence_;
    std::vector<Node*> roots_;
};

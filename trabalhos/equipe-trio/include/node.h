#pragma once

const int MAX_MODS = 2;

struct Node;

struct Mod {
    int version;
    Node* value;
};

struct Node {
    int key;
    Node* next;
    Node* back;
    int mod_count;
    Mod mods[MAX_MODS];
};

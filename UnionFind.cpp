// UnionFind.cpp
#include "UnionFind.h"

UnionFind::UnionFind(int max_size) : size(0), max_size(max_size) {
    elements = new Node*[max_size + 1];
    for (int i = 0; i <= max_size; i++) {
        elements[i] = nullptr;
    }
}

UnionFind::~UnionFind() {
    for (int i = 0; i <= max_size; i++) {
        delete elements[i];
    }
    delete[] elements;
}

void UnionFind::make_set(int id) {
    if (elements[id] != nullptr) return;

    elements[id] = new Node(id);
    elements[id]->parent = elements[id];
    size++;
}

int UnionFind::find(int id) {
    if (elements[id] == nullptr) return -1;

    Node* current = elements[id];
    Node* root = current;

    // מציאת השורש
    while (root != root->parent) {
        root = root->parent;
    }

    // דחיסת מסלול
    while (current != root) {
        Node* next = current->parent;
        current->parent = root;
        current = next;
    }

    return root->id;
}

void UnionFind::union_sets(int id1, int id2) {
    int root1 = find(id1);
    int root2 = find(id2);

    if (root1 == root2 || root1 == -1 || root2 == -1) return;

    Node* node1 = elements[root1];
    Node* node2 = elements[root2];

    // איחוד לפי rank
    if (node1->rank > node2->rank) {
        node2->parent = node1;
        node1->size += node2->size;
    }
    else if (node1->rank < node2->rank) {
        node1->parent = node2;
        node2->size += node1->size;
    }
    else {
        node2->parent = node1;
        node1->size += node2->size;
        node1->rank++;
    }
}

bool UnionFind::are_same_set(int id1, int id2) {
    return find(id1) == find(id2);
}

int UnionFind::get_size(int id) {
    int root = find(id);
    if (root == -1) return 0;
    return elements[root]->size;
}
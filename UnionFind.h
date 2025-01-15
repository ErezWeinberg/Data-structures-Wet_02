
#ifndef UNIONFIND_H
#define UNIONFIND_H
#endif //UNIONFIND_H



class UnionFind {
private:
    class Node {
    public:
        int id;
        Node* parent;
        int size;
        int rank;

        Node(int id) : id(id), parent(nullptr), size(1), rank(0) {}
    };

    Node** elements;
    int size;
    int max_size;

public:
    UnionFind(int max_size = 100000);
    ~UnionFind();

    void make_set(int id);
    int find(int id);
    void union_sets(int id1, int id2);
    bool are_same_set(int id1, int id2);
    int get_size(int id);
};

#endif //UNIONFIND_H
// HashMap.h
#ifndef HASHMAP_H
#define HASHMAP_H

template<typename K, typename V>
class HashMap {
private:
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int size;
    int capacity;
    const static int INITIAL_SIZE = 10007; // מספר ראשוני

    int hash(const K& key) const {
        return (key % capacity + capacity) % capacity;
    }

    void resize() {
        int old_capacity = capacity;
        Node** old_table = table;

        capacity = capacity * 2 + 1;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }

        size = 0;
        for (int i = 0; i < old_capacity; i++) {
            Node* current = old_table[i];
            while (current != nullptr) {
                insert(current->key, current->value);
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }

        delete[] old_table;
    }

public:
    HashMap() : size(0), capacity(INITIAL_SIZE) {
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    void insert(const K& key, const V& value) {
        if (size >= capacity * 0.75) {
            resize();
        }

        int index = hash(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        Node* new_node = new Node(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        size++;
    }

    V* get(const K& key) {
        int index = hash(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        return nullptr;
    }

    bool contains(const K& key) {
        return get(key) != nullptr;
    }

    void remove(const K& key) {
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    int getSize() const {
        return size;
    }
};

#endif //HASHMAP_H
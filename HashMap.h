#pragma once

#include <functional>
#include <memory>
#include <cmath>

#include "List.h"

using namespace std;

// Define a structure for each bucket in the hash table
template<typename ValueType>
struct HashBucket {
    int key;
    List<ValueType*> values; // Changed from List<shared_ptr<ValueType>>
    bool occupied;

    HashBucket() : key(0), occupied(false) {} // 'occupied' indicates if 'key' is initialized
};

// HashMap class with integer keys and generic values
template<typename ValueType>
class HashMap {
private:
    HashBucket<ValueType>* buckets; // Array of hash buckets

    int size;
    int capacity;

    static constexpr int INITIAL_CAPACITY = 10007; // Prime number for initial capacity

    // Compute hash index for a given key
    int compute_hash(int key) const;

    // Expand and rehash the table when load factor is exceeded
    void expand_table();

public:
    HashMap();
    ~HashMap();

    // Add a key-value pair to the hash map
    void insert(int key, ValueType* value); // Changed parameter type

    // Retrieve values associated with a key
    ValueType* get_values(int key) const; // Changed return type

    // Check if a key exists in the hash map
    bool contains(int key) const;

    // Remove a specific key-value pair
    void remove_pair(int key, ValueType* value); // Changed parameter type

    // Get the number of key-value pairs in the hash map
    int get_size() const;

    // Check if we have duplicates with the same key
    bool check_duplicates(const int key) const;
};

// Implementations

template<typename ValueType>
HashMap<ValueType>::HashMap() : size(0), capacity(INITIAL_CAPACITY) {
    buckets = new HashBucket<ValueType>[capacity];
}

template<typename ValueType>
HashMap<ValueType>::~HashMap() {
    delete[] buckets;
}

template<typename ValueType>
int HashMap<ValueType>::compute_hash(int key) const {
    // Ensures that negative keys are handled correctly by mapping them within [0, capacity-1]
    return ((key % capacity) + capacity) % capacity;
}

template<typename ValueType>
void HashMap<ValueType>::expand_table() {
    int old_capacity = capacity;
    capacity *= 2;
    HashBucket<ValueType>* new_buckets = new HashBucket<ValueType>[capacity];

    for (int i = 0; i < old_capacity; ++i) {
        if (buckets[i].occupied) {
            for (auto& valPtr : buckets[i].values) {
                int new_index = compute_hash(buckets[i].key);
                if (!new_buckets[new_index].occupied) {
                    new_buckets[new_index].key = buckets[i].key;
                    new_buckets[new_index].occupied = true;
                }
                new_buckets[new_index].values.push_back(valPtr);
            }
        }
    }

    delete[] buckets;
    buckets = new_buckets;
}

template<typename ValueType>
void HashMap<ValueType>::insert(int key, ValueType* value) { // Changed parameter type
    if (static_cast<float>(size) / capacity > 0.75f) {
        expand_table();
    }

    int index = compute_hash(key);
    if (buckets[index].occupied && buckets[index].key == key) {
        buckets[index].values.push_back(value);
        size++;
    } else {
        buckets[index].key = key;
        buckets[index].values.push_back(value);
        buckets[index].occupied = true;
        size++;
    }
}

template<typename ValueType>
ValueType* HashMap<ValueType>::get_values(int key) const { // Changed return type
    int index = compute_hash(key);
    if (buckets[index].occupied && buckets[index].key == key) {
        if (buckets[index].values.size() == 1) { // Check if only one entry exists
            return *buckets[index].values.begin(); // Return the first ValueType* if exists
        }
    }
    return nullptr; // Return nullptr if not exactly one entry found
}

template<typename ValueType>
bool HashMap<ValueType>::contains(int key) const {
    int index = compute_hash(key);
    return buckets[index].occupied && buckets[index].key == key;
}

template<typename ValueType>
void HashMap<ValueType>::remove_pair(int key, ValueType* value) { // Changed parameter type
    int index = compute_hash(key);
    if (buckets[index].occupied) {
        buckets[index].values.remove(value);
        if (buckets[index].values.empty()) {
            buckets[index].occupied = false;
            size--;
        }
    }
}

template<typename ValueType>
int HashMap<ValueType>::get_size() const {
    return size;
}

template<typename ValueType>
bool HashMap<ValueType>::check_duplicates(const int key) const {
    int index = compute_hash(key);
    return buckets[index].values.size() > 1;
}
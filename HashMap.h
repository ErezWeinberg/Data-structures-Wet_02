#pragma once

#include <functional>
#include <memory>
#include <cmath>

#include "List.h"

using namespace std;

template<typename ValueType>
struct HashNode {
    int m_key;
    ValueType* m_value; 

    HashNode() : m_key(0), m_value(nullptr) {} 

    bool operator==(const HashNode<ValueType>& other) const {
        return m_key == other.m_key && m_value == other.m_value;
    }
};


// HashMap class with integer keys and generic values
template<typename ValueType>
class HashMap {
private:
    List<HashNode<ValueType>>* m_buckets; 

    int m_size;
    int m_capacity;

    static constexpr int INITIAL_CAPACITY = 10007; // Prime number for initial capacity

    // Compute hash index for a given key
    int compute_hash(int key) const;

    // Expand and rehash the table when load factor is exceeded
    void expand_table();

public:

    // Constructor and destructor
    HashMap();

    ~HashMap();

    // Add a key-value pair to the hash map
    void insert(int key, ValueType* value); // Ensure ValueType* is used correctly

    // Retrieve values associated with a key
    ValueType* get_value(int key) const;

    // Retrieve values associated with a key
    ValueType* remove_and_get_values(int key);

    // Check if a key exists in the hash map
    bool contains(int key) const;

    // Remove a specific key-value pair
    bool remove_pair(int key, ValueType* value);

    // Get the number of key-value pairs in the hash map
    int get_size() const;

    // Check if we have duplicates with the same key
    bool check_duplicates(const int key) const;

    // Delete all nodes in the hash map
    void delete_all_nodes();
};

// Implementations

template<typename ValueType>
HashMap<ValueType>::HashMap() : m_size(0), m_capacity(INITIAL_CAPACITY) {
    m_buckets = new List<HashNode<ValueType>>[m_capacity];
}

template<typename ValueType>
HashMap<ValueType>::~HashMap() {
    delete[] m_buckets;
}

template<typename ValueType>
int HashMap<ValueType>::compute_hash(int key) const {
    // Ensure positive index within capacity
    return ((key % m_capacity) + m_capacity) % m_capacity;
}

template<typename ValueType>
void HashMap<ValueType>::expand_table() {
    int old_capacity = m_capacity;
    m_capacity *= 2;
    List<HashNode<ValueType>>* new_buckets = new List<HashNode<ValueType>>[m_capacity];

    for (int i = 0; i < old_capacity; ++i) {
        // Traverse the existing bucket
        for(auto& node : m_buckets[i]){
            if(node.m_key != 0 || node.m_value != nullptr){
                int new_index = compute_hash(node.m_key);
                new_buckets[new_index].push_back(node);
            }
        }
    }

    delete[] m_buckets;
    m_buckets = new_buckets;
}

template<typename ValueType>
void HashMap<ValueType>::insert(int key, ValueType* value) {
    if (static_cast<float>(m_size) / m_capacity > 0.75f) {
        expand_table();
    }

    int index = compute_hash(key);
    for(auto& node : m_buckets[index]){
        if(node.m_key == key){
            node.m_value = value;
            return;
        }
    }

    HashNode<ValueType> new_node;
    new_node.m_key = key;
    new_node.m_value = value;
    m_buckets[index].push_back(new_node);
    m_size++;
}

template<typename ValueType>
ValueType* HashMap<ValueType>::get_value(int key) const {
    int index = compute_hash(key);
    for(const auto& node : m_buckets[index]){
        if(node.m_key == key){
            return node.m_value;
        }
    }
    return nullptr;
}

template<typename ValueType>
ValueType* HashMap<ValueType>::remove_and_get_values(int key) {
    int index = compute_hash(key);
    for(auto it = m_buckets[index].begin(); it != m_buckets[index].end(); ++it){
        if((*it).m_key == key){
            ValueType* value = (*it).m_value;
            m_buckets[index].remove(*it);
            m_size--;
            return value;
        }
    }
    return nullptr;
}

template<typename ValueType>
bool HashMap<ValueType>::contains(int key) const {
    int index = compute_hash(key);
    for(const auto& node : m_buckets[index]){
        if(node.m_key == key){
            if(node.m_value != nullptr){
                return true;
            }
            return false;
        }
    }
    return false;
}

template<typename ValueType>
bool HashMap<ValueType>::remove_pair(int key, ValueType* value) {
    int index = compute_hash(key);
    for(auto it = m_buckets[index].begin(); it != m_buckets[index].end(); ++it){
        if((*it).m_key == key && (*it).m_value == value){
            m_buckets[index].remove(*it);
            m_size--;
            return true;
        }
    }
    return false;
}

template<typename ValueType>
int HashMap<ValueType>::get_size() const {
    return m_size;
}

template<typename ValueType>
bool HashMap<ValueType>::check_duplicates(const int key) const {
    int index = compute_hash(key);
    int count = 0;
    for(const auto& node : m_buckets[index]){
        if(node.m_key == key){
            count++;
            if(count > 1){
                return true;
            }
        }
    }
    return false;
}


// template<typename ValueType>
// void HashMap<ValueType>::delete_all_nodes() {
//     for(int i = 0; i < m_capacity; ++i){
//         for(auto& node : m_buckets[i]){
//             if(node.m_value != nullptr){
//                 delete node;
//             }
//         }
//     }
// }
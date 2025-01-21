#ifndef UNIONFIND_H
#define UNIONFIND_H

#include "HashMap.h"
#include <memory>

// K - is the id V - is the Team or Jockey 
// each Team or Jockey has a record and methods to get and update the record
template<typename K, typename T>
class UnionFind {
private:
    class Node {
    public:
        K id;
        std::shared_ptr<Node> parent;
        int m_size;
        int rank;
        std::shared_ptr<T> element;
        std::shared_ptr<Node> next; // Pointer for linked list in m_record_map

        Node(K id, std::shared_ptr<T> element);
    };

    HashMap<K, std::shared_ptr<Node>> m_elements; // Use HashMap for O(1) access by id
    HashMap<int, std::shared_ptr<Node>> m_record_map; // HashMap to maintain records and their nodes (linked list)
    int m_size;
    int m_max_size;

    std::shared_ptr<Node> findRootNode(std::shared_ptr<Node> node);
    bool link(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);
    void add_to_record_map(int record, std::shared_ptr<Node> node);
    void remove_from_record_map(int record);

public:

    UnionFind();
    ~UnionFind() = default;

    void make_set(K id, std::shared_ptr<T> element);
    std::shared_ptr<T> find(K id);
    bool union_sets(K id1, K id2);
    bool are_same_set(K id1, K id2);
    int get_size(K id);
    int get_record(K id);
    bool update_record(K victory_id, K lose_id);
    bool unite_by_record(int record);
    std::shared_ptr<T> find_root(K id);
    std::shared_ptr<T> find_root(std::shared_ptr<Node> node);
};

// Implementation

// Node constructor
template<typename K, typename T>
UnionFind<K, T>::Node::Node(K id, std::shared_ptr<T> element)
    : id(id), parent(this), m_size(1), rank(0), element(element), next(nullptr) {}

// Returns the root std::shared_ptr<Node> (path compression)
template<typename K, typename T>
std::shared_ptr<typename UnionFind<K, T>::Node> UnionFind<K, T>::findRootNode(std::shared_ptr<typename UnionFind<K, T>::Node> node) {
    if (node->parent != node) {
        node->parent = findRootNode(node->parent);
    }
    return node->parent;
}

// Returns the root element as std::shared_ptr<T>
template<typename K, typename T>
std::shared_ptr<T> UnionFind<K, T>::find_root(K id) {
    if (!m_elements.contains(id)) {
        return nullptr;
    }
    std::shared_ptr<Node> node = *m_elements.get(id);
    std::shared_ptr<Node> root = findRootNode(node);
    return root->element;
}
// Returns the root element as std::shared_ptr<T>
template<typename K, typename T>
std::shared_ptr<T> UnionFind<K, T>::find_root(std::shared_ptr<Node> node) {
    if (!m_elements.contains(node->id)) {
        return nullptr;
    }
    std::shared_ptr<Node> root = findRootNode(node);
    if(root == nullptr) return nullptr;
    return root->element;
}

// Link two nodes by rank
template<typename K, typename T>
bool UnionFind<K, T>::link(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) 
{
    if (node1 == nullptr || node2 == nullptr) return false;
    if (node1->size <= node2->size){
        std::shared_ptr<Node> temp = node1;
        node1 = node2;
        node2 = temp;
    }
    node2->parent = node1;
    node1->m_size += node2->m_size;
    node1->element->update_record(node2->element->get_record() - node1->element->get_record());
    remove_from_record_map(node2->element->get_record());
    add_to_record_map(node1->element->get_record(), node1);
    return true;
}

// Add node to record map
template<typename K, typename T>
void UnionFind<K, T>::add_to_record_map(int record, std::shared_ptr<Node> node) {
    std::shared_ptr<Node> head = m_record_map.get(record);
    node->next = head;
    m_record_map.insert(record, node);
}

// Remove node from record map
template<typename K, typename T>
void UnionFind<K, T>::remove_from_record_map(int record) {
    // Implementation to remove a node from the linked list if needed
}

// Constructor
template<typename K, typename T>
UnionFind<K, T>::UnionFind() : m_size(1000), m_max_size(m_max_size), m_elements(), m_record_map() {}

// Make set
template<typename K, typename T>
void UnionFind<K, T>::make_set(K id, std::shared_ptr<T> element) {
    if (m_elements.contains(id)) return;

    std::shared_ptr<Node> new_node = std::make_shared<Node>(id, element);
    m_elements.insert(id, ((new_node)->element));
    add_to_record_map(element->get_record(), new_node);
    m_size++;
}

// Find set and return element
template<typename K, typename T>
std::shared_ptr<T> UnionFind<K, T>::find(K id) 
{
    if (!m_elements.contains(id)) return nullptr;
    std::shared_ptr<Node> node = *m_elements.get(id);
    if(node == nullptr) return nullptr;
    return node->element;
}

// Union sets
template<typename K, typename T>
bool UnionFind<K, T>::union_sets(K id1, K id2) 
{
    if (!m_elements.contains(id1) || !m_elements.contains(id2)) {
        return false;
    }
    
    std::shared_ptr<Node> node1 = *m_elements.get(id1);
    std::shared_ptr<Node> node2 = *m_elements.get(id2);

    return link(node1, node2);
}

// Are same set
template<typename K, typename T>
bool UnionFind<K, T>::are_same_set(K id1, K id2) {
    if (!m_elements.contains(id1) || !m_elements.contains(id2)) {
        return false;
    }
    std::shared_ptr<Node> node1 = *m_elements.get(id1);
    std::shared_ptr<Node> node2 = *m_elements.get(id2);
    return findRootNode(node1) == findRootNode(node2);
}

// Get size
template<typename K, typename T>
int UnionFind<K, T>::get_size(K id) {
    if (!m_elements.contains(id)) return 0;
    std::shared_ptr<Node> node = *m_elements.get(id);
    std::shared_ptr<Node> rootNode = findRootNode(node);
    return rootNode->m_size;
}

// Get record
template<typename K, typename T>
int UnionFind<K, T>::get_record(K id) {
    if (!m_elements.contains(id)) return -1;
    std::shared_ptr<Node> node = *m_elements.get(id);
    std::shared_ptr<Node> rootNode = findRootNode(node);
    return rootNode->element->get_record();
}

// Update record
template<typename K, typename T>
bool UnionFind<K, T>::update_record(K victory_id, K lose_id) {
    int root_victory = find(victory_id);
    int root_lose = find(lose_id);

    if (root_victory == -1 || root_lose == -1) return false;

    std::shared_ptr<Node> node_victory = *m_elements.get(root_victory);
    std::shared_ptr<Node> node_lose = *m_elements.get(root_lose);

    remove_from_record_map(node_victory->element->get_record());
    remove_from_record_map(node_lose->element->get_record());

    node_victory->element->update_record(true);
    node_lose->element->update_record(false);

    add_to_record_map(node_victory->element->get_record(), node_victory);
    add_to_record_map(node_lose->element->get_record(), node_lose);

    return true;
}

// Unite by record
template<typename K, typename T>
bool UnionFind<K, T>::unite_by_record(int record) 
{
    auto positive_node = m_record_map.get(record);
    auto negative_node = m_record_map.get(-record);

    positive_node = dynamic_cast<Node*>(positive_node);
    negative_node = dynamic_cast<Node*>(negative_node);
    
    if (positive_node && negative_node) {
        return union_sets(positive_node->id, negative_node->id);   
    }    
    return false;
}

#endif // UNIONFIND_H

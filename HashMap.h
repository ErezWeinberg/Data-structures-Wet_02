#ifndef HASHMAP_H
#define HASHMAP_H

#include <cmath>
#include <memory>

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i <= std::sqrt(n); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

template<typename K, typename V>
struct Bucket {
    K key;
    std::shared_ptr<V> value;
    bool occupied;
    ~Bucket() = default;
};

template<typename K, typename V>
class HashMap {
private:
    Bucket<K, V>* buckets;
    int m_size;
    int m_capacity;
    const static int INITIAL_SIZE = 10007; // מספר ראשוני

    int hash(const K& m_key) const;
    void resize();

public:
    HashMap();
    ~HashMap();

    void insert(const K& m_key, std::shared_ptr<V> m_value);
    std::shared_ptr<V> get(const K& m_key);
    bool contains(const K& m_key);
    void remove(const K& m_key);
    int getSize() const;
};

// Implementations

template<typename K, typename V>
int HashMap<K, V>::hash(const K& m_key) const {
    return (m_key % m_capacity + m_capacity) % m_capacity;
}

template<typename K, typename V>
void HashMap<K, V>::resize() {
    int old_capacity = m_capacity;
    Bucket<K, V>* old_buckets = buckets;

    // שמירה על קיבולת ראשונית
    m_capacity = m_capacity * 2 + 1;
    while (!isPrime(m_capacity)) {
        m_capacity += 1;
    }

    buckets = new Bucket<K, V>[m_capacity]();
    m_size = 0;
    for (int i = 0; i < old_capacity; ++i) {
        if (old_buckets[i].occupied) {
            insert(old_buckets[i].key, old_buckets[i].value);
        }
    }
    delete[] old_buckets;
}

template<typename K, typename V>
HashMap<K, V>::HashMap() : m_size(0), m_capacity(INITIAL_SIZE) {
    buckets = new Bucket<K, V>[m_capacity]();
}

template<typename K, typename V>
HashMap<K, V>::~HashMap() {
    delete[] buckets;
}

template<typename K, typename V>
void HashMap<K, V>::insert(const K& m_key, std::shared_ptr<V> m_value) {
    if (m_size >= m_capacity * 0.75) {
        resize();
    }

    int index = hash(m_key);
    while (buckets[index].occupied) {
        if (buckets[index].key == m_key) {
            buckets[index].value = m_value;
            return;
        }
        index = (index + 1) % m_capacity;
    }

    buckets[index].key = m_key;
    buckets[index].value = m_value;
    buckets[index].occupied = true;
    m_size++;
}

template<typename K, typename V>
std::shared_ptr<V> HashMap<K, V>::get(const K& m_key) {
    int index = hash(m_key);
    while (buckets[index].occupied) {
        if (buckets[index].key == m_key) {
            return buckets[index].value;
        }
        index = (index + 1) % m_capacity;
    }
    return nullptr;
}

template<typename K, typename V>
bool HashMap<K, V>::contains(const K& m_key) {
    int index = hash(m_key);
    while (buckets[index].occupied) {
        if (buckets[index].key == m_key) {
            return true;
        }
        index = (index + 1) % m_capacity;
    }
    return false;
}

template<typename K, typename V>
void HashMap<K, V>::remove(const K& m_key) {
    int index = hash(m_key);
    while (buckets[index].occupied) {
        if (buckets[index].key == m_key) {
            buckets[index].occupied = false;
            m_size--;
            return;
        }
        index = (index + 1) % m_capacity;
    }
}

template<typename K, typename V>
int HashMap<K, V>::getSize() const {
    return m_size;
}

#endif //HASHMAP_H
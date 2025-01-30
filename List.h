#ifndef LIST_H
#define LIST_H


#include <cstddef> // For nullptr

template<typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;

    Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

template<typename T>
class List {
private:
    Node<T>* head;
    Node<T>* tail;
    std::size_t m_size;

public:
    List() : head(nullptr), tail(nullptr), m_size(0) {}

    ~List() {
        Node<T>* current = head;
        while (current) {
            Node<T>* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        m_size++;
    }

    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            head->prev = newNode;
            newNode->next = head;
            head = newNode;
        }
        m_size++;
    }

    void pop_back() {
        if (!tail) return;
        Node<T>* toDelete = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete toDelete;
        m_size--;
    }

    void pop_front() {
        if (!head) return;
        Node<T>* toDelete = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete toDelete;
        m_size--;
    }

    std::size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    void remove(const T& value) {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }

                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }

                delete current;
                m_size--;
                return;
            }
            current = current->next;
        }
    }


    // void remove_by_iterator(Iterator it) {
        
    // }

    // Iterator implementation
    class Iterator {
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* node) : current(node) {}

        T& operator*() const {
            return current->data;
        }

        Iterator& operator++() { // Pre-increment
            if (current) current = current->next;
            return *this;
        }

        Iterator operator++(int) { // Post-increment
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
};

#endif // LIST_H
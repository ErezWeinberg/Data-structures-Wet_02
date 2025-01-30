#pragma once

#include "HashMap.h"
#include "Participant.h"
#include "plains25a2.h"
#include <memory>
#include <iostream>

using namespace std;

// Extracted GenericNode struct outside of UnionFind class
template <typename Jockey, typename Team>
class GenericNode {
private:


public:

    friend class Plains;

    std::shared_ptr<Participant> m_data;  // Pointer to the actual object (Jockey, Team)
    GenericNode* m_parent;                       // Raw pointer to the parent node
    int m_size;                           // Score of the team

    GenericNode(const std::shared_ptr<Participant>& m_data) : m_data(m_data), m_parent(this), m_size(0) {}

    bool operator==(const GenericNode& other) const {
        return m_data->m_id == other.m_data->m_id;
    }

};
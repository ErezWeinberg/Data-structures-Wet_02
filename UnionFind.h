#pragma once

#include "HashMap.h"
#include "Participant.h"
#include <memory>
#include <iostream>

using namespace std;

template <typename Jockey, typename Team>
class UnionFind {
public:
    struct Node {
        shared_ptr<Participant> data;  // Pointer to the actual object (jockey, Team)
        Node* parent;            // Raw pointer to the parent node
        int size;               // Score of the team

        Node(const shared_ptr<Participant>& data) : data(data), parent(nullptr), size(0) {}

        bool operator==(const Node& other) const {
            return data == other.data;
        }
    };

private:
    HashMap<Node> m_jockey_map;  // Hash table of pointers to jockey nodes
    HashMap<Node> m_team_map;    // Hash table of pointers to Team nodes

    Node* find_root(Node* node) const;

public:
    // Constructor: initializes an empty union-find structure
    UnionFind();

    ~UnionFind();

    // Finds the team that a given jockey (by ID) currently belongs to
    shared_ptr<Team> find_jockey_team(int jockeyId) const;

    // Finds the team that a given jockey (by ID) currently belongs to
    Node* find_jockey_team_node(int jockeyId) const;

    // Returns the jockey pointer if present in the structure
    shared_ptr<Jockey> find_jockey(int jockeyId) const;

    // Finds the root team of the given teamId (just the "super-team")
    shared_ptr<Team> find_real_team(int teamId) const;

    // Finds the root team of the given teamId (just the "super-team")
    Node* find_real_team_node(int teamId) const;

    // Finds the root team of the given teamId (or the merged "super-team")
    shared_ptr<Team> find_team(int teamId) const;

    // Unites the set containing a jockey with the set containing a team
    void unite_jockey_with_team(int jockeyId, int teamId);

    // Merges two teams. Child is the team with the lower ranking
    bool merge_teams(int childTeamId, int parentTeamId);

    Node* merge_team_nodes(Node* childTeamNode, Node* parentTeamNode);

    // Adds a new jockey to the union-find structure
    bool add_jockey(const shared_ptr<Jockey>& jockey, const shared_ptr<Team>& team);

    // Adds a new team to the union-find structure
    bool add_team(const shared_ptr<Team>& team);

    // Updates the match between two jockeys
    bool update_match(int victoriousJockeyId, int losingJockeyId);

    // Gets the group of a jockey
    int get_team_jockey(int jockeyId) const;

    // Unites teams by record
    bool unite_by_record(int record);

    // Gets the record of a jockey
    int get_jockey_record(int jockeyId) const;
};

// Implementations

template <typename Jockey, typename Team>
UnionFind<Jockey, Team>::UnionFind() : m_jockey_map(), m_team_map() {
    // The m_jockey_map and m_team_map are initialized using their default constructors
}

template <typename Jockey, typename Team>
UnionFind<Jockey, Team>::~UnionFind() {
    // Properly destroy HashMaps
}

template <typename Jockey, typename Team>
typename UnionFind<Jockey, Team>::Node* UnionFind<Jockey, Team>::find_root(Node* node) const {
    if (!node) {
        return nullptr;
    }
    if (node->parent != node) {
        node = find_root(node->parent); // Path compression
    }
    return node->parent;
}

template <typename Jockey, typename Team>
shared_ptr<Team> UnionFind<Jockey, Team>::find_jockey_team(int jockeyId) const {
    Node* root = find_jockey_team_node(jockeyId);
    if (!root) {
        return nullptr;
    }
    return std::static_pointer_cast<Team>(root->data);
}

template <typename Jockey, typename Team>
typename UnionFind<Jockey, Team>::Node* UnionFind<Jockey, Team>::find_jockey_team_node(int jockeyId) const {
    Node* jockeyNode = m_jockey_map.get_value(jockeyId);
    if (!jockeyNode) {
        return nullptr;
    }
    Node* root = find_root(jockeyNode);
    // The root is a Team node.
    return root;
}

template <typename Jockey, typename Team>
shared_ptr<Jockey> UnionFind<Jockey, Team>::find_jockey(int jockeyId) const {
    Node* jockeyNode = m_jockey_map.get_value(jockeyId);
    if (jockeyNode) {
        return std::static_pointer_cast<Jockey>(jockeyNode->data);
    }
    return nullptr;
}


template <typename Jockey, typename Team>
shared_ptr<Team> UnionFind<Jockey, Team>::find_real_team(int teamId) const {
    Node* teamNode = m_team_map.get_value(teamId);
    if (!teamNode) {
        return nullptr;
    }
    if (teamNode->parent != teamNode) {
        return nullptr;
    }
    return std::static_pointer_cast<Team>(teamNode->data);
}

template <typename Jockey, typename Team>
typename UnionFind<Jockey, Team>::Node* UnionFind<Jockey, Team>::find_real_team_node(int teamId) const
{
    Node* teamNodePtr = m_team_map.get_value(teamId);
    if (!teamNodePtr) {
        return nullptr;
    }
    if (teamNodePtr->parent != teamNodePtr) {
        return nullptr;
    }
    return teamNodePtr;
}


template <typename Jockey, typename Team>
shared_ptr<Team> UnionFind<Jockey, Team>::find_team(int teamId) const {
    Node* teamNode = m_team_map.get_value(teamId);
    if (!teamNode) {
        return nullptr;
    }
    return std::static_pointer_cast<Team>(teamNode->data);
}

template <typename Jockey, typename Team>
void UnionFind<Jockey, Team>::unite_jockey_with_team(int jockeyId, int teamId) {
    Node* jockeyNode = m_jockey_map.get_value(jockeyId);
    Node* teamNode  = m_team_map.get_value(teamId);

    if (jockeyNode && teamNode) {
        Node* rootJockey = find_root(jockeyNode);
        Node* rootTeam  = find_root(teamNode);
        // Attach the jockey's root directly to the team's root
        rootJockey->parent = rootTeam;
        rootTeam->size++;
    }
}

template <typename Jockey, typename Team>
bool UnionFind<Jockey, Team>::merge_teams(int childTeamId, int parentTeamId) {
    Node* parentTeamNode = m_team_map.get_value(parentTeamId);
    Node* childTeamNode = m_team_map.get_value(childTeamId);

    if (!parentTeamNode || !childTeamNode) {
        return false;
    }

    if(parentTeamNode->size < childTeamNode->size) {
        swap(parentTeamId, childTeamId);
    }

    Node* mergedTeam = merge_team_nodes(childTeamNode, parentTeamNode);
    return mergedTeam != nullptr;
}

template <typename Jockey, typename Team>
typename UnionFind<Jockey, Team>::Node* UnionFind<Jockey, Team>::merge_team_nodes(Node* childTeamNode, Node* parentTeamNode) {
    if (!childTeamNode || !parentTeamNode) {
        return nullptr;
    }
    // Find roots of each team
    childTeamNode = find_root(childTeamNode);
    parentTeamNode = find_root(parentTeamNode);

    // If they are already in the same set, do nothing
    if (childTeamNode == parentTeamNode) {
        return nullptr;
    }

    childTeamNode->parent = parentTeamNode;

    parentTeamNode->data->m_record += childTeamNode->data->m_record;

    return parentTeamNode;
}

template <typename Jockey, typename Team>
bool UnionFind<Jockey, Team>::add_jockey(const shared_ptr<Jockey>& jockey, const shared_ptr<Team>& team) {
    if (!m_jockey_map.get_value(jockey->getId())) {
        Node* newNode = new (std::nothrow) Node(jockey);  // Create a new node with raw pointer
        if(!newNode) {
            throw std::bad_alloc();
        }
        newNode->parent = newNode;  // Initially, the jockey is its own parent
        m_jockey_map.insert(jockey->getId(), newNode->parent); // Use raw pointer

        // Unite the new jockey with the given team (team is the root)
        unite_jockey_with_team(jockey->getId(), team->getId());

        return true;
    }
    return false;
}

template <typename Jockey, typename Team>
bool UnionFind<Jockey, Team>::add_team(const shared_ptr<Team>& team) {
    if (!m_team_map.get_value(team->getId())) {
        Node* newNode = new (std::nothrow) Node(team);  // Create a new node with raw pointer
        if(!newNode) {
            throw std::bad_alloc();
        }
        newNode->parent = newNode;  // Team is its own root
        m_team_map.insert(team->getId(), newNode->parent); // Use raw pointer
        return true;
    }
    return false;
}

template <typename Jockey, typename Team>
bool UnionFind<Jockey, Team>::update_match(int victoriousJockeyId, int losingJockeyId) {
    Node* winnerNode = m_jockey_map.get_value(victoriousJockeyId);
    Node* loserNode = m_jockey_map.get_value(losingJockeyId);

    if (!winnerNode || !loserNode) {
        return false;
    }

    winnerNode->data->increase_record();
    loserNode->data->decrease_record();

    return true;
}

template <typename Jockey, typename Team>
int UnionFind<Jockey, Team>::get_team_jockey(int jockeyId) const {
    Node* jockeyNode = m_jockey_map.get_value(jockeyId);
    if (!jockeyNode) {
        return -1;
    }
    Node* root = find_root(jockeyNode);
    return root->data->m_id;
}

template <typename Jockey, typename Team>
bool UnionFind<Jockey, Team>::unite_by_record(int record) {
    // Implement the logic to unite teams by record
    return true;
}

template <typename Jockey, typename Team>
int UnionFind<Jockey, Team>::get_jockey_record(int jockeyId) const {
    Node* jockeyNode = m_jockey_map.get_value(jockeyId);
    if (!jockeyNode) {
        return -1;
    }
    return jockeyNode->data->m_record;
}

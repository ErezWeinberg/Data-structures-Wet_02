#ifndef UNIONFIND_H
#define UNIONFIND_H

#include "HashMap.h"
#include "Participant.h"
#include <memory>

using namespace std;

class UnionFind {
private:
    class Node {
    public:
        int id;
        int record;
        bool isTeam;
        Node* parent;
        shared_ptr<Participant> participant;

        Node(int id, bool isTeam) : id(id), record(0), isTeam(isTeam), parent(nullptr), participant(nullptr) {}
        
        Node* findRoot() {
            if (parent == nullptr) {
                return this;
            }
            parent = parent->findRoot();
            return parent;
        }
    };

    HashMap<Node> teamMap;
    HashMap<Node> jockeyMap;

    int size;
    int maxSize;

    Node* findRoot(Node* node);
    void linkNodes(Node* node1, Node* node2);

    friend class Plains;

public:
    UnionFind();
    ~UnionFind();

    void addTeam(int id);
    bool addJockey(int jockeyId, int teamId);
    bool updateMatch(int winnerId, int loserId);
    bool teamsExist(int teamId1, int teamId2);
    bool mergeTeams(int teamId1, int teamId2);
    bool uniteByRecord(int record);
    Node* getTeamNode(int teamId);
    int getTeamOfJockey(int jockeyId);
    int getJockeyRecord(int jockeyId);
    Node* getJockeyNode(int jockeyId);
    int fetchJockeyRecord(int jockeyId);
    int fetchTeamRecord(int teamId);
};

// Implementations

UnionFind::UnionFind() : teamMap(), jockeyMap(), size(0), maxSize(0) {}

UnionFind::~UnionFind() {
    for (int i = 0; i < teamMap.get_size(); ++i) {
        Node* node = teamMap.get_values(i);
        delete node;
    }
    for (int i = 0; i < jockeyMap.get_size(); ++i) {
        Node* node = jockeyMap.get_values(i);
        delete node;
    }
}

UnionFind::Node* UnionFind::findRoot(Node* node) {
    if (node->parent == nullptr) {
        return node;
    }
    node->parent = findRoot(node->parent);
    return node->parent;
}

void UnionFind::addTeam(int id) {
    if (!teamMap.contains(id)) {
        Node* newTeam = new Node(id, true);
        teamMap.insert(id, newTeam);
    }
}

bool UnionFind::addJockey(int jockeyId, int teamId) {
    if (!teamMap.contains(teamId) || jockeyMap.contains(jockeyId)) {
        return false;
    }
    Node* teamNode = teamMap.get_values(teamId);
    Node* newJockey = new Node(jockeyId, false);
    jockeyMap.insert(jockeyId, newJockey);
    linkNodes(findRoot(teamNode), findRoot(newJockey));
    return true;
}

bool UnionFind::updateMatch(int winnerId, int loserId) {
    Node* winner = getJockeyNode(winnerId);
    Node* loser = getJockeyNode(loserId);
    if (!winner || !loser) return false;

    Node* rootWin = findRoot(winner);
    Node* rootLose = findRoot(loser);
    if (rootWin == rootLose) return false;

    winner->record += 1;
    loser->record -= 1;

    if (rootWin->isTeam) {
        rootWin->record += 1;
    }
    if (rootLose->isTeam) {
        rootLose->record -= 1;
    }

    return true;
}

bool UnionFind::teamsExist(int teamId1, int teamId2) {
    return teamMap.contains(teamId1) && teamMap.contains(teamId2);
}

bool UnionFind::mergeTeams(int teamId1, int teamId2) {
    Node* t1 = getTeamNode(teamId1);
    Node* t2 = getTeamNode(teamId2);
    if (!t1 || !t2) return false;
    Node* r1 = findRoot(t1);
    Node* r2 = findRoot(t2);
    if (r1 == r2) return false;
    linkNodes(r1, r2);
    return true;
}

bool UnionFind::uniteByRecord(int record) {
    // Implementation as needed
    return false;
}

UnionFind::Node* UnionFind::getTeamNode(int teamId) {
    return teamMap.contains(teamId) ? teamMap.get_values(teamId) : nullptr;
}

int UnionFind::getTeamOfJockey(int jockeyId) {
    Node* jockey = getJockeyNode(jockeyId);
    if (!jockey) return -1;
    Node* root = findRoot(jockey);
    return root->isTeam ? root->id : -1;
}

int UnionFind::getJockeyRecord(int jockeyId) {
    Node* jockey = getJockeyNode(jockeyId);
    return jockey ? jockey->record : 0;
}

UnionFind::Node* UnionFind::getJockeyNode(int jockeyId) {
    return jockeyMap.contains(jockeyId) ? jockeyMap.get_values(jockeyId) : nullptr;
}

int UnionFind::fetchJockeyRecord(int jockeyId) {
    Node* jockey = getJockeyNode(jockeyId);
    return jockey ? jockey->record : 0;
}

int UnionFind::fetchTeamRecord(int teamId) {
    Node* team = getTeamNode(teamId);
    return team ? findRoot(team)->record : 0;
}

void UnionFind::linkNodes(Node* rootA, Node* rootB) {
    if (rootA->id < rootB->id) {
        rootB->parent = rootA;
        if (rootA->isTeam && rootB->isTeam) {
            rootA->record += rootB->record;
            rootB->record = 0; 
        }
    } else {
        rootA->parent = rootB;
        if (rootA->isTeam && rootB->isTeam) {
            rootB->record += rootA->record;
            rootA->record = 0; 
        }
    }
}

#endif // UNIONFIND_H

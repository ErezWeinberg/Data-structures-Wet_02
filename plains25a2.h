// 
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef PLAINS25A2_H
#define PLAINS25A2_H

#include "wet2util.h"
#include "HashMap.h"
#include "GenericNode.h"
#include "Participant.h"

class Plains {
private:

    HashMap<GenericNode<Jockey, Team>> m_team_map;
    HashMap<GenericNode<Jockey, Team>> m_jockey_map;
    HashMap<GenericNode<Jockey, Team>> m_record_map;

    // Finds the root team of the given teamId (just the "super-team")
    GenericNode<Jockey, Team>* find_real_team_node(int teamId) const
    {
        GenericNode<Jockey, Team>* teamNodePtr = m_team_map.get_value(teamId);
        if (!teamNodePtr) {
            return nullptr;
        }
        if (teamNodePtr->m_parent != teamNodePtr) {
            return nullptr;
        }
        return teamNodePtr;
    }

    GenericNode<Jockey, Team>* find_root(GenericNode<Jockey, Team>* node) const {
        if (!node) {
            return nullptr;
        }
        if (node->m_parent != node) {
            node = find_root(node->m_parent); // Path compression
        }
        return node->m_parent;
    }


public:
    // <DO-NOT-MODIFY> {-----------------
    Plains();
    ~Plains();
    StatusType add_team(int teamId);
    StatusType add_jockey(int jockeyId, int teamId);
    StatusType update_match(int victoriousJockeyId, int losingJockeyId);
    StatusType merge_teams(int teamId1, int teamId2);
    StatusType unite_by_record(int record);
    output_t<int> get_jockey_record(int jockeyId);
    output_t<int> get_team_record(int teamId);
    // } </DO-NOT-MODIFY>---------------
};

#endif // PLAINS25A2_H
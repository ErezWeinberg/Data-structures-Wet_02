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
#include "UnionFind.h"
#include "team.h"
#include "jockey.h"

class Plains {
private:
    HashMap<int, Team*>* teams;        // מיפוי מזהה קבוצה לקבוצה
    HashMap<int, Jockey*>* jockeys;    // מיפוי מזהה רוכב לרוכב
    UnionFind* teams_union;            // מבנה Union-Find לניהול איחוד קבוצות
    int total_teams;                   // מספר כולל של קבוצות שנוספו (כולל אלו שאוחדו)
    int total_jockeys;
    
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

#endif // PLAINS25WINTER_WET1_H_

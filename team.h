// team.h
#ifndef TEAM_H
#define TEAM_H

#include "HashMap.h"
#include "jockey.h"

class Team {
private:
    int team_id;
    HashMap<int, Jockey*>* jockeys; // מיפוי מזהה רוכב לרוכב
    int total_record; // סכום המאזנים של כל הרוכבים

public:
    Team(int id);
    ~Team();
    
    // getters
    int getId() const;
    int getRecord() const;
    int getNumJockeys() const;
    
    // פעולות על רוכבים
    void addJockey(Jockey* jockey);
    void removeJockey(Jockey* jockey);
    bool hasJockey(int jockey_id) const;
    void updateRecord(int jockey_id, bool won);
    
    // operators
    bool operator==(const Team& other) const;
    bool operator<(const Team& other) const;
};

#endif //TEAM_H
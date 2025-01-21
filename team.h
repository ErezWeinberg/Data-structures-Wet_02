// team.h
#ifndef TEAM_H
#define TEAM_H

#include "HashMap.h"
#include "jockey.h"

#include <memory>


class Team {
private:
    int m_team_id;
    int m_total_record; // סכום המאזנים של כל הרוכבים
    HashMap<int, Jockey>* m_jockeys; // מיפוי מזהה רוכב לרוכב

public:
    Team(int id);
    ~Team();
    
    // getters
    int get_id() const;
    int get_record() const;
    int get_num_jockeys() const;
    
    // פעולות על רוכבים
    void add_jockey(std::shared_ptr<Jockey> jockey);
    void remove_jockey(Jockey* jockey);
    bool has_jockey(int jockey_id) const;
    void update_record(int jockey_id, bool won);
    void update_record(int record);
    
    // operators
    bool operator==(const Team& other) const;
    bool operator<(const Team& other) const;
};

#endif //TEAM_H
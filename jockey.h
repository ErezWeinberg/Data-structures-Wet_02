// jockey.h
#ifndef JOCKEY_H
#define JOCKEY_H

class Team; // forward declaration

class Jockey {
private:
    int jockey_id;
    Team* team;
    int record; // מאזן אישי (ניצחונות - הפסדים)

public:
    Jockey(int id, Team* t = nullptr);
    ~Jockey() = default;
    
    // getters
    int getId() const;
    Team* getTeam() const;
    int getRecord() const;
    
    // setters
    void setTeam(Team* new_team);
    
    // עדכון המאזן
    void updateRecord(bool won); // true לניצחון, false להפסד
    
    // operators
    bool operator==(const Jockey& other) const;
    bool operator<(const Jockey& other) const;
};

#endif //JOCKEY_H
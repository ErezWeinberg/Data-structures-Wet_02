// jockey.h
#ifndef JOCKEY_H
#define JOCKEY_H

#include <memory>

class Team; // forward declaration

class Jockey {
private:
    int m_jockey_id;
    std::shared_ptr<Team> m_team;
    int m_record; // מאזן אישי (ניצחונות - הפסדים)

public:
    Jockey(int id, std::shared_ptr<Team> t = nullptr);
    ~Jockey() = default;
    
    // getters
    int get_id() const;
    std::shared_ptr<Team> get_team() const;
    int get_record() const;
    
    // setters
    void set_team(std::shared_ptr<Team> new_team);
    
    // עדכון המאזן
    void update_record(bool won); // true לניצחון, false להפסד
    
    // operators
    bool operator==(const Jockey& other) const;
    bool operator<(const Jockey& other) const;
};

#endif //JOCKEY_H
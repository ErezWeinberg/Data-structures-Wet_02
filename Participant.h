#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <memory>

class Participant {
protected:
public:

    int m_id;
    int m_record;

    Participant(int id) : m_id(id), m_record(0) {}
    virtual ~Participant() = default;


    // setters
    void increase_record() { m_record++; }  
    void decrease_record() { m_record--; }
    void reset_record() { m_record = 0; }

    friend class UnionFind;

};

class Jockey : public Participant {
private:
    Jockey(int id) : Participant(id) {}
};

class Team : public Participant {
private:
    Team(int id) : Participant(id) {}
};

#endif //PARTICIPANT_H


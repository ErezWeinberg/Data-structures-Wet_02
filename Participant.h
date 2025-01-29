#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <memory>

template <typename Jockey, typename Team>
class UnionFind;

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

    int getId() const { return m_id; }

    bool operator==(const Participant& other) const {
        return m_id == other.m_id;
    }

    template <typename Jockey, typename Team>
    friend class UnionFind;
};

class Jockey : public Participant {
public:
    Jockey(int id) : Participant(id) {}
};

class Team : public Participant {
public:
    Team(int id) : Participant(id) {}
};

#endif //PARTICIPANT_H


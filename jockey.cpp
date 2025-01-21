// jockey.cpp
#include "jockey.h"
#include "team.h"

Jockey::Jockey(int id, std::shared_ptr<Team> t) :
    m_jockey_id(id),
    m_team(t),
    m_record(0) {}

int Jockey::get_id() const {
    return m_jockey_id;
}

std::shared_ptr<Team> Jockey::get_team() const {
    return m_team;
}

int Jockey::get_record() const {
    return m_record;
}

void Jockey::set_team(std::shared_ptr<Team> new_team) {
    m_team = new_team;
}

void Jockey::update_record(bool won) {
    if (won) {
        m_record++;
    } else {
        m_record--;
    }
}

bool Jockey::operator==(const Jockey& other) const {
    return m_jockey_id == other.m_jockey_id;
}

bool Jockey::operator<(const Jockey& other) const {
    return m_jockey_id < other.m_jockey_id;
}
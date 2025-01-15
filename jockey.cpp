// jockey.cpp
#include "jockey.h"
#include "team.h"

Jockey::Jockey(int id, Team* t) :
    jockey_id(id),
    team(t),
    record(0) {}

int Jockey::getId() const {
    return jockey_id;
}

Team* Jockey::getTeam() const {
    return team;
}

int Jockey::getRecord() const {
    return record;
}

void Jockey::setTeam(Team* new_team) {
    team = new_team;
}

void Jockey::updateRecord(bool won) {
    if (won) {
        record++;
    } else {
        record--;
    }
}

bool Jockey::operator==(const Jockey& other) const {
    return jockey_id == other.jockey_id;
}

bool Jockey::operator<(const Jockey& other) const {
    return jockey_id < other.jockey_id;
}
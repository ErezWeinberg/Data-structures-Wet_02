// team.cpp
#include "team.h"

Team::Team(int id) :
    team_id(id),
    jockeys(new HashMap<int, Jockey*>()),
    total_record(0) {}

Team::~Team() {
    delete jockeys;
}

int Team::getId() const {
    return team_id;
}

int Team::getRecord() const {
    return total_record;
}

int Team::getNumJockeys() const {
    return jockeys->getSize();
}

void Team::addJockey(Jockey* jockey) {
    if (jockey == nullptr) return;

    jockeys->insert(jockey->getId(), jockey);
    total_record += jockey->getRecord();
    jockey->setTeam(this);
}

void Team::removeJockey(Jockey* jockey) {
    if (jockey == nullptr) return;

    if (jockeys->contains(jockey->getId())) {
        total_record -= jockey->getRecord();
        jockeys->remove(jockey->getId());
        jockey->setTeam(nullptr);
    }
}

bool Team::hasJockey(int jockey_id) const {
    return jockeys->contains(jockey_id);
}

void Team::updateRecord(int jockey_id, bool won) {
    Jockey** jockey_ptr = jockeys->get(jockey_id);
    if (jockey_ptr != nullptr) {
        (*jockey_ptr)->updateRecord(won);
        if (won) {
            total_record++;
        } else {
            total_record--;
        }
    }
}

bool Team::operator==(const Team& other) const {
    return team_id == other.team_id;
}

bool Team::operator<(const Team& other) const {
    return team_id < other.team_id;
}

// team.cpp
#include "team.h"

Team::Team(int id) :
    m_team_id(id),
    m_total_record(0) 
{
    m_jockeys = new HashMap<int, Jockey>();
}

Team::~Team() {
    delete m_jockeys;
}

int Team::get_id() const {
    return m_team_id;
}

int Team::get_record() const {
    return m_total_record;
}

int Team::get_num_jockeys() const {
    return m_jockeys->getSize();
}

void Team::add_jockey(std::shared_ptr<Jockey> jockey) {
    if (jockey == nullptr) return;

    m_jockeys->insert(jockey->get_id(), jockey);
    m_total_record += jockey->get_record();
    jockey->set_team(std::make_shared<Team>(*this));
}

void Team::remove_jockey(Jockey* jockey) {
    if (jockey == nullptr) return;

    if (m_jockeys->contains(jockey->get_id())) {
        m_total_record -= jockey->get_record();
        m_jockeys->remove(jockey->get_id());
        jockey->set_team(nullptr);
    }
}

bool Team::has_jockey(int jockey_id) const {
    return m_jockeys->contains(jockey_id);
}

void Team::update_record(int jockey_id, bool won) {
    std::shared_ptr<Jockey> jockey_ptr = m_jockeys->get(jockey_id);
    if (jockey_ptr != nullptr) {
        (jockey_ptr)->update_record(won);
        if (won) {
            m_total_record++;
        } else {
            m_total_record--;
        }
    }
}

void Team::update_record(int record) {
    m_total_record = record;
}

bool Team::operator==(const Team& other) const {
    return m_team_id == other.m_team_id;
}

bool Team::operator<(const Team& other) const {
    return m_team_id < other.m_team_id;
}

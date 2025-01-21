// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"

// סעיף 1
Plains::Plains() {
    // אתחול טבלת גיבוב לשמירת הרוכבים
    m_jockeys = new HashMap<int, Jockey>();
    // אתחול מבנה Union-Find לניהול איחוד קבוצות
    m_teams_union = new UnionFind<int, Team>();
    // אתחול מונה הקבוצות הכולל (כולל אלו שאוחדו)
    m_total_teams = 0;
    // אתחול מונה הרוכבים הכולל
    m_total_jockeys = 0;
};
// הדסטרוקטור
Plains::~Plains() {
    delete m_jockeys;
    delete m_teams_union;
}


StatusType Plains::add_team(int team_id) {
    // בדיקת תקינות קלט
    if (team_id <= 0) {
        return StatusType::INVALID_INPUT;
    }
    // בדיקה אם הקבוצה כבר קיימת
    if (m_teams_union->find(team_id) == nullptr) {
        return StatusType::FAILURE;
    }
    try {
        // יצירת קבוצה חדשה
        std::shared_ptr<Team> new_team = std::make_shared<Team>(team_id);
        // הוספה למבנה Union-Find
        m_teams_union->make_set(team_id, new_team);
        // עדכון מונה הקבוצות
        m_total_teams++;
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_jockey(int jockey_id, int team_id) {
    if (jockey_id <= 0 || team_id <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (m_jockeys->contains(jockey_id) || m_teams_union->find(team_id) == nullptr) {
        return StatusType::FAILURE;
    }

    try {
        std::shared_ptr<Team> team = m_teams_union->find(team_id);
        std::shared_ptr<Jockey> new_jockey = std::make_shared<Jockey>(jockey_id, team); // יצירת רוכב חדש 
        m_jockeys->insert(jockey_id, new_jockey);
        team->add_jockey(new_jockey);
        m_total_jockeys++;
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::update_match(int victorious_jockey_id, int losing_jockey_id) {
    if (victorious_jockey_id <= 0 || losing_jockey_id <= 0 ||
        victorious_jockey_id == losing_jockey_id) {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Jockey> victor_ptr = m_jockeys->get(victorious_jockey_id);
    std::shared_ptr<Jockey> loser_ptr = m_jockeys->get(losing_jockey_id);

    if (victor_ptr == nullptr || loser_ptr == nullptr) {
        return StatusType::FAILURE;
    }

    // בדיקה שהרוכבים לא באותה קבוצה
    if (m_teams_union->find(victor_ptr->get_team()->get_id()) ==
        m_teams_union->find(loser_ptr->get_team()->get_id())) {
        return StatusType::FAILURE;
    }

    try {
        victor_ptr->update_record(true);  // עדכון ניצחון
        loser_ptr->update_record(false);  // עדכון הפסד
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::merge_teams(int team_id1, int team_id2) {
    if (team_id1 <= 0 || team_id2 <= 0 || team_id1 == team_id2) {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Team> team1_ptr = m_teams_union->find(team_id1);
    std::shared_ptr<Team> team2_ptr = m_teams_union->find(team_id2);

    if (team1_ptr == nullptr || team2_ptr == nullptr) {
        return StatusType::FAILURE;
    }

    try {
        // עדכון הקבוצה הגדולה במספר רוכבים
        m_teams_union->union_sets(team_id1, team_id2);

        return StatusType::SUCCESS;

    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    return m_teams_union->unite_by_record(record) ?
           StatusType::SUCCESS : StatusType::FAILURE;    

}


output_t<int> Plains::get_jockey_record(int jockey_id) {
    if (jockey_id <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    std::shared_ptr<Jockey> jockey_ptr = m_jockeys->get(jockey_id);

    if (jockey_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    return output_t<int>((jockey_ptr)->get_record());
}

output_t<int> Plains::get_team_record(int team_id) {
    if (team_id <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    std::shared_ptr<Team> team_ptr = m_teams_union->find(team_id);

    if (team_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    return output_t<int>((team_ptr)->get_record());
}
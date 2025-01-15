// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"

// סעיף 1
plains_t::plains() {
    // אתחול טבלת גיבוב לשמירת הקבוצות
    teams = new HashMap<int, Team*>();
    // אתחול טבלת גיבוב לשמירת הרוכבים
    jockeys = new HashMap<int, Jockey*>();
    // אתחול מבנה Union-Find לניהול איחוד קבוצות
    teams_union = new UnionFind();
    // אתחול מונה הקבוצות הכולל (כולל אלו שאוחדו)
    total_teams = 0;
    // אתחול מונה הרוכבים הכולל
    total_jockeys = 0;
}
// הדסטרוקטור
plains::~plains() {
    // שחרור הזיכרון של כל הקבוצות
    if (teams != nullptr) {
        // שחרור כל אובייקטי Team* בטבלת הגיבוב
        for (const auto& pair : *teams) {
            delete pair.second;  // שחרור ה-Team עצמו
        }
        delete teams;  // שחרור טבלת הגיבוב עצמה
        teams = nullptr;
    }
    // שחרור הזיכרון של כל הרוכבים
    if (jockeys != nullptr) {
        // שחרור כל אובייקטי Jockey* בטבלת הגיבוב
        for (const auto& pair : *jockeys) {
            delete pair.second;  // שחרור ה-Jockey עצמו
        }
        delete jockeys;  // שחרור טבלת הגיבוב עצמה
        jockeys = nullptr;
    }
    // שחרור מבנה ה-Union-Find
    if (teams_union != nullptr) {
        delete teams_union;
        teams_union = nullptr;
    }
    // איפוס המונים
    total_teams = 0;
    total_jockeys = 0;
}


StatusType Plains::add_team(int teamId) {
    // בדיקת תקינות קלט
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    // בדיקה אם הקבוצה כבר קיימת
    if (teams->contains(teamId)) {
        return StatusType::FAILURE;
    }
    try {
        // יצירת קבוצה חדשה
        Team* new_team = new Team(teamId);
        // הוספה לטבלת הגיבוב
        teams->insert(teamId, new_team);
        // הוספה למבנה Union-Find
        teams_union->make_set(teamId);
        // עדכון מונה הקבוצות
        total_teams++;
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (jockeys->contains(jockeyId) || !teams->contains(teamId)) {
        return StatusType::FAILURE;
    }

    try {
        Team* team = *(teams->get(teamId));
        Jockey* new_jockey = new Jockey(jockeyId, team);
        jockeys->insert(jockeyId, new_jockey);
        team->addJockey(new_jockey);
        total_jockeys++;
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 ||
        victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
        }

    Jockey** victor_ptr = jockeys->get(victoriousJockeyId);
    Jockey** loser_ptr = jockeys->get(losingJockeyId);

    if (victor_ptr == nullptr || loser_ptr == nullptr) {
        return StatusType::FAILURE;
    }

    Jockey* victor = *victor_ptr;
    Jockey* loser = *loser_ptr;

    // בדיקה שהרוכבים לא באותה קבוצה
    if (teams_union->find(victor->getTeam()->getId()) ==
        teams_union->find(loser->getTeam()->getId())) {
        return StatusType::FAILURE;
        }

    try {
        victor->updateRecord(true);  // עדכון ניצחון
        loser->updateRecord(false);  // עדכון הפסד
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::merge_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    Team** team1_ptr = teams->get(teamId1);
    Team** team2_ptr = teams->get(teamId2);

    if (team1_ptr == nullptr || team2_ptr == nullptr) {
        return StatusType::FAILURE;
    }

    Team* team1 = *team1_ptr;
    Team* team2 = *team2_ptr;

    try {
        // בחירת המזהה לפי המאזן הטוב יותר
        int new_id;
        if (team1->getRecord() >= team2->getRecord()) {
            new_id = teamId1;
        } else {
            new_id = teamId2;
        }

        teams_union->union_sets(teamId1, teamId2);
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Team* positive_team = nullptr;
    Team* negative_team = nullptr;
    int positive_id = -1;
    int negative_id = -1;

    // חיפוש שתי קבוצות עם מאזנים מנוגדים
    for (const auto& pair : *teams) {
        Team* team = pair.second;
        if (team->getRecord() == record && positive_team == nullptr) {
            positive_team = team;
            positive_id = team->getId();
        } else if (team->getRecord() == -record && negative_team == nullptr) {
            negative_team = team;
            negative_id = team->getId();
        }
    }

    if (positive_team == nullptr || negative_team == nullptr) {
        return StatusType::FAILURE;
    }

    return merge_teams(positive_id, negative_id);
}


output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Jockey** jockey_ptr = jockeys->get(jockeyId);
    if (jockey_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    return output_t<int>((*jockey_ptr)->getRecord());
}

output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Team** team_ptr = teams->get(teamId);
    if (team_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    return output_t<int>((*team_ptr)->getRecord());
}

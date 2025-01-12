// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"

// סעיף 1
plains_t::plains_t() {
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
plains_t::~plains_t() {
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

StatusType Plains::add_jockey(int jockeyId, int teamId)
{
    return StatusType::FAILURE;
}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId)
{
    return StatusType::FAILURE;
}

StatusType Plains::merge_teams(int teamId1, int teamId2)
{
    return StatusType::FAILURE;
}

StatusType Plains::unite_by_record(int record)
{
    return StatusType::FAILURE;
}

output_t<int> Plains::get_jockey_record(int jockeyId)
{
    return 0;
}

output_t<int> Plains::get_team_record(int teamId)
{
    return 0;
}

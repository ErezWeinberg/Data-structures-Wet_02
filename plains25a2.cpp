// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"

// Remove any UnionFind definitions that might be here and keep only Plains methods.

Plains::Plains() : m_unionFind(), m_recordMap() {
    // Initialize m_recordMap if necessary
}

Plains::~Plains() {
    // ...existing code...
}

// Add a new team. Initialize its record to 0 in m_recordMap
StatusType Plains::add_team(int teamId) {
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    // Check existence
    if (m_unionFind.getTeamNode(teamId) || m_recordMap.contains(teamId)) {
        return StatusType::FAILURE;
    }
    m_unionFind.addTeam(teamId);
    // Add to m_recordMap with initial record 0
    UnionFind::Node* newTeamNode = new UnionFind::Node(teamId, true);
    m_recordMap.insert(teamId, newTeamNode);
    return StatusType::SUCCESS;
}

// Add a jockey with ID jockeyId, link to teamId
StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;
    if (!m_unionFind.addJockey(jockeyId, teamId)) {
        return StatusType::FAILURE; 
    }
    return StatusType::SUCCESS;
}

// Update match results and manage team records via m_recordMap
StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 
        || victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_unionFind.updateMatch(victoriousJockeyId, losingJockeyId)) {
        return StatusType::FAILURE;
    }
    // Fetch the teams of both jockeys
    int teamWin = m_unionFind.getTeamOfJockey(victoriousJockeyId);
    int teamLose = m_unionFind.getTeamOfJockey(losingJockeyId);
    
    // Update team records
    UnionFind::Node* teamWinNode = m_recordMap.get_values(teamWin);
    UnionFind::Node* teamLoseNode = m_recordMap.get_values(teamLose);
    if (teamWinNode) {
        teamWinNode->record += 1;
    }
    if (teamLoseNode) {
        teamLoseNode->record -= 1;
    }
    return StatusType::SUCCESS;
}

// Merge two teams and manage their records via m_recordMap
StatusType Plains::merge_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_unionFind.teamsExist(teamId1, teamId2)) {
        return StatusType::FAILURE;
    }
    // Determine the new team ID based on records
    UnionFind::Node* team1 = m_recordMap.get_values(teamId1);
    UnionFind::Node* team2 = m_recordMap.get_values(teamId2);
    if (!team1 || !team2) return StatusType::FAILURE;

    int record1 = team1->record;
    int record2 = team2->record;
    int newTeamId = (record1 > record2) ? teamId1 : (record2 > record1) ? teamId2 : teamId1;
    int mergedTeamId = (newTeamId == teamId1) ? teamId2 : teamId1;
    
    // Merge teams in UnionFind
    if (!m_unionFind.mergeTeams(teamId1, teamId2)) {
        return StatusType::FAILURE;
    }
    
    // Reset the merged team's record to 0 and remove it from m_recordMap
    m_recordMap.remove_pair(mergedTeamId, m_recordMap.get_values(mergedTeamId));
    return StatusType::SUCCESS;
}

// If exactly one team with record=record and exactly one with record=-record, merge them
StatusType Plains::unite_by_record(int record) {
    if (record <= 0) return StatusType::INVALID_INPUT;
    if (!m_unionFind.uniteByRecord(record)) {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

// Fetch and return a jockey's record with modified logic
output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    // Validate jockey existence
    if (m_unionFind.getJockeyNode(jockeyId) == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    int recordValue = m_unionFind.getJockeyRecord(jockeyId);
    return output_t<int>(recordValue);
}

// get the record for a team
output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    // Team must exist
    UnionFind::Node* teamNode = m_recordMap.get_values(teamId);
    if (!teamNode) {
        return output_t<int>(StatusType::FAILURE);
    }
    int record = teamNode->record;
    return output_t<int>(record);
}

// ...existing code...



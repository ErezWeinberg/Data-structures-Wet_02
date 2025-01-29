#include "plains25a2.h"
#include "UnionFind.h"
#include <cassert>

Plains::Plains() : m_unionFind(), m_record_map() {}

// Releases the data structure (all allocated memory must be freed).
// Parameters: none
// Return value: none
// Time complexity: O(n + m) in the worst case.
Plains::~Plains() {
    m_unionFind.~UnionFind();
    m_record_map.delate_all_nodes();
}


// Adds a new team with no riders to the data structure.

// Parameters:
// teamId: the ID of the team to be added.

// Return value:
// • ALLOCATION_ERROR in case of a memory allocation/release problem.
// • INVALID_INPUT if teamId <= 0.
// • FAILURE if a team with ID teamId has already been successfully added in the past.
// • SUCCESS on success.
// Time complexity: O(1) on average over the expected input.
StatusType Plains::add_team(int teamId){
    try{
        if(teamId <= 0){
            return StatusType::INVALID_INPUT;
        }

        shared_ptr<Team> new_team = make_shared<Team>(teamId);

        if(m_unionFind.add_team(new_team)){
            // Add to m_record_map with initial record 0
            UnionFind<Jockey, Team>::Node* newTeamNode = new UnionFind<Jockey, Team>::Node(new_team);
            m_record_map.insert(0, newTeamNode);
            return StatusType::SUCCESS;
        }else{
            return StatusType::FAILURE;
        }
    }catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

// A rider with a unique ID jockeyId joins team teamId. The rider starts with a zero record (0).

// Parameters:
// • jockeyId: the ID of the rider to add.
// • teamId: the rider’s team ID.

// Return value:
// • ALLOCATION_ERROR if there is a memory allocation/release problem.
// • INVALID_INPUT if jockeyId <= 0 or teamId <= 0.
// • FAILURE if a rider with ID jockeyId already exists or if there is no existing team with ID teamId.
// • SUCCESS on success.
// Time complexity: O(1) on average over the expected input.
StatusType Plains::add_jockey(int jockeyId, int teamId){
    try{
        if(jockeyId <= 0 || teamId <= 0){
            return StatusType::INVALID_INPUT;
        }
        shared_ptr<Team> team_ptr = m_unionFind.find_team(teamId);
        if(team_ptr == nullptr){
            return StatusType::FAILURE;
        }
        if(m_unionFind.find_jockey(jockeyId) != nullptr){
            return StatusType::FAILURE;
        }
        if(m_unionFind.add_jockey(make_shared<Jockey>(jockeyId), team_ptr)){
            return StatusType::SUCCESS;
        }else{
            return StatusType::FAILURE;
        }
    }catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

// After a match between riders from two different teams, we want to update that the rider with ID victoriousJockeyId won against the rider with ID losingJockeyId. After a successful command, the record of the winning rider (victoriousJockeyId) increases by 1, and the record of the losing rider (losingJockeyId) decreases by 1 (records can be negative).

// Parameters:
// • victoriousJockeyId: the ID of the winning rider.
// • losingJockeyId: the ID of the losing rider.

// Return value:
// • ALLOCATION_ERROR if there is a memory allocation/release problem.
// • INVALID_INPUT if victoriousJockeyId <= 0, losingJockeyId <= 0, or victoriousJockeyId == losingJockeyId.
// • FAILURE if there are no riders with IDs losingJockeyId or victoriousJockeyId, or if both riders are in the same team.
// • SUCCESS on success.
// Time complexity: O(log* m) on average over the input evaluated together with merge_teams and unite_by_record.
StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId){
    try{
        if(victoriousJockeyId <= 0 || losingJockeyId <= 0 || victoriousJockeyId == losingJockeyId){
            return StatusType::INVALID_INPUT;
        }
        if(m_unionFind.update_match(victoriousJockeyId, losingJockeyId))
        {
            // Fetch the teams of both jockeys
            int teamWin = m_unionFind.get_team_jockey(victoriousJockeyId);
            int teamLose = m_unionFind.get_team_jockey(losingJockeyId);
            // Check if the jockeys are in the same team
            if(teamWin == teamLose || teamWin == -1 || teamLose == -1){
                return StatusType::FAILURE;
            }
            // Update team records
            UnionFind<Jockey, Team>::Node* teamWinNode = m_record_map.remove_and_get_values(teamWin);
            UnionFind<Jockey, Team>::Node* teamLoseNode = m_record_map.remove_and_get_values(teamLose);
            // update the records of the teams
            if (teamWinNode && teamLoseNode) {
                // Update the records of the teams
                teamWinNode->data->increase_record();
                teamLoseNode->data->decrease_record();
                // Reinsert the nodes to the record map
                m_record_map.insert(teamWinNode->data->m_record, teamWinNode); 
                m_record_map.insert(teamLoseNode->data->m_record, teamLoseNode);               
            }
            assert(teamWinNode && teamLoseNode); // Both teams should exist
            if (teamLoseNode) {
            }
            return StatusType::SUCCESS;
        }else{
            return StatusType::FAILURE;
        }
    }catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}


// The teams with IDs teamId1 and teamId2 merge (meaning after the merge, all riders from both teams will be in a common team), and the new ID of the merged team is the team that has the better win-loss record (in case of a tie, the new ID is teamId1). After the merge, for example, if ultimately the new team ID is teamId1, then there is no longer a team with ID teamId2, and it won’t be possible to add a new team in the future with that ID.

// Parameters:
// • teamId1: the first team’s ID.
// • teamId2: the second team’s ID.

// Return value:
// • ALLOCATION_ERROR if there is a memory allocation/release problem.
// • INVALID_INPUT if teamId1 <= 0, teamId2 <= 0, or teamId1 == teamId2.
// • FAILURE if there are no teams with ID teamId1 or teamId2.
// • SUCCESS on success.
// Time complexity: O(log* m) on average over the input considered together with unite_by_record and update_match.
StatusType Plains::merge_teams(int teamId1, int teamId2){
    try{
        if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
            return StatusType::INVALID_INPUT;
        }

        UnionFind<Jockey, Team>::Node* team_node_ptr1 = m_unionFind.find_real_team_node(teamId1);
        UnionFind<Jockey, Team>::Node* team_node_ptr2 = m_unionFind.find_real_team_node(teamId2);

        if(team_node_ptr1 == nullptr || team_node_ptr2 == nullptr){
            return StatusType::FAILURE;
        }

        int record1 = team_node_ptr1->data->m_record;
        int record2 = team_node_ptr2->data->m_record;

        int bigger_team_id = team_node_ptr1->size > team_node_ptr2->size ? team_node_ptr1->data->m_id : team_node_ptr2->data->m_id;

        if(m_unionFind.merge_teams(teamId1, teamId2))
        {
            // Update the record map
            m_record_map.remove_pair(record1, team_node_ptr1);
            m_record_map.remove_pair(record2, team_node_ptr2);
            
            shared_ptr<Team> team_ptr = m_unionFind.find_real_team(bigger_team_id);
            UnionFind<Jockey, Team>::Node* team_node = new UnionFind<Jockey, Team>::Node(team_ptr);
            m_record_map.insert(team_ptr->m_record, team_node);
            return StatusType::SUCCESS;
        }else{
            return StatusType::FAILURE;
        }
    }catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

// In order to make the league fairer, the league managers want to unite weak teams with strong ones. After running this command, if there are exactly 2 teams such that one has a record of “record” and the other has a record of “-record,” we unite them.
// In more detail: suppose there are exactly 2 teams with IDs teamId1 and teamId2, where teamId1’s record is “record” and teamId2’s record is “-record.” In that case, we merge those two teams, and the new ID will be teamId1 (i.e., the team with the positive record). Similarly to the previous section, after the operation, it will no longer be possible to add a new team with ID teamId2 to the data structure.

// Parameters:
// • record: the record based on which we want to unite.

// Return value:
// • ALLOCATION_ERROR if there is a memory allocation/release problem.
// • INVALID_INPUT if record <= 0.
// • FAILURE if there are no exactly 2 teams with IDs teamId1 and teamId2 having record = record and record = -record, respectively.
// • SUCCESS on success.
// Time complexity: O(log* m) on average over input evaluated together with update_match and merge_teams.
StatusType Plains::unite_by_record(int record)
{
    try{
        if(record <= 0){
            return StatusType::INVALID_INPUT;
        }

        if(m_record_map.check_duplicates(record) || m_record_map.check_duplicates(-record)){
            return StatusType::FAILURE;
        }

        // Find teams with the given record and -record
        UnionFind<Jockey, Team>::Node* team1 = m_record_map.get_value(record);
        UnionFind<Jockey, Team>::Node* team2 = m_record_map.get_value(-record);

        if (!team1 || !team2) {
            return StatusType::FAILURE;
        }

        int big_team_id = team1->size > team2->size ? team1->data->m_id : team2->data->m_id;

        // Merge the teams
        if (m_unionFind.merge_teams(team1->data->m_id, team2->data->m_id)) {
            // Update the record map
            m_record_map.remove_pair(team1->data->m_id, team1);
            m_record_map.remove_pair(team2->data->m_id, team2);
            shared_ptr<Team> team_ptr = m_unionFind.find_real_team(big_team_id);
            UnionFind<Jockey, Team>::Node* team_node = new UnionFind<Jockey, Team>::Node(team_ptr);
            m_record_map.insert(team1->data->m_record, team_node);

            return StatusType::SUCCESS;
        } else {
            return StatusType::FAILURE;
        }
    }catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}


// Returns the record of the rider with ID jockeyId.

// Parameters:
// • jockeyId: the ID of the rider whose record should be returned.

// Return value:
// • ALLOCATION_ERROR in case of memory allocation/release problem.
// • INVALID_INPUT if jockeyId <= 0.
// • FAILURE if there’s no rider with ID jockeyId.
// • SUCCESS if successful, in which case the rider’s record is returned as well.
// Time complexity: O(1) on average over the input.
output_t<int> Plains::get_jockey_record(int jockeyId){
    try{
        if(jockeyId <= 0){
            return output_t<int>(StatusType::INVALID_INPUT);
        }
        int record = m_unionFind.get_jockey_record(jockeyId);
        if(record == -1){
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(record);
    }catch(std::bad_alloc& e){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

// Returns the record of the team with ID teamId, i.e., the sum of the riders’ wins in the team minus the sum of the riders’ losses in the team.

// Parameters:
// • teamId: the ID of the team whose record should be returned.

// Return value:
// • ALLOCATION_ERROR if there is a memory allocation/release problem.
// • INVALID_INPUT if teamId <= 0.
// • FAILURE if there is no team with the given teamId.
// • SUCCESS if successful, in which case the team’s record is also returned.
// Time complexity: O(1) on average over the input.
output_t<int> Plains::get_team_record(int teamId){
    try{
        if(teamId <= 0){
            return output_t<int>(StatusType::INVALID_INPUT);
        }
        shared_ptr<Team> team_ptr = m_unionFind.find_real_team(teamId);
        // Team must exist
        if (team_ptr == nullptr) {
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(team_ptr->m_record);
    }catch(std::bad_alloc& e){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}






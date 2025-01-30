#include "plains25a2.h"
#include "GenericNode.h"
#include <cassert>


Plains::Plains() : m_team_map(), m_jockey_map(), m_record_map() {
}

// Releases the data structure (all allocated memory must be freed).
// Parameters: none
// Return value: none
// Time complexity: O(n + m) in the worst case.
Plains::~Plains() {
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
        if(m_team_map.get_value(teamId) == nullptr){
            shared_ptr<Team> team_ptr = make_shared<Team>(teamId);
            GenericNode<Jockey, Team>* team_node = new GenericNode<Jockey, Team>(team_ptr);
            
            // Set the team node's parent to itself to denote it's a root
            team_node->m_parent = team_node;
            
            m_team_map.insert(teamId, team_node);
            m_record_map.insert(team_ptr->m_record, m_team_map.get_value(teamId));
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
        if(m_jockey_map.get_value(jockeyId) != nullptr || find_real_team_node(teamId) == nullptr){
            return StatusType::FAILURE;
        }
        shared_ptr<Jockey> jockey_ptr = make_shared<Jockey>(jockeyId);
        GenericNode<Jockey, Team>* jockey_node = new GenericNode<Jockey, Team>(jockey_ptr);
        GenericNode<Jockey, Team>* team_node = find_real_team_node(teamId);
        jockey_node->m_parent = team_node;
        team_node->m_size++;
        m_jockey_map.insert(jockeyId, jockey_node);
        return StatusType::SUCCESS;
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
        GenericNode<Jockey, Team>* victorious_jockey_node = m_jockey_map.get_value(victoriousJockeyId);
        GenericNode<Jockey, Team>* losing_jockey_node = m_jockey_map.get_value(losingJockeyId);
        if(victorious_jockey_node == nullptr || losing_jockey_node == nullptr || find_root(victorious_jockey_node) == find_root(losing_jockey_node)){
            return StatusType::FAILURE;
        }
        // Update the records
        victorious_jockey_node->m_data->increase_record();
        losing_jockey_node->m_data->decrease_record();
        GenericNode<Jockey, Team>* victorious_team_node = find_root(victorious_jockey_node);
        GenericNode<Jockey, Team>* losing_team_node = find_root(losing_jockey_node);
        victorious_team_node->m_data->m_record++;
        losing_team_node->m_data->m_record--;
        // Update the record map
        m_record_map.remove_pair(victorious_team_node->m_data->m_record, victorious_team_node);
        m_record_map.remove_pair(losing_team_node->m_data->m_record, losing_team_node);
        m_record_map.insert(victorious_team_node->m_data->m_record, victorious_team_node);
        m_record_map.insert(losing_team_node->m_data->m_record, losing_team_node);
        return StatusType::SUCCESS;
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

        GenericNode<Jockey, Team>* team_node_ptr1 = find_real_team_node(teamId1);
        GenericNode<Jockey, Team>* team_node_ptr2 = find_real_team_node(teamId2);

        if(team_node_ptr1 == nullptr || team_node_ptr2 == nullptr){
            return StatusType::FAILURE;
        }

        if(team_node_ptr1->m_size < team_node_ptr2->m_size){
            std::swap(team_node_ptr1, team_node_ptr2);
            std::swap(teamId1, teamId2);
        }

        team_node_ptr1->m_size += team_node_ptr2->m_size;
        team_node_ptr2->m_parent = team_node_ptr1;

        // Update the record map
        m_record_map.remove_pair(team_node_ptr1->m_data->m_record, team_node_ptr1);
        m_record_map.remove_pair(team_node_ptr2->m_data->m_record, team_node_ptr2);
        team_node_ptr1->m_data->m_record += team_node_ptr2->m_data->m_record;
        m_record_map.insert(team_node_ptr1->m_data->m_record, team_node_ptr1);

        return StatusType::SUCCESS;

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
        GenericNode<Jockey, Team>* team1 = m_record_map.get_value(record);
        GenericNode<Jockey, Team>* team2 = m_record_map.get_value(-record);

        if (!team1 || !team2) {
            return StatusType::FAILURE;
        }

        return merge_teams(team1->m_data->m_id, team2->m_data->m_id);

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
        GenericNode<Jockey, Team>* jockey_node = m_jockey_map.get_value(jockeyId);
        if(jockey_node == nullptr){
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(jockey_node->m_data->m_record);
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
        GenericNode<Jockey, Team>* team_node_ptr = find_real_team_node(teamId);
        // Team must exist
        if (team_node_ptr == nullptr) {
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(team_node_ptr->m_data->m_record);
    }catch(std::bad_alloc& e){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}
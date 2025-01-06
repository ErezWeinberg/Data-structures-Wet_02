// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"


Plains::Plains()
{
    
}

Plains::~Plains()
{
    
}

StatusType Plains::add_team(int teamId)
{
    return StatusType::FAILURE;
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

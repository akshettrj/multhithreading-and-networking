#include "Team.h"

Team* new_team(char name)
{
    Team *team = malloc(sizeof(Team));
    if (team == NULL)
        err_n_die("Failed to create a new team");

    team->name = name;
    team->goals = 0;
    pthread_mutex_init(&team->lock, NULL);
    pthread_cond_init(&team->goal_cond, NULL);

    return team;
}

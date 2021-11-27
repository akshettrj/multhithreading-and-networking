#ifndef __Q2_TEAM_H__
#define __Q2_TEAM_H__

#include "../common.h"

typedef struct Team
{
    char name;
    llint goals;
    pthread_mutex_t lock;
    pthread_cond_t goal_cond;
}
Team;

Team* new_team(char name);

#endif

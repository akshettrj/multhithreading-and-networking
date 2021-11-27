#ifndef __Q2_GOAL_CHANCE_H__
#define __Q2_GOAL_CHANCE_H__

#include "../common.h"

typedef struct GoalChance
{
    pthread_t thread;
    char team;
    llint time;
    double probability;

    bool (*is_successful)(struct GoalChance*);
}
GoalChance;

GoalChance* new_goal_chance_from_input();
void print_goal_chance(GoalChance *gc);

#endif

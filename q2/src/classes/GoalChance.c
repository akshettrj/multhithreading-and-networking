#include "GoalChance.h"

bool is_successful(GoalChance *gc)
{
    double rand_prob = (double)rand() / (double)RAND_MAX;

    return rand_prob <= gc->probability ? true : false;
}

GoalChance* new_goal_chance_from_input()
{
    GoalChance *goal_chance = malloc(sizeof(GoalChance));
    if (goal_chance == NULL)
        err_n_die("Failed to create a new GoalChance");

    scanf(" %c %lld %lf", &goal_chance->team, &goal_chance->time, &goal_chance->probability);

    goal_chance->is_successful = is_successful;

    return goal_chance;
}

void print_goal_chance(GoalChance *gc)
{
    printf("\n");

    printf(COLOR_BLUE "Goal Chance Team: " COLOR_YELLOW "%c" COLOR_RESET "\n", gc->team);
    printf(COLOR_BLUE "Goal Chance Time: " COLOR_YELLOW "%lld" COLOR_RESET "\n", gc->time);
    printf(COLOR_BLUE "Goal Chance Probability: " COLOR_YELLOW "%lf" COLOR_RESET "\n", gc->probability);

    printf("\n");
}

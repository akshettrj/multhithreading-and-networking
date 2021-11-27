#include "Group.h"

Group* new_group_from_input(llint group_num, llint num_people)
{
    Group *group = malloc(sizeof(Group));
    if (group == NULL)
        err_n_die("Failed to create a new Group");

    group->id = group_num;
    group->num_people = num_people;
    group->people = malloc(num_people * sizeof(Person*));
    if (group->people == NULL)
        err_n_die("Failed to create Group->People");

    group->num_not_at_gate = num_people;
    pthread_mutex_init(&group->lock, NULL);

    return group;
}

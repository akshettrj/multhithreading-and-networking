#ifndef __Q2_GROUP_H__
#define __Q2_GROUP_H__

#include "../common.h"
#include "Person.h"

typedef struct Group
{
    llint id;
    llint num_people;
    Person **people;
    llint num_not_at_gate;
    pthread_mutex_t lock;
}
Group;

Group* new_group_from_input(llint group_num, llint num_people);

#endif

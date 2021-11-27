#ifndef __Q2_PERSON_H__
#define __Q2_PERSON_H__

#include "../common.h"
#include "Zone.h"

typedef struct Person
{
    pthread_t thread;
    char name[PERSON_NAME_LEN];
    char team;
    llint patience_time;
    llint group_number;
    llint arrival_time_delay;
    llint goal_threshold;
    Zone *zone;
    bool finished_watching;
}
Person;

Person* new_person_from_input(llint group_number);
void print_person(Person *person);

#endif

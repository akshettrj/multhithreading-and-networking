#ifndef __Q1_LAB_H
#define __Q1_LAB_H

#include "../common.h"
#include "Mentor.h"

typedef struct Lab
{
    llint id;

    char name[LAB_NAME_LEN];
    llint num_mentors;
    llint taship_limit;

    Mentor **mentors;
    pthread_mutex_t lock;

    llint curr_max_taship;
    llint num_mentors_wo_max_taship;

    bool lab_mentors_available;
}
Lab;

Lab* new_lab_from_input(llint id);
void print_lab(Lab *lab);

#endif

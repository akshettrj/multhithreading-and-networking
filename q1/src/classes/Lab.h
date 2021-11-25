#ifndef __Q1_LAB_H
#define __Q1_LAB_H

#include "../common.h"
#include "Mentor.h"

typedef struct Lab
{
    llint id;                   /* Done */

    /* Input Start */
    char name[LAB_NAME_LEN];    /* Done */
    llint num_mentors;          /* Done */
    llint taship_limit;         /* Done */
    /* Input End */

    MentorQueue *mentors;       /* Done */

    // Mentors ki array
    // Max TA ship count amount mentors

} Lab;

Lab* new_lab_from_input(llint id);
void print_lab(Lab *lab);

#endif

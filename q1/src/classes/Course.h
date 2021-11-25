#ifndef __Q1_COURSE_H
#define __Q1_COURSE_H

#include "../common.h"
#include "Mentor.h"
#include "Lab.h"

typedef struct Course
{
    llint id;

    /* Input Start */
    char name[COURSE_NAME_LEN+1];
    double interest_quotient;
    llint tut_slots_limit;
    llint num_labs;
    llint *lab_ids;
    /* Input End */

    pthread_mutex_t lock;               // Initialized in constructor
    pthread_cond_t tut_slots_cond;      // Initialized in constructor
    pthread_cond_t tut_session_cond;    // Initialized in constructor
    llint tut_slots;                    // Initialized in constructor
    MentorNode *ta;                     // Initialized in constructor
    Lab *ta_lab;                        // Initialized in constructor
    bool is_open;                       // Initialized in constructor

} Course;

Course* new_course_from_input(llint id);
void print_course(Course *course);

#endif

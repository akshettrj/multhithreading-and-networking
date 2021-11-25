#ifndef __Q1_STUDENT_H
#define __Q1_STUDENT_H

#include "../common.h"
#include "Course.h"

typedef struct Student
{
    llint id;

    double calibre_quotient;                                    /* Done */
    llint preferences[STUDENT_NUM_PREFERENCES];                 /* Done */
    llint preferences_filling_time;                             /* Done */

    bool (*withdraw_from_course)(struct Student*, Course*);     /* Done */

} Student;

Student* new_student_from_input(llint id);
void print_student(Student* student);

#endif

#ifndef Q1_UTILS_H
#define Q1_UTILS_H

#include "q1.h"

Course* new_course_from_input(ll);
Student* new_student_from_input(ll);
Lab* new_lab_from_input(ll);

Mentor** make_lab_mentors(ll, ll);

// Returns the smaller from a and b
ll min_ll(ll a, ll b);

void update_lab_taship_count(Lab* lab);

bool select_course_permanently(double course_interest_quotient, double student_calibre);

#endif

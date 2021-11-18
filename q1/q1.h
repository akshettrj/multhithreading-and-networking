#ifndef Q1_H
#define Q1_H

#include <stdbool.h>
#include <pthread.h>

typedef long long int ll;

// Define limits
#ifndef MAX_COURSES
#define MAX_COURSES 1000
#endif
#ifndef MAX_STUDENTS
#define MAX_STUDENTS 1000
#endif
#ifndef MAX_LABS
#define MAX_LABS 1000
#endif
#ifndef MAX_MENTORS
#define MAX_MENTORS 1000
#endif

#define COURSE_NAME_LEN 9
#define LAB_NAME_LEN 9
#define STUDENT_NUM_PREFERENCES 3

typedef struct Mentor {
    ll id;
    ll lab_id;
    ll taship_count;
    pthread_mutex_t lock;
} Mentor;

typedef struct Course {
    ll id;
    char name[COURSE_NAME_LEN+1];
    double interest_quotient;
    ll tut_max_slots;
    ll num_labs;
    ll *lab_ids;
    bool registeration_open;
    Mentor *ta;
} Course;

typedef struct Lab {
    ll id;
    char name[LAB_NAME_LEN+1];
    ll num_mentors;
    ll taship_limit;
    ll cur_max_taship_count;
    Mentor** mentors;
    pthread_mutex_t lock;
} Lab;

typedef struct Student {
    ll id;
    double calibre_quotient;
    ll course_preferences[STUDENT_NUM_PREFERENCES];
    ll preferences_filling_time;
} Student;

#endif

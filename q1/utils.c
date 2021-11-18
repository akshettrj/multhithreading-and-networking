#include "q1.h"
#include "utils.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

ll min_ll(ll a, ll b)
{
    return a < b ? a : b;
}

// For Bonus Part
void update_lab_taship_count(Lab *lab)
{
    pthread_mutex_lock(&lab->lock);
    bool cond = true;
    for (ll mentor_num=0; mentor_num<lab->num_mentors; mentor_num++)
    {
        Mentor *mentor = lab->mentors[mentor_num];
        if (mentor->taship_count < lab->cur_max_taship_count)
        {
            cond = false;
            break;
        }
    }
    if (cond == true)
    {
        lab->cur_max_taship_count += 1;
    }
    pthread_mutex_unlock(&lab->lock);
}

Course* new_course_from_input(ll id)
{
    Course* course = (Course*)malloc(sizeof(Course));
    if (course == NULL)
    {
        perror("Initializing Course");
        exit(1);
    }

    course->id = id;
    scanf("%s", course->name);
    scanf("%lf", &course->interest_quotient);
    scanf("%lld", &course->tut_max_slots);
    scanf("%lld", &course->num_labs);
    course->lab_ids = (ll*)malloc(course->num_labs * sizeof(ll));
    if (course->lab_ids == NULL)
    {
        perror("Initializing Lab IDs for Course");
        exit(1);
    }
    for (ll lab_num=0; lab_num<course->num_labs; lab_num++)
    {
        scanf("%lld", &course->lab_ids[lab_num]);
    }
    course->registeration_open = true;
    course->ta = NULL;

    return course;
}

Student* new_student_from_input(ll id)
{
    Student* student = (Student*)malloc(sizeof(Student));
    if (student == NULL)
    {
        perror("Initializing Student");
        exit(1);
    }

    student->id = id;
    scanf("%lf", &student->calibre_quotient);
    for (ll pref_num=0; pref_num<STUDENT_NUM_PREFERENCES; pref_num++)
    {
        scanf("%lld", &student->course_preferences[pref_num]);
    }
    scanf("%lld", &student->preferences_filling_time);

    return student;
}

Lab* new_lab_from_input(ll id)
{
    Lab* lab = (Lab*)malloc(sizeof(Lab));
    if (lab == NULL)
    {
        perror("Initializing Lab");
        exit(1);
    }

    lab->id = id;
    scanf("%s", lab->name);
    scanf("%lld", &lab->num_mentors);
    scanf("%lld", &lab->taship_limit);

    // Initialize the mutex lock
    if (pthread_mutex_init(&lab->lock, NULL) != 0)
    {
        perror("Initializing Lab Lock");
        exit(1);
    }

    lab->mentors = make_lab_mentors(id, lab->num_mentors);
    lab->cur_max_taship_count = 1;

    return lab;
}

Mentor** make_lab_mentors(ll lab_id, ll num_mentors)
{
    Mentor** mentors = (Mentor**)malloc(num_mentors * sizeof(Mentor*));
    if (mentors == NULL)
    {
        perror("Initializing Lab Mentors");
        exit(1);
    }

    for (ll mentor_num=0; mentor_num<num_mentors; mentor_num++)
    {
        Mentor* mentor = (Mentor*)malloc(sizeof(Mentor));
        if (mentor == NULL)
        {
            perror("Initializing Lab Mentor");
            exit(1);
        }

        mentor->id = mentor_num;
        mentor->lab_id = lab_id;
        mentor->taship_count = 0;

        // Initialize the mutex lock
        if (pthread_mutex_init(&mentor->lock, NULL) != 0)
        {
            perror("Initializing Lab Mentor Lock");
            exit(1);
        }

        mentors[mentor_num] = mentor;
    }

    return mentors;
}

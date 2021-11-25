#include "Student.h"
#include "../utils.h"

bool widthraw_from_course(Student *student, Course *course)
{
    // Pobability of accepting the course
    double prob = student->calibre_quotient * course->interest_quotient;
    double random_prob = (double)rand() / (double)RAND_MAX;

    return (random_prob <= prob ? true : false);
}

Student* new_student_from_input(llint id)
{
    Student *student = malloc(sizeof(Student));

    if (student == NULL)
        err_n_die("Failed to create a new student");

    student->id = id;

    scanf("%lf", &student->calibre_quotient);
    for (int pref_num=0; pref_num<STUDENT_NUM_PREFERENCES; pref_num++)
        scanf("%lld", &student->preferences[pref_num]);
    scanf("%lld", &student->preferences_filling_time);

    student->withdraw_from_course = widthraw_from_course;

    return student;
}

void print_student(Student *student)
{
    printf("\n");

    printf(\
            "%sStudent ID: %s%lld%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            student->id,\
            COLOR_RESET\
        );

    printf(\
            "%sStudent Calibre: %s%lf%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            student->calibre_quotient,\
            COLOR_RESET\
        );

    printf(\
            "%sStudent Preferences: %s",\
            COLOR_BLUE,\
            COLOR_GREEN\
        );
    for (int pref_num=0; pref_num<STUDENT_NUM_PREFERENCES; pref_num++)
        printf("%lld ", student->preferences[pref_num]);
    printf("%s\n", COLOR_RESET);

    printf(\
            "%sStudent Preferences Filling Time: %s%lld%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            student->preferences_filling_time,\
            COLOR_RESET\
        );

    printf("\n");
}

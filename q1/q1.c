#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "q1.h"
#include "utils.h"

// Counts
ll num_courses = 0, num_students = 0, num_labs = 0;

Course**    all_courses;
Student**   all_students;
Lab**       all_labs;

// Threads
pthread_t*  course_threads;
pthread_t*  student_threads;

// Thread function for each course
void* course_thread_function(void *arg)
{
    Course* course = (Course*)arg;

    while (course->registeration_open == true)
    {
        for (ll lab_num=0; lab_num<course->num_labs; lab_num++)
        {
            Lab* lab = all_labs[course->lab_ids[lab_num]];
            for (ll mentor_num=0; mentor_num<lab->num_mentors; mentor_num++)
            {
                Mentor* mentor = lab->mentors[mentor_num];

                if (pthread_mutex_trylock(&mentor->lock) != 0) continue;

                if (mentor->taship_count < min_ll(lab->taship_limit, lab->cur_max_taship_count))
                {
                    mentor->taship_count += 1;
                    printf("TA %lld from lab %s has been allocated to course %s for his %lld TA ship\n", mentor->id, lab->name, course->name, mentor->taship_count);
                    update_lab_taship_count(lab); // For bonus
                }
                pthread_mutex_unlock(&mentor->lock);
            }
        }
    }

    return NULL;
}

// Thread function for each student
void* student_thread_function(void *arg)
{
    Student* student = (Student*)arg;
    return NULL;
}

int main()
{
    // Take the total number of entities as input
    scanf("%lld %lld %lld", &num_students, &num_labs, &num_courses);

    all_courses = (Course**)malloc(num_courses*sizeof(Course*));
    all_students = (Student**)malloc(num_students*sizeof(Student*));
    all_labs = (Lab**)malloc(num_labs*sizeof(Lab*));

    course_threads = (pthread_t*)malloc(num_courses*sizeof(pthread_t));
    student_threads = (pthread_t*)malloc(num_students*sizeof(pthread_t));

    if (all_courses == NULL || all_students == NULL || all_labs == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    if (course_threads == NULL || student_threads == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Take inputs for the courses
    for (ll c_num=0; c_num<num_courses; c_num++)
        all_courses[c_num] = new_course_from_input(c_num);

    // Take inputs for the Students
    for (ll s_num=0; s_num<num_students; s_num++)
        all_students[s_num] = new_student_from_input(s_num);

    // Take inputs for the Labs
    for (ll l_num=0; l_num<num_labs; l_num++)
        all_labs[l_num] = new_lab_from_input(l_num);

    for (ll c_num=0; c_num<num_courses; c_num++)
        pthread_create(&course_threads[c_num], NULL, course_thread_function, (void*)all_courses[c_num]);

    for (ll s_num=0; s_num<num_students; s_num++)
        pthread_create(&student_threads[s_num], NULL, student_thread_function, (void*)all_students[s_num]);

    for (ll s_num=0; s_num<num_students; s_num++)
        pthread_join(student_threads[s_num], NULL);

    for (ll c_num=0; c_num<num_courses; c_num++)
        pthread_join(course_threads[c_num], NULL);

    return 0;
}

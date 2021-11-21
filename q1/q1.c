#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "q1.h"
#include "utils.h"

#define DEBUG 1

// Counts
ll num_courses = 0, num_students = 0, num_labs = 0;

Course**    all_courses;
Student**   all_students;
Lab**       all_labs;

// Threads
pthread_t*  course_threads;
pthread_t*  student_threads;

void* course_thread_function(void *arg)
{
    Course* course = (Course*)arg;

#if DEBUG == 1
    printf("Thread for course %s (id: %lld) started\n", course->name, course->id);
#endif

    while (course->registeration_open)
    {
        pthread_mutex_lock(&course->tut_lock);
#if DEBUG == 1
        printf("Lock for course %s acquired by course itself\n", course->name);
#endif
        // Look for TA
        course->ta = NULL;
        for (ll lab_num=0; lab_num<course->num_labs; lab_num++)
        {
            if (course->ta != NULL)
                break;

            ll lab_id = course->lab_ids[lab_num];
            Lab *lab = all_labs[lab_id];
            for (ll mentor_num=0; mentor_num<lab->num_mentors; mentor_num++)
            {
                Mentor *mentor = lab->mentors[mentor_num];
                pthread_mutex_lock(&mentor->lock);

                // Check if Mentor can be made a TA
                bool ta_requirements = mentor->avail;
                ta_requirements = ta_requirements && (mentor->taship_count < lab->taship_limit);
                ta_requirements = ta_requirements && (mentor->taship_count < lab->cur_max_taship_count);
                if (!ta_requirements)
                {
                    pthread_mutex_unlock(&mentor->lock);
                    continue;
                }

                course->ta = mentor;
                mentor->avail = false;
                mentor->taship_count += 1;
                printf("TA %lld from lab %s has been allocated to course %s for his %lldth TA ship\n",\
                        mentor->id,\
                        lab->name,\
                        course->name,\
                        mentor->taship_count\
                      );
                break;
                pthread_mutex_unlock(&mentor->lock);
            }
        }

        ll num_tut_slots = (rand() % course->tut_max_slots) + 1;
        course->tut_slots_avail = num_tut_slots;
        printf("Course %s has been allocated %lld seats\n", course->name, course->tut_slots_avail);
        pthread_mutex_unlock(&course->tut_lock);
        pthread_cond_broadcast(&course->tut_cond);
        sleep(TUTORIAL_DELAY);

    }

    return NULL;
}

void* student_thread_function(void *arg)
{
    Student* student = (Student*)arg;
#if DEBUG == 1
    printf("Thread for student %lld started\n", student->id);
#endif

    sleep(student->preferences_filling_time);
    printf("Student %lld has filled in preferences for course registeration\n", student->id);

    for (ll pref_num=0; pref_num<STUDENT_NUM_PREFERENCES; pref_num++)
    {
        ll course_id = student->course_preferences[pref_num];
        Course* course = all_courses[course_id];

        // Print Simulation Details
        if (pref_num > 0)
        {
            ll last_pref_course_id = student->course_preferences[pref_num-1];
            Course* last_pref_course = all_courses[last_pref_course_id];
            printf("Student %lld has changed current preference from %s (priority %lld) to %s (priority %lld)\n",\
                    student->id,\
                    last_pref_course->name,\
                    pref_num,\
                    course->name,\
                    pref_num+1\
                  );
        }



        pthread_mutex_lock(&course->tut_lock);

#if DEBUG == 1
        printf("Lock for course %s acquired by student number %lld\n", course->name, student->id);
#endif

        while (course->tut_slots_avail == 0 && course->registeration_open)
        {
            pthread_cond_wait(&course->tut_cond, &course->tut_lock);
        }

        if (!course->registeration_open)
        {
            pthread_mutex_unlock(&course->tut_lock);
            continue;
        }

        if (course->tut_slots_avail > 0)
        {
            course->tut_slots_avail = course->tut_slots_avail - 1;
            printf("Student %lld has been allocated a seat in the course %s\n", student->id, course->name);
            sleep(TUTORIAL_DELAY);
        }

    }
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

#if DEBUG == 1
    printf("Took all the inputs\n");
#endif

    for (ll s_num=0; s_num<num_students; s_num++)
        pthread_create(&student_threads[s_num], NULL, student_thread_function, (void*)all_students[s_num]);

    for (ll c_num=0; c_num<num_courses; c_num++)
        pthread_create(&course_threads[c_num], NULL, course_thread_function, (void*)all_courses[c_num]);

    for (ll s_num=0; s_num<num_students; s_num++)
        pthread_join(student_threads[s_num], NULL);

    for (ll c_num=0; c_num<num_courses; c_num++)
        pthread_join(course_threads[c_num], NULL);

    return 0;
}

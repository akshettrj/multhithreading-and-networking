#include "common.h"
#include "utils.h"

#include "classes/Lab.h"
#include "classes/Course.h"
#include "classes/Mentor.h"
#include "classes/Student.h"

Course **course_nodes;
Student **student_nodes;
Lab **lab_nodes;

pthread_t *course_threads;
pthread_t *student_threads;
pthread_t *lab_threads;

/* Course  *course_nodes[MAX_COURSES];
Student *student_nodes[MAX_STUDENTS];
Lab     *lab_nodes[MAX_LABS]; */

/* pthread_t course_threads[MAX_COURSES];
pthread_t student_threads[MAX_STUDENTS];
pthread_t lab_threads[MAX_LABS]; */

void* course_thread_function(void *arg)
{
#if DEBUG >= 2
    llint thread_id = pthread_self();
    printf("%s:%d:%s(): thread_id = %lld\n", __FILE__, __LINE__, __func__, thread_id);
#endif

    Course *course = (Course*)arg;
#if DEBUG >= 2
    printf("%s%lld: Thread for course %s (id:%lld) started%s\n", COLOR_YELLOW, thread_id, course->name, course->id, COLOR_RESET);
#endif

    while (course->is_open)
    {
#if INFO_LEVEL >= 3
        printf(\
                "%s%sCourse %s going to look for another TA for a new tutorial%s\n",\
                COLOR_RED_BOLD,\
                TEXT_UNDERLINE,\
                course->name,\
                COLOR_RESET\
              );
#endif
        sleep(GAP_BETWEEN_TUTORIAL_SESSIONS);

        course->ta = NULL;
        course->ta_lab = NULL;
        bool can_find_ta = false;

        // TODO: Add logic for selecting TA
        for (llint lab_num=0; lab_num<course->num_labs; lab_num++)
        {
            if (course->ta != NULL) break;

            llint lab_id = course->lab_ids[lab_num];
            Lab *lab = lab_nodes[lab_id];

#if INFO_LEVEL >= 2
            printf(\
                    TEXT_UNDERLINE\
                    COLOR_RED_BOLD\
                    "Course %s is looking for TA in %s lab"\
                    COLOR_RESET"\n",\
                    course->name,\
                    lab->name\
                  );
#endif

            pthread_mutex_lock(&lab->lock);
            if (!lab->lab_mentors_available)
            {
                pthread_mutex_unlock(&lab->lock);
                continue;
            }
            pthread_mutex_unlock(&lab->lock);
            for (llint mentor_num=0; mentor_num<lab->num_mentors; mentor_num++)
            {
                Mentor *mentor = lab->mentors[mentor_num];
                if (pthread_mutex_trylock(&mentor->lock) != 0) {
                    /* Some other Course is using this Mentor */
                    continue;
                }
                /* Mentor is locked */
                if (mentor->taships_done != lab->curr_max_taship) {
                    /* Cannot use this mentor */
                    pthread_mutex_unlock(&mentor->lock);
                    continue;
                }

                course->ta = mentor;
                course->ta_lab = lab;
                mentor->taships_done += 1;
                printf(\
                        "%sTA %lld from lab %s has been allocated to course %s for his %lldth TA ship%s\n",\
                        COLOR_BLUE,\
                        course->ta->id,\
                        course->ta_lab->name,\
                        course->name,\
                        course->ta->taships_done,\
                        COLOR_RESET\
                      );

                /* Mentor can be used */
                pthread_mutex_lock(&lab->lock);
                lab->num_mentors_wo_max_taship -= 1;
                if (lab->num_mentors_wo_max_taship == 0)
                {
                    lab->curr_max_taship += 1;
                    lab->num_mentors_wo_max_taship = lab->num_mentors;
                    if (lab->curr_max_taship == lab->taship_limit)
                    {
                        lab->lab_mentors_available = false;
                        printf(\
                                COLOR_BLUE\
                                "Lab %s no longer has students available for TAship"\
                                COLOR_RESET"\n",\
                                lab->name\
                              );
                    }
                }
                pthread_mutex_unlock(&lab->lock);
                break;
            }
        }

        pthread_mutex_lock(&course->lock);

        if (course->ta == NULL)
        {
            if (!can_find_ta)
            {
                course->is_open = false;
                printf(\
                        "%sCourse %s does not have any TA mentors eligible and is removed from course offerings%s\n",\
                        COLOR_CYAN,\
                        course->name,\
                        COLOR_RESET\
                      );
                pthread_cond_broadcast(&course->tut_slots_cond);
                pthread_cond_broadcast(&course->tut_session_cond);
            }
            pthread_mutex_unlock(&course->lock);
            continue;
        }

        llint num_tut_slots = 1 + (rand() % course->tut_slots_limit);
        course->tut_slots = num_tut_slots;

        printf(\
                "Course %s has been allocated %lld seats\n",\
                course->name,\
                course->tut_slots\
              );

        pthread_cond_broadcast(&course->tut_slots_cond);
        pthread_mutex_unlock(&course->lock);

        sleep(TUTORIAL_SEAT_ALLOTMENT_DELAY);

        pthread_mutex_lock(&course->lock);

        printf(\
                "Tutorial has started for course %s with %lld seats filled out of %lld\n",\
                course->name,\
                num_tut_slots - course->tut_slots,\
                num_tut_slots\
              );
        course->tut_slots = 0;

        sleep(TUTORIAL_DURATION);

        pthread_cond_broadcast(&course->tut_session_cond);
        pthread_mutex_unlock(&course->lock);

        pthread_mutex_unlock(&course->ta->lock);
    }

#if DEBUG >= 2
    printf("%s%lld: Thread for course %s (id:%lld) ending%s\n", COLOR_YELLOW, thread_id, course->name, course->id, COLOR_RESET);
#endif
    return NULL;
}

void* student_thread_function(void *arg)
{
#if DEBUG >= 2
    llint thread_id = pthread_self();
    printf("%s:%d:%s(): thread_id = %lld\n", __FILE__, __LINE__, __func__, thread_id);
#endif

    Student *student = (Student*)arg;
#if DEBUG >= 2
    printf("%s%lld: Thread for student %lld started%s\n", COLOR_YELLOW, thread_id, student->id, COLOR_RESET);
#endif

    // Fill the preferences
    sleep(student->preferences_filling_time);
    printf(\
            "%sStudent %lld has filled in preferences for course registeration%s\n",\
            COLOR_YELLOW,\
            student->id,\
            COLOR_RESET\
          );

    for (llint pref_num=0; pref_num<STUDENT_NUM_PREFERENCES; pref_num++)
    {
        llint course_id = student->preferences[pref_num];
        Course *course = course_nodes[course_id];

        if (pref_num > 0)
        {
            llint prev_course_id = student->preferences[pref_num-1];
            Course *prev_course = course_nodes[prev_course_id];
            printf(\
                    "%sStudent %lld has changed current preference from %s (priority %lld) to %s (priority %lld)%s\n",\
                    COLOR_YELLOW,\
                    student->id,\
                    prev_course->name,\
                    pref_num,\
                    course->name,\
                    pref_num+1,\
                    COLOR_RESET\
                  );
        }

        pthread_mutex_lock(&course->lock);

#if INFO_LEVEL >= 2
        printf(\
                "%s%sStudent %lld is trying to register for course %s%s\n",\
                COLOR_YELLOW,\
                TEXT_UNDERLINE,\
                student->id,\
                course->name,\
                COLOR_RESET\
              );
#endif

        while (course->is_open && course->tut_slots == 0)
        {
            pthread_cond_wait(&course->tut_slots_cond, &course->lock);
        }

        if (!course->is_open)
        {
            pthread_mutex_unlock(&course->lock);
            continue;
        }

        course->tut_slots -= 1;
        printf(\
                "%sStudent %lld has been allocated a seat in course %s%s\n",\
                COLOR_CYAN,\
                student->id,\
                course->name,\
                COLOR_RESET\
              );

        pthread_cond_wait(&course->tut_session_cond, &course->lock);
        pthread_mutex_unlock(&course->lock);

        bool withdraw = student->withdraw_from_course(student, course);
        if (withdraw)
        {
            printf(\
                    "%sStudent %lld has withdrawn from course %s%s\n",\
                    COLOR_MAGENTA,\
                    student->id,\
                    course->name,\
                    COLOR_RESET\
                  );
        }
        else
        {
            printf(\
                    "%s%sStudent %lld has selected course %s permanently%s\n",\
                    COLOR_GREEN_BOLD,\
                    TEXT_UNDERLINE,\
                    student->id,\
                    course->name,\
                    COLOR_RESET\
                  );
            return NULL;
        }

    }

    printf(\
            "%s%sStudent %lld couldn't get any of his preferred courses%s\n",\
            COLOR_RED_BOLD,\
            TEXT_UNDERLINE,\
            student->id,\
            COLOR_RESET\
          );

#if DEBUG >= 2
    printf("%s%lld: Thread for student %lld ending%s\n", COLOR_YELLOW, thread_id, student->id, COLOR_RESET);
#endif
    return NULL;
}

/* void* lab_thread_function(void *arg)
{
#if DEBUG >= 2
    llint thread_id = pthread_self();
    printf("%s:%d:%s(): thread_id = %lld\n", __FILE__, __LINE__, __func__, thread_id);
#endif

    Lab *lab = (Lab*)arg;
#if DEBUG >= 2
    printf("%s%lld: Thread for lab %s (id:%lld) started%s\n", COLOR_YELLOW, thread_id, lab->name, lab->id, COLOR_RESET);
#endif

    // sleep(10);

#if DEBUG >= 2
    printf("%s%lld: Thread for lab %s (id:%lld) ending%s\n", COLOR_YELLOW, thread_id, lab->name, lab->id, COLOR_RESET);
#endif
    return NULL;
} */

int main(int argc, char **argv)
{
    llint num_students = 0;
    llint num_labs = 0;
    llint num_courses = 0;

    scanf(\
            "%lld %lld %lld",\
            &num_students,\
            &num_labs,\
            &num_courses\
         );

    course_nodes = (Course**)malloc(num_courses * sizeof(Course*));
    student_nodes = (Student**)malloc(num_students * sizeof(Student*));
    lab_nodes = (Lab**)malloc(num_labs * sizeof(Lab*));

    course_threads = malloc(num_courses * sizeof(pthread_t));
    student_threads = malloc(num_students * sizeof(pthread_t));
    lab_threads = malloc(num_labs * sizeof(pthread_t));

    if (course_nodes == NULL || student_nodes == NULL || lab_nodes == NULL)
        err_n_die("Failed to store all the entities");

    if (course_threads == NULL || student_threads == NULL || lab_threads == NULL)
        err_n_die("Failed to store all the threads");

    for (llint course_num=0; course_num<num_courses; course_num++)
    {
        course_nodes[course_num] = new_course_from_input(course_num);
#if DEBUG >= 2
        print_course(course_nodes[course_num]);
#endif
    }

    for (llint student_num=0; student_num<num_students; student_num++)
    {
        student_nodes[student_num] = new_student_from_input(student_num);
#if DEBUG >= 2
        print_student(student_nodes[student_num]);
#endif
    }

    for (llint lab_num=0; lab_num<num_labs; lab_num++)
    {
        lab_nodes[lab_num] = new_lab_from_input(lab_num);
#if DEBUG >= 2
        print_lab(lab_nodes[lab_num]);
#endif
    }

    for (llint student_num=0; student_num<num_students; student_num++)
    {
        Student *student = student_nodes[student_num];
        if (pthread_create(&student_threads[student_num], NULL, student_thread_function, (void*)student) != 0)
            err_n_die("Failed to create thread for student %lld", student->id);
    }

    for (llint course_num=0; course_num<num_courses; course_num++)
    {
        Course *course = course_nodes[course_num];
        if (pthread_create(&course_threads[course_num], NULL, course_thread_function, (void*)course) != 0)
            err_n_die("Failed to create thread for course %s (id:%lld)", course->name, course->id);
    }

    /* for (llint lab_num=0; lab_num<num_labs; lab_num++)
    {
        Lab *lab = lab_nodes[lab_num];
        if (pthread_create(&lab_threads[lab_num], NULL, lab_thread_function, (void*)lab) != 0)
            err_n_die("Failed to create thread for lab %s (id:%lld)", lab->name, lab->id);
    } */

#if DEBUG >= 2
    printf("%sAll threads started%s\n", COLOR_YELLOW, COLOR_RESET);
#endif

    for (llint student_num=0; student_num<num_students; student_num++)
    {
#if INFO_LEVEL >= 1
        printf("Waiting for thread of student %lld to join back...\n", student_num);
#endif
        if (pthread_join(student_threads[student_num], NULL) != 0)
            err_n_die("Failed to join thread for student %lld", student_nodes[student_num]->id);
    }

    /* for (llint course_num=0; course_num<num_courses; course_num++)
    {
        if (pthread_join(course_threads[course_num], NULL) != 0)
            err_n_die("Failed to join thread for course %s (id:%lld)", course_nodes[course_num]->name, course_nodes[course_num]->id);
    }

    for (llint lab_num=0; lab_num<num_labs; lab_num++)
    {
        if (pthread_join(lab_threads[lab_num], NULL) != 0)
            err_n_die("Failed to join thread for lab %s (id:%lld)", lab_nodes[lab_num]->name, lab_nodes[lab_num]->id);
    } */

    return 0;
}

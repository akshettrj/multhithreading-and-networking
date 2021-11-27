#include "Course.h"
#include "../utils.h"

Course* new_course_from_input(llint id)
{
    Course* course = malloc(sizeof(Course));

    if (course == NULL)
        err_n_die("Failed to create new course");

    course->id = id;

    scanf("%s", course->name);
    scanf("%lf", &course->interest_quotient);
    scanf("%lld", &course->tut_slots_limit);
    scanf("%lld", &course->num_labs);

    course->lab_ids = malloc(course->num_labs * sizeof(llint));
    for (llint lab_num=0; lab_num<course->num_labs; lab_num++)
        scanf("%lld", &course->lab_ids[lab_num]);

    pthread_mutex_init(&course->lock, NULL);
    pthread_cond_init(&course->tut_slots_cond, NULL);
    pthread_cond_init(&course->tut_session_cond, NULL);
    course->tut_slots = 0;
    course->ta = NULL;
    course->ta_lab = NULL;

    if (course->num_labs == 0 || course->tut_slots_limit < 1) {
        course->is_open = false;
        printf(\
                "%sCourse %s does not have any TA mentors eligible and is removed from course offerings%s\n",\
                COLOR_CYAN,\
                course->name,\
                COLOR_RESET\
              );
    }
    else
        course->is_open = true;

    return course;
};

void print_course(Course* course)
{
    printf("\n");

    printf(\
            "%sCoure ID: %s%lld%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            course->id,\
            COLOR_RESET\
          );

    printf(\
            "%sCoure Name: %s%s%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            course->name,\
            COLOR_RESET\
          );

    printf(\
            "%sCoure Interest: %s%lf%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            course->interest_quotient,\
            COLOR_RESET\
          );

    printf(\
            "%sCoure Labs: %s",\
            COLOR_BLUE,\
            COLOR_GREEN\
          );
    for (llint lab_num=0; lab_num<course->num_labs; lab_num++)
        printf("%lld ", course->lab_ids[lab_num]);
    printf("%s\n", COLOR_RESET);

    printf("\n");
}

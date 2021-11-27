#include "Lab.h"
#include "Mentor.h"
#include "../utils.h"

Lab* new_lab_from_input(llint id)
{
    Lab *lab = malloc(sizeof(Lab));

    if (lab == NULL)
        err_n_die("Failed to create a new lab");

    lab->id = id;

    scanf("%s", lab->name);
    scanf("%lld", &lab->num_mentors);
    scanf("%lld", &lab->taship_limit);

/* #if DEBUG >= 1
    printf("Making a new queue for lab %s\n", lab->name);
#endif */

    lab->mentors = malloc(lab->num_mentors * sizeof(Mentor*));
    for (llint mentor_num=0; mentor_num<lab->num_mentors; mentor_num++)
    {
        lab->mentors[mentor_num] = new_mentor(mentor_num, id, lab->taship_limit);
    }
    pthread_mutex_init(&lab->lock, NULL);
    lab->curr_max_taship = 0;
    lab->num_mentors_wo_max_taship = lab->num_mentors;
    lab->lab_mentors_available = true;

    return lab;
}

void print_lab(Lab *lab)
{
    printf("\n");

    printf(\
            "%sLab ID: %s%lld%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            lab->id,\
            COLOR_RESET\
          );

    printf(\
            "%sLab Name: %s%s%s\n",\
            COLOR_BLUE,\
            COLOR_GREEN,\
            lab->name,\
            COLOR_RESET\
          );

    printf(\
            "%sLab Mentors: %s",\
            COLOR_BLUE,\
            COLOR_GREEN\
          );
    printf("%s\n", COLOR_RESET);

    printf("\n");
}

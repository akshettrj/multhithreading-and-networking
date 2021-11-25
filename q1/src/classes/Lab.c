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

    lab->mentors = new_mentor_queue(lab->num_mentors, id);

/* #if DEBUG >= 1
    printf("Successfully made a new queue for lab %s\n", lab->name);
#endif */

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
    print_mentor_queue(lab->mentors);
    printf("%s\n", COLOR_RESET);

    printf("\n");
}

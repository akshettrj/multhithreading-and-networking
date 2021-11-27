#include "Mentor.h"
#include "../utils.h"

Mentor* new_mentor(llint mentor_id, llint lab_id, llint taships)
{
    Mentor *mentor = malloc(sizeof(Mentor));
    if (mentor == NULL)
        err_n_die("Failed to create a new Mentor");

    mentor->id = mentor_id;
    mentor->lab_id = lab_id;
    mentor->taships_done = 0;
    pthread_mutex_init(&mentor->lock, NULL);

    return mentor;
}

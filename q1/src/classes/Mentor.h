#ifndef __Q1_MENTOR_H
#define __Q1_MENTOR_H

#include "../common.h"

typedef struct Mentor
{
    llint id;
    llint lab_id;
    llint taships_done;
    pthread_mutex_t lock;
}
Mentor;

Mentor* new_mentor(llint mentor_id, llint lab_id, llint taships);

#endif

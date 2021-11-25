#ifndef __Q1_MENTOR_H
#define __Q1_MENTOR_H

#include "../common.h"

typedef struct MentorNode
{
    llint id;                           /* Done */
    llint lab_id;                       /* Done */

    struct MentorNode *next;            /* Done */
    struct MentorNode *prev;            /* Done */

    llint taship_count;                 /* Done */
    pthread_mutex_t lock;               /* Done */

} MentorNode;

typedef struct MentorQueue
{
    MentorNode *head;                                            /* Done */
    MentorNode *tail;                                            /* Done */

    /* Current number of mentors in the queue */
    llint size;                                                  /* Done */
    /* The total mentors that may come.
     * Will decrease as the mentors reach the taship limit */
    llint max_size;                                              /* Done */

    pthread_mutex_t lock;                                        /* Done */

    void (*enqueue)(struct MentorQueue*, MentorNode *);          /* Done */
    MentorNode* (*dequeue)(struct MentorQueue *);                /* Done */

} MentorQueue;

MentorQueue* new_mentor_queue(llint num_mentors, llint lab_id);
MentorNode* new_mentor_node(llint mentor_id, llint lab_id);

void print_mentor_node(MentorNode *node);
void print_mentor_queue(MentorQueue *queue);

#endif

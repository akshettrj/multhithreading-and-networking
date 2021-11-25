#include "Mentor.h"
#include "../utils.h"

void enqueue_mentor(MentorQueue* mq, MentorNode* mentor)
{
    // Lock should be acquired by the caller

    if (mentor->taship_count <= 0)
    {
        // If the mentor has no taship, then it is not in the queue
        pthread_mutex_destroy(&mentor->lock);
        free(mentor);
        mq->max_size--;
        return;
    }

    if (mq->head == NULL)
    {
        mq->head = mentor;
        mq->tail = mentor;
        mentor->next = NULL;
        mentor->prev = NULL;
        mq->size += 1;
        return;
    }

    /* else { */
    mq->tail->next = mentor;
    mentor->prev = mq->tail;
    mentor->next = NULL;
    mq->tail = mentor;
    mq->size += 1;
    return;

    // TODO: Mechanism for handling the mentors reaching taship limit
    /* } */
}

MentorNode* dequeue_mentor(MentorQueue *mq)
{
    // Lock should be acquired by the caller

    if (mq->size == 0)
        return NULL;

    /* else { */
    mq->size --;
    MentorNode *mentor = mq->head;
    mq->head = mentor->next;

    if (mq->head != NULL)
        mq->head->prev = NULL;

    mentor->next = NULL;
    mentor->prev = NULL;

    return mentor;
    /* } */
}

MentorQueue* new_mentor_queue(llint num_mentors, llint lab_id)
{
    MentorQueue *m_queue = malloc(sizeof(MentorQueue));

    if (m_queue == NULL)
        err_n_die("Failed to create a mentor queue");

    m_queue->size = 0;
    m_queue->max_size = num_mentors;

    m_queue->head = NULL;
    m_queue->tail = NULL;

    m_queue->enqueue = enqueue_mentor;
    m_queue->dequeue = dequeue_mentor;

    pthread_mutex_init(&m_queue->lock, NULL);

    pthread_mutex_lock(&m_queue->lock);
    for (llint mentor_num=0; mentor_num<num_mentors; mentor_num++)
    {
        MentorNode *mentor = new_mentor_node(mentor_num, lab_id);
        pthread_mutex_lock(&mentor->lock);
        m_queue->enqueue(m_queue, mentor);
        pthread_mutex_unlock(&mentor->lock);
    }
    pthread_mutex_unlock(&m_queue->lock);

    return m_queue;
};

MentorNode* new_mentor_node(llint mentor_id, llint lab_id)
{
    MentorNode *mentor = malloc(sizeof(MentorNode));

    if (mentor == NULL)
        err_n_die("Failed to create a mentor node");

    mentor->id = mentor_id;
    mentor->lab_id = lab_id;
    mentor->next = NULL;
    mentor->prev = NULL;
    mentor->taship_count = 1;

    pthread_mutex_init(&mentor->lock, NULL);

    return mentor;
}

void print_mentor_queue(MentorQueue *mq)
{
    MentorNode *mentor = mq->head;
    while (mentor != NULL)
    {
        print_mentor_node(mentor);

        if (mentor->next != NULL)
            printf("->");

        mentor = mentor->next;
    }
}

void print_mentor_node(MentorNode *mentor)
{
    printf("(%lld,%lld,%lld)", mentor->id, mentor->lab_id, mentor->taship_count);
}

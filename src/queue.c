#include "queue.h"
#include <malloc.h>

#define QUEUE_DEFAULT_SIZE 10

void queue_init(queue_t *q)
{
    q->s = QUEUE_DEFAULT_SIZE;
    q->f = -1;
    q->b = -1;
    q->e = (double *) malloc(q->s * sizeof (double));
}

void queue_enqueue(queue_t *q, double val)
{
    q->b++;

    if (q->b+1 == q->s) {
        q->s *= 2;
        q->e = (double *) realloc(q->e, q->s * sizeof (double));
    }

    if (q->f == (size_t)-1) {
        q->f = 0;
    }

    q->e[q->b] = val;
}

double queue_dequeue(queue_t *q)
{
    if (queue_is_empty(q)) {
        exit(-10);
    }

    return q->e[q->f++];
}

void queue_free(queue_t *q)
{
    free(q->e);
}

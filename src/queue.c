#include "queue.h"
#include <malloc.h>

#define QUEUE_DEFAULT_SIZE 10

void queue_init(queue_t *q)
{
    q->s = QUEUE_DEFAULT_SIZE;
    q->f = -1;
    q->b = -1;
    q->e = (token_t *) malloc(q->s * sizeof (token_t));
}

void queue_enqueue(queue_t *q, token_t val)
{
    q->b++;

    if (q->b+1 == q->s) {
        q->s *= 2;
        q->e = (token_t *) realloc(q->e, q->s * sizeof (token_t));
    }

    if (q->f == (size_t)-1) {
        q->f = 0;
    }

    q->e[q->b] = val;
}

token_t queue_dequeue(queue_t *q)
{
    if (queue_is_empty(q)) {
        return (token_t) {.t = T_NO_TOKEN, .c = -1};
    }

    return q->e[q->f++];
}

void queue_free(queue_t *q)
{
    free(q->e);
}

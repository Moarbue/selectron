#include "queue.h"
#include <malloc.h>

queue_t queue_init(void)
{
    queue_t q;

    q.c = QUEUE_INITIAL_CAPACITY;
    q.f = -1;
    q.b = -1;
    q.e = (_token *) malloc(q.c * sizeof (_token));
    if (q.e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for queue!"));

    return q;
}

void queue_enqueue(queue_t *q, _token val)
{
    if (++q->b >= q->c) {
        q->c *= 2;
        q->e = (_token *) realloc(q->e, q->c * sizeof (_token));
        
        if (q->e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for queue!"));
    }

    if (q->f == (size_t)-1) q->f = 0;

    q->e[q->b] = val;
}

_token queue_dequeue(queue_t *q)
{
    if (queue_is_empty(*q)) exit_on_error(error(ERROR_QUEUE_EMPTY, "Queue is empty"));

    return q->e[q->f++];
}

void queue_free(queue_t *q)
{
    free(q->e);
}

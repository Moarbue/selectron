#include "queue.h"
#include <malloc.h>

#define QUEUE_DEFAULT_SIZE 10

void queue_init(queue_t *q)
{
    q->s = 0;
    q->f = -1;
    q->b = -1;
    q->e = NULL;
}

error_t queue_enqueue(queue_t *q, token_t val)
{
    q->b++;

    if (q->b == q->s) {
        if (q->e == NULL) {
            q->s = QUEUE_DEFAULT_SIZE;
            q->e = (token_t *) malloc((q->s+1) * sizeof (token_t));
        } else {
            q->s *= 2;
            q->e = (token_t *) realloc(q->e, (q->s+1) * sizeof (token_t));
        }
        
        if (q->e == NULL)
            return error(ERROR_NO_MEMORY, "Failed to allocate memory for queue!");
    }

    if (q->f == (size_t)-1) {
        q->f = 0;
    }

    q->e[q->b] = val;

    return error(ERROR_NO_ERROR, NULL);
}

token_t queue_dequeue(queue_t *q)
{
    if (queue_is_empty(q)) {
        return (token_t){0};
    }

    return q->e[q->f++];
}

void queue_free(queue_t *q)
{
    free(q->e);
}

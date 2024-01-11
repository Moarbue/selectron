#include <stddef.h>
#include "tokenizer.h"

#ifndef QUEUE_H_
#define QUEUE_H_

#define QUEUE_INITIAL_CAPACITY 16
#define queue_is_empty(q) ((q).f == (size_t)-1 || (q).f > (q).b)

typedef struct {
    size_t c;   // capacity
    size_t f;   // front
    size_t b;   // back
    _token *e;
} queue_t;

queue_t queue_init(void);
void    queue_enqueue(queue_t *q, _token val);
_token  queue_dequeue(queue_t *q);

void queue_free(queue_t *q);

#endif // QUEUE_H_
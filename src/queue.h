#include <stddef.h>

#ifndef QUEUE_H_
#define QUEUE_H_

#define queue_is_empty(q) (q->f == (size_t)-1 || q->f > q->b)

typedef struct {
    size_t s;   // size
    size_t f;   // front
    size_t b;   // back
    double *e;
} queue_t;

void queue_init(queue_t *q);

void queue_enqueue(queue_t *q, double val);

double queue_dequeue(queue_t *q);

void queue_free(queue_t *q);

#endif // QUEUE_H_
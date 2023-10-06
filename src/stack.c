#include "stack.h"
#include <malloc.h>

#define STACK_DEFAULT_SIZE 10

void stack_init(stack_t *s)
{
    s->s = STACK_DEFAULT_SIZE;
    s->c = -1;
    free(s->e);
    s->e = (char *) malloc(s->s * sizeof (char));
}

void stack_push(stack_t *s, char val)
{
    s->c++;

    if (s->c+1 == s->s) {
        s->s *= 2;
        s->e = (char *) realloc(s->e, s->s * sizeof (char));
    }

    s->e[s->c] = val;
}

char stack_pop(stack_t *s)
{
    if (stack_is_empty(s)) {
        exit(-10);
    }

    return s->e[s->c--];
}

char stack_peek(stack_t *s)
{
    if (stack_is_empty(s)) {
        exit(-10);
    }

    return s->e[s->c];
}

void stack_free(stack_t *s)
{
    free(s->e);
}

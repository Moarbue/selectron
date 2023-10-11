#include "stack.h"
#include <malloc.h>

#define STACK_DEFAULT_SIZE 10

void stack_init(stack_t *s)
{
    s->s = STACK_DEFAULT_SIZE;
    s->c = -1;
    s->e = (token_t *) malloc(s->s * sizeof (token_t));
}

void stack_push(stack_t *s, token_t val)
{
    s->c++;

    if (s->c+1 == s->s) {
        s->s *= 2;
        s->e = (token_t *) realloc(s->e, s->s * sizeof (token_t));
    }

    s->e[s->c] = val;
}

token_t stack_pop(stack_t *s)
{
    if (stack_is_empty(s)) {
        return (token_t) {.t = T_NO_TOKEN, .c = -1};
    }

    return s->e[s->c--];
}

token_t stack_peek(stack_t *s)
{
    if (stack_is_empty(s)) {
        return (token_t) {.t = T_NO_TOKEN, .c = -1};
    }

    return s->e[s->c];
}

void stack_free(stack_t *s)
{
    free(s->e);
}

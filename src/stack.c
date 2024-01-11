#include "stack.h"
#include <malloc.h>

stack_t stack_init(void)
{
    stack_t s;

    s.c = STACK_INITIAL_CAPACITY;
    s.s = 0;
    s.e = (_token *) malloc(s.c * sizeof (_token));
    if (s.e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for stack"));

    return s;
}

void stack_push(stack_t *s, _token val)
{
    if (++s->s >= s->c) {
        s->c *= 2;
        s->e = (_token *) realloc(s->e, s->c * sizeof (_token));
        if (s->e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for stack!"));
    }

    s->e[s->s-1] = val;
}

_token stack_pop(stack_t *s)
{
    if (stack_is_empty(*s)) exit_on_error(error(ERROR_STACK_EMPTY, "Stack is empty"));

    return s->e[--s->s];
}

_token stack_peek(stack_t *s)
{
    if (stack_is_empty(*s)) return dummy_token;

    return s->e[s->s-1];
}

void stack_free(stack_t *s)
{
    free(s->e);
}

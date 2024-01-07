#include "stack.h"
#include <malloc.h>

#define STACK_DEFAULT_SIZE 16

error_t stack_init(stack_t *s)
{
    s->s = STACK_DEFAULT_SIZE;
    s->c = -1;
    s->e = (token_t *) malloc(s->s * sizeof (token_t));
    if (s->e == NULL)
        return error(ERROR_NO_MEMORY, "Failed to allocate memory for stack!");

    return error(ERROR_NO_ERROR, NULL);
}

error_t stack_push(stack_t *s, token_t val)
{
    s->c++;

    if (s->c == s->s) {
        s->s *= 2;
        s->e = (token_t *) realloc(s->e, s->s * sizeof (token_t));

        if (s->e == NULL)
            return error(ERROR_NO_MEMORY, "Failed to allocate memory for stack!");
    }

    s->e[s->c] = val;

    return error(ERROR_NO_ERROR, NULL);
}

token_t stack_pop(stack_t *s)
{
    if (stack_is_empty(s)) {
        return (token_t){0};
    }

    return s->e[s->c--];
}

token_t stack_peek(stack_t *s)
{
    if (stack_is_empty(s)) {
        return (token_t){0};
    }

    return s->e[s->c];
}

void stack_free(stack_t *s)
{
    free(s->e);
}

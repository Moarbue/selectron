#include "token.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void _token_print(_token t)
{
    switch (t.t) {
        case T_NUMBER:
            printf("%.7lg", t.n);
        break;
        case T_LBRACKET:
        case T_RBRACKET:
        case T_COMMA:
            printf("%c", t.c);
        break;
        case T_OPERATOR:
        case T_FUNCTION:
            printf("%s", t.op.name);
        break;
        default:
        break;
    }
}

tokens_t tokens_init(void)
{
    tokens_t t;

    t.c = TOKENS_INITIAL_CAPACITY;
    t.s = 0;
    t.e = (_token *) malloc(t.c * sizeof (_token));
    if (t.e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for tokens"));

    return t;
}

void tokens_add(tokens_t *t, _token tok)
{
    if (++t->s > t->c) {
        t->c *= 2;
        t->e  = (_token *) realloc(t->e, t->c * sizeof (_token));
        if (t->e == NULL) exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for tokens"));
    }

    t->e[t->s-1] = tok;
}

void tokens_print(tokens_t t)
{
    for (size_t i = 0; i < t.s; i++) {
        _token_print(t.e[i]);
    }
    printf("\n");
}

void tokens_free(tokens_t t)
{
    free(t.e);
}

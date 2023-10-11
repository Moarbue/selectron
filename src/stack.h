#include <stddef.h>
#include "tokenizer.h"

#ifndef STACK_H_
#define STACK_H_

#define stack_is_empty(s) ((s)->c == (size_t)-1)

typedef struct {
    size_t s;   // size
    size_t c;   // capacity
    token_t *e;
} stack_t;

void stack_init(stack_t *s);

void stack_push(stack_t *s, token_t val);

token_t stack_pop(stack_t *s);

token_t stack_peek(stack_t *s);

#endif // STACK_H_
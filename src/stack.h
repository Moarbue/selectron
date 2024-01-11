#include <stddef.h>
#include "token.h"
#include "tokenizer.h"

#ifndef STACK_H_
#define STACK_H_

#define STACK_INITIAL_CAPACITY 16
#define stack_is_empty(stack) ((stack).s == 0)

typedef struct {
    size_t c;   // capacity
    size_t s;   // size
    _token *e;  // tokens
} stack_t;

stack_t stack_init(void);
void    stack_push(stack_t *s, _token val);
_token  stack_pop(stack_t *s);
_token  stack_peek(stack_t *s);

void stack_free(stack_t *s);

#endif // STACK_H_
#include "operator.h"
#include <stddef.h>

#ifndef TOKEN_H_
#define TOKEN_H_

enum Token {
    T_DUMMY = 0,
    T_NUMBER,
    T_FUNCTION,
    T_OPERATOR,
    T_COMMA,
    T_LBRACKET,
    T_RBRACKET,
};

typedef struct {
    int t;
    union {
        double n;
        char   c;
        operator_t op;
    };
} _token;

#define dummy_token          (_token){.t = T_DUMMY,    .c  = '\0'}
#define number_token(num)    (_token){.t = T_NUMBER,   .n  = num}
#define function_token(name) (_token){.t = T_FUNCTION, .op = str_to_op(name)}
#define operator_token(c)    (_token){.t = T_OPERATOR, .op = char_to_op(c)}
#define comma_token          (_token){.t = T_COMMA,    .c = ','}
#define lbracket_token       (_token){.t = T_LBRACKET, .c = '('}
#define rbracket_token       (_token){.t = T_RBRACKET, .c = ')'}

void _token_print(_token t);


#define TOKENS_INITIAL_CAPACITY 16

typedef struct {
    size_t c;   // capacity
    size_t s;   // size
    _token *e;  // tokens
} tokens_t;

tokens_t tokens_init(void);
void     tokens_add(tokens_t *t, _token tok);
void     tokens_print(tokens_t t);
void     tokens_free(tokens_t t);

#endif // TOKEN_H_
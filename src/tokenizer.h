#include <stddef.h>
#include "error.h"
#include "operator.h"

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#ifndef DECIMAL_DELIMITER
#define DECIMAL_DELIMITER '.'
#endif

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
} token_t;

error_t tokenize(const char exp[], token_t **tokens, size_t *count);

#endif // TOKENIZER_H_
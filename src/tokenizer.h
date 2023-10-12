#include <stddef.h>
#include "error.h"

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#ifndef DECIMAL_DELIMITER
#define DECIMAL_DELIMITER '.'
#endif

enum Token {
    T_NUMBER = 0,
    T_OPERATOR,
    T_LBRACKET,
    T_RBRACKET,
};

typedef struct {
    int t;
    union {
        double n;
        char   c;
    };
} token_t;

error_t tokenize(const char exp[], token_t **tokens, size_t *count);

#endif // TOKENIZER_H_
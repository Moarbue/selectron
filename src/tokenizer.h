#include <stddef.h>

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#define TOKENIZE_SUCCESS            0
#define ERROR_EXPRESSION_EMPTY     -1
#define ERROR_EXCESS_DECIMAL_POINT -2
#define ERROR_UNKNOWN_CHARACTER    -3

#ifndef DECIMAL_DELIMITER
#define DECIMAL_DELIMITER '.'
#endif

enum Token {
    T_NUMBER = 0,
    T_OPERATOR,
    T_LBRACKET,
    T_RBRACKET,
    T_NO_TOKEN,
};

typedef struct {
    int t;
    union {
        double n;
        char   c;
    };
} token_t;

int tokenize(const char exp[], token_t **tokens, size_t *count);

#endif // TOKENIZER_H_
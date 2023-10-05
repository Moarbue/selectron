#include <stdio.h>

#include "tokenizer.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./selectron <expression>\n");
        return -1; 
    }

    token_t *toks;
    size_t count;

    int res = tokenize(argv[1], &toks, &count);
    switch (res) {
        case ERROR_EXPRESSION_EMPTY:
            fprintf(stderr, "ERROR: Failed to tokenize expression: Expression is empty!");
        break;
        case ERROR_EXCESS_DECIMAL_POINT:
            fprintf(stderr, "ERROR: Failed to tokenize expression: Number has to many decimal points!");
        break;
        case ERROR_UNKNOWN_CHARACTER:
            fprintf(stderr, "ERROR: Failed to tokenize expression: Unknown character!");
        break;
    }

    for (size_t i = 0; i < count; i++) {
        token_t t = toks[i]; 
        switch (t.t) {
            case T_NUMBER:
                printf("Number: %.3lf\n", t.n);
            break;
            case T_OPERATOR:
                printf("Operator: %c\n", t.c);
            break;
            case T_LBRACKET:
                printf("Left Bracket: %c\n", t.c);
            break;
            case T_RBRACKET:
                printf("Right Bracket: %c\n", t.c);
            break;
        }
    }

    return 0;
}
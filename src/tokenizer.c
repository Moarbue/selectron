#include "tokenizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

#define isoperator(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%')

// TODO: Better memory allocation of tokens
// TODO: Recognize unary operators (e.g. -1)

int tokenize(const char exp[], token_t **tokens, size_t *count)
{
    if (exp == NULL || strlen(exp) == 0) return ERROR_EXPRESSION_EMPTY;

    token_t *toks;
    token_t *t;
    size_t n, len;
    char c;

    n = 0;
    toks = (token_t *) malloc((n+1) * sizeof(token_t));
    len = strlen(exp);
    for (size_t i = 0; i < len; i++) {
        toks = realloc(toks, (n+1) * sizeof (token_t));
        c =  exp [i];
        t = &toks[n];
        n++;
        
        if (isspace(c)) 
            continue;
        else if (isdigit(c)) {
            t->t = T_NUMBER;
            t->n = 0.0;

            bool   isdec   = false;
            double divisor = 10.0;

            while (isdigit(c) || c == DECIMAL_DELIMITER) {
                if (isdigit(c)) {
                    if (isdec) {
                        t->n += (double)(c - '0') / divisor;
                        divisor *= 10.0;
                    } else 
                        t->n = t->n * 10 + (c - '0');
                } else if (c == DECIMAL_DELIMITER) {
                    if (isdec) 
                        return ERROR_EXCESS_DECIMAL_POINT;
                    isdec = true;
                }

                c = exp[++i];
            }
            i--;
        } else if (isoperator(c)) {
            t->t = T_OPERATOR;
            t->c = c;
        } else if (c == '(') {
            t->t = T_LBRACKET;
            t->c = c;
        } else if (c == ')') {
            t->t = T_RBRACKET;
            t->c = c;
        } else
            return ERROR_UNKNOWN_CHARACTER;
    }

    *tokens = toks;
    *count  = n;

    return TOKENIZE_SUCCESS;
}
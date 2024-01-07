#include "tokenizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_TOKENS_CAPACITY 16

error_t tokenize(const char exp[], token_t **tokens, size_t *count)
{
    if (exp == NULL || strlen(exp) == 0) 
        return error(ERROR_EXPRESSION_EMTPY, "Input expression is empty!");

    token_t *toks;
    token_t *t;
    size_t n, s, len;
    char c;

    n = 0;
    s = INITIAL_TOKENS_CAPACITY;
    len = strlen(exp);

    toks = (token_t *) malloc(s * sizeof (token_t));
    if (toks == NULL)
        return error(ERROR_NO_MEMORY, "Failed to allocate memory for tokens!");


    for (size_t i = 0; i < len; i++) {
        c =  exp[i];

        if (isspace(c)) 
            continue;

        if ((n+1) > s) {
            s *= 2;
            toks = (token_t *) realloc(toks, s * sizeof (token_t));
            if (toks == NULL)
                return error(ERROR_NO_MEMORY, "Failed to allocate memory for tokens!");
        }

        t = &toks[n];
        n++;
        
        if (isdigit(c)) {
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
                        return error(ERROR_EXCESS_DECIMAL_DELIM, "Excess decimal delimiter at character %.*s<-", i+1, exp);
                    isdec = true;
                }

                c = exp[++i];
            }
            i--;
        } else if (isalpha(c)) {
            char fun[128] = {0};
            size_t j = 0;

            while (isalpha(c)) {
                fun[j++] = c;
                if (isfunction(fun)) {
                    if (n > 1 && (toks[n-2].t == T_RBRACKET || toks[n-2].t == T_NUMBER)) {
                        t->t  = T_OPERATOR;
                        t->op = char_to_op('*');
                        i -= j;
                    } else {
                        t->t = T_FUNCTION;
                        t->op = str_to_op(fun);
                    }
                    break;
                }
                c = exp[++i];
            }
            if (!isfunction(fun))
                return error(ERROR_UNKNOWN_EXPRESSION, "Unknown expression %.*s<-", i+1, exp);
        } else if (isoperator(c)) {
            t->t  = T_OPERATOR;
            if (n == 1) {
                t->op = char_to_op(c == '+' ? 'p' : c == '-' ? 'm' : c);
            }
            else if (n > 1) {
                if (toks[n-2].t == T_OPERATOR || toks[n-2].t == T_LBRACKET)
                    t->op = char_to_op(c == '+' ? 'p' : c == '-' ? 'm' : c);
                else
                    t->op = char_to_op(c);
            }
        } else if (c == ',') {
            t->t = T_COMMA;
            t->c = c;
        } else if (c == '(') {
            if (n > 1 && toks[n-2].t == T_RBRACKET) {
                t->t  = T_OPERATOR;
                t->op = char_to_op('*');
                i--;
            } else {
                t->t = T_LBRACKET;
                t->c = c;
            }
        } else if (c == ')') {
            t->t = T_RBRACKET;
            t->c = c;
        } else
            return error(ERROR_UNKNOWN_CHARACTER, "Unknown character at %.*s<-'", i+1, exp);
    }

    if (n == 0)
        return error(ERROR_EXPRESSION_EMTPY, "Input expression is empty!");

    *tokens = toks;
    *count  = n;

    return error(ERROR_NO_ERROR, NULL);
}
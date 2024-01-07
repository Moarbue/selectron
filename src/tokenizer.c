#include "tokenizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_TOKENS_CAPACITY 16
#define MAXIMAL_FUNCTION_LENGTH 128

error_t token_add(token_t **tokens, size_t *count, token_t t)
{
    static size_t c = INITIAL_TOKENS_CAPACITY;

    (*count)++;
    if (*count > c || *count == 1) {
        c *= 2;
        *tokens = (token_t *) realloc(*tokens, c * sizeof (token_t));
        if (*tokens == NULL)
            return error(ERROR_NO_MEMORY, "Failed to allocate memory for tokens!");
    }

    (*tokens)[*count - 1] = t;

    return error(ERROR_NO_ERROR, NULL);
}

error_t str_to_num(const char *exp, size_t *i, double *num)
{
    double n = 0.0;
    double divisor = 10.0;
    bool   isdec   = false;
    char c = exp[*i];

    while (isdigit(c) || c == DECIMAL_DELIMITER) {
        if (isdigit(c)) {
            if (isdec) {
                n += (double)(c - '0') / divisor;
                divisor *= 10.0;
            } else 
                n = n * 10 + (c - '0');
        } else if (c == DECIMAL_DELIMITER) {
            if (isdec) 
                return error(ERROR_EXCESS_DECIMAL_DELIM, "Excess decimal delimiter at character %.*s<-", *i+1, exp);
            isdec = true;
        }

        c = exp[++(*i)];
    }
    (*i)--;

    *num = n;
    return error(ERROR_NO_ERROR, NULL);
}

void print_tokens(token_t *toks, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        switch (toks[i].t) {
            case T_NUMBER:
                printf("%lf", toks[i].n);
            break;
            case T_LBRACKET:
            case T_RBRACKET:
            case T_COMMA:
                printf("%c", toks[i].c);
            break;
            case T_OPERATOR:
            case T_FUNCTION:
                printf("%s", toks[i].op.c);
            break;
            default:
            break;
        }
    }
    printf("\n");
}

error_t tokenize(const char exp[], token_t **tokens, size_t *count)
{
    if (exp == NULL || strlen(exp) == 0) 
        return error(ERROR_EXPRESSION_EMTPY, "Input expression is empty!");

    error_t e;
    token_t *toks, prev;
    size_t n, len;
    char c;

    toks = NULL;
    n = 0;
    len = strlen(exp);


    for (size_t i = 0; i < len; i++) {
        c  = exp[i];
        prev = n > 0 ? toks[n-1] : (token_t){0};

        if (isspace(c)) 
            continue;

        if (isdigit(c)) {

            if (n > 0 && prev.t == T_RBRACKET) {
                e = token_add(&toks, &n, (token_t) {.t = T_OPERATOR, .op = char_to_op('*')});
                return_on_error(e);
            } else if (n > 0 && prev.t == T_FUNCTION) {
                if (prev.op.argc == 1) {
                    e = token_add(&toks, &n, (token_t){.t = T_LBRACKET, .c = '('});
                    return_on_error(e);
                } else if (prev.op.argc == 0) {
                    e = token_add(&toks, &n, (token_t){.t = T_LBRACKET, .c = '('});
                    return_on_error(e);
                    e = token_add(&toks, &n, (token_t){.t = T_RBRACKET, .c = ')'});
                    return_on_error(e);
                    e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('*')});
                    return_on_error(e);
                }
            }

            double num;
            e = str_to_num(exp, &i, &num);
            return_on_error(e);
            e = token_add(&toks, &n, (token_t){.t = T_NUMBER, .n = num});
            return_on_error(e);

            if (n > 0 && prev.t == T_FUNCTION) {
                if (prev.op.argc == 1) {
                    e = token_add(&toks, &n, (token_t){.t = T_LBRACKET, .c = '('});
                    return_on_error(e);
                }
            }

        } else if (isalpha(c)) {

            char fun[MAXIMAL_FUNCTION_LENGTH] = {0};
            size_t j = 0;

            while (isalpha(c)) {
                fun[j++] = c;

                if (isfunction(fun) && !isalpha(exp[i+1])) {
                    if (n > 0 && (prev.t == T_RBRACKET || prev.t == T_NUMBER)) {
                        e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('*')});
                        return_on_error(e);
                    }

                    e = token_add(&toks, &n, (token_t){.t = T_FUNCTION, .op = str_to_op(fun)});
                    return_on_error(e);
                    break;
                }
                c = exp[++i];
            }

            if (!isfunction(fun))
                return error(ERROR_UNKNOWN_EXPRESSION, "Unknown expression \'%s\'", fun);
                
        } else if (isoperator(c)) {

            if (n == 0) {
                if (c == '+') {
                    e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('p')});
                    return_on_error(e);
                }
                else if (c == '-') {
                    e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('m')});
                    return_on_error(e);
                }
                else
                    return error(ERROR_LEADING_OPERATOR, "Leading operator \'%c\' is not allowed", c);
            } else if (n > 0) {
                if (prev.t == T_OPERATOR || prev.t == T_LBRACKET) {
                    if (c == '+') {
                        e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('p')});
                        return_on_error(e);
                    }
                    else if (c == '-') {
                        e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('m')});
                        return_on_error(e);
                    }
                } else {
                    if (prev.t == T_FUNCTION && prev.op.argc == 0) {
                        e = token_add(&toks, &n, (token_t){.t = T_LBRACKET, .c = '('});
                        return_on_error(e);
                        e = token_add(&toks, &n, (token_t){.t = T_RBRACKET, .c = ')'});
                        return_on_error(e);
                    }
                    
                    e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op(c)});
                    return_on_error(e);
                }
            }

        } else if (c == ',') {

            e = token_add(&toks, &n, (token_t){.t = T_COMMA, .c = c});
            return_on_error(e);

        } else if (c == '(') {

            if (n > 0 && (prev.t == T_RBRACKET || prev.t == T_NUMBER)) {
                e = token_add(&toks, &n, (token_t){.t = T_OPERATOR, .op = char_to_op('*')});
                return_on_error(e);
            }

            e = token_add(&toks, &n, (token_t){.t = T_LBRACKET, .c = c});
            return_on_error(e);

        } else if (c == ')') {
            
            e = token_add(&toks, &n, (token_t){.t = T_RBRACKET, .c = c});
            return_on_error(e);

        } else
            return error(ERROR_UNKNOWN_CHARACTER, "Unknown character at %.*s<-'", i+1, exp);
    }

    if (n == 0)
        return error(ERROR_EXPRESSION_EMTPY, "Input expression is empty!");

    print_tokens(toks, n);

    *tokens = toks;
    *count  = n;

    return error(ERROR_NO_ERROR, NULL);
}
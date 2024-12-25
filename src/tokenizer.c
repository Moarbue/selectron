#include "tokenizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define MAX_FUNCTION_NAME_LENGTH 128

error_t str_to_num(char **str, double *num)
{
    double n = 0.0;
    double divisor = 10.0;
    bool   isdec   = false;
    char *c = *str;

    while (true) {
        
        if (isdigit(*c)) {
            if (isdec) {
                n += (double)(*c - '0') / divisor;
                divisor *= 10.0;
            } else 
                n = n * 10 + (*c - '0');
        } else if (*c == DECIMAL_DELIMITER) {
            if (isdec) 
                return error(ERROR_EXCESS_DECIMAL_DELIM, "%.*s<-", c-*str, *str);
            isdec = true;
        }

        if (isdigit(*(c+1)) || *(c+1) == DECIMAL_DELIMITER) c++;
        else break;
    }

    *str = c;
    *num = n;
    return error(ERROR_NO_ERROR, NULL);
}

error_t tokenize(const char exp[], tokens_t *tokens)
{
    if (exp == NULL || strlen(exp) == 0) 
        return error(ERROR_EXPRESSION_EMTPY, "");

    _token prev;
    tokens_t t = tokens_init();
    tokens_add(&t, dummy_token);

    for (char *c = (char *)exp; *c != '\0'; c++) {
        if (isspace(*c)) continue;

        prev = t.e[t.s-1];

        if (isdigit(*c)) {

            double num;
            error_t e = str_to_num(&c, &num); 
            return_on_error(e);

            switch (prev.t) {
                case T_NUMBER:
                return error(ERROR_MISSING_OPERATOR, "%.*s<-", c-exp, exp);

                case T_FUNCTION:
                    // e.g pi3 -> pi()*3
                    if (prev.op.argc == 0) {
                        tokens_add(&t, lbracket_token);
                        tokens_add(&t, rbracket_token);
                        tokens_add(&t, operator_token('*'));
                        tokens_add(&t, number_token(num));
                    }
                    // e.g. sin3 -> sin(3) 
                    else if (prev.op.argc == 1) {
                        tokens_add(&t, lbracket_token);
                        tokens_add(&t, number_token(num));
                        tokens_add(&t, rbracket_token);
                    }
                break;

                // e.g. )3 -> )*3
                case T_RBRACKET:
                    tokens_add(&t, operator_token('*'));
                /* fall through */

                case T_DUMMY:
                case T_OPERATOR:
                case T_COMMA:
                case T_LBRACKET:
                    tokens_add(&t, number_token(num));
            }

        } else if (isalpha(*c)) {

            char buf[MAX_FUNCTION_NAME_LENGTH] = {0};
            size_t i = 0;

            while (isalpha(*c)) {
                buf[i++] = *c;
                if (isfunction(buf) && !isalpha(*(c+1))) break;
                c++;
            }

            if (isfunction(buf)) {
                switch (prev.t) {
                    case T_FUNCTION:
                    return error(ERROR_FUNCTION_PLACEMENT, "%.*s<-", c-exp, exp);

                    // e.g. 3sin -> 3*sin
                    case T_NUMBER:
                    case T_RBRACKET:
                        tokens_add(&t, operator_token('*'));
                    /* fall through */

                    case T_DUMMY:
                    case T_OPERATOR:
                    case T_COMMA:
                    case T_LBRACKET:
                        tokens_add(&t, function_token(buf));
                }
            } else return error(ERROR_UNKNOWN_EXPRESSION, "\'%s\'", buf);

        } else if (isoperator(*c)) {

            switch (prev.t) {
                // unary operators
                case T_DUMMY:
                case T_LBRACKET:
                case T_OPERATOR:
                case T_COMMA:
                    if (*c == '+')
                        tokens_add(&t, operator_token('p'));
                    else if (*c == '-')
                        tokens_add(&t, operator_token('m'));
                    else
                        return error(ERROR_OPERATOR_PLACEMENT, "%.*s<-", c-exp, exp);
                break;

                case T_FUNCTION:
                    if (prev.op.argc == 0) {
                        tokens_add(&t, lbracket_token);
                        tokens_add(&t, rbracket_token);
                    } else return error(ERROR_OPERATOR_PLACEMENT, "%.*s<-", c-exp, exp);
                /* fall through */
                
                case T_NUMBER:
                case T_RBRACKET:
                    tokens_add(&t, operator_token(*c));
            }

        } else if (*c == ',') {

            switch (prev.t) {
                case T_DUMMY:
                case T_FUNCTION:
                case T_OPERATOR:
                case T_LBRACKET:
                return error(ERROR_COMMA_PLACEMENT, "%.*s<-", c-exp, exp);

                case T_NUMBER:
                case T_RBRACKET:
                    tokens_add(&t, comma_token);
            }

        } else if (*c == '(') {

            switch (prev.t) {
                case T_NUMBER:
                case T_RBRACKET:
                    tokens_add(&t, operator_token('*'));
                /* fall through */

                case T_DUMMY:
                case T_FUNCTION:
                case T_OPERATOR:
                case T_COMMA:
                case T_LBRACKET:
                    tokens_add(&t, lbracket_token);
            }

        } else if (*c == ')') {

            switch (prev.t) {
                case T_DUMMY:
                case T_FUNCTION:
                case T_OPERATOR:
                case T_COMMA:
                case T_LBRACKET:
                return error(ERROR_RBRACKET_PLACEMENT, "%.*s<-", c-exp, exp);

                case T_NUMBER:
                case T_RBRACKET:
                    tokens_add(&t, rbracket_token);
            }

        }
    }

    if (t.s == 0)
        return error(ERROR_EXPRESSION_EMTPY, "Input expression is empty!");

    *tokens = t;

    return error(ERROR_NO_ERROR, NULL);
}
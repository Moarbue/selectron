#include "parser.h"
#include "tokenizer.h"
#include "stack.h"
#include "queue.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum Operator_precedence {
    OPP1 = 0,
    OPP2 = 1,
    OPP3 = 2,
    OPP4 = 3,
};

size_t op_get_precedence(char op)
{
    switch (op) {
        case '+':
        case '-':
        return OPP1;

        case '*':
        case '/':
        return OPP2;

        case '%':
        return OPP3;

        case '^':
        return OPP4;

        default: 
        exit(-10);
    };
}

bool op_greater_precedence(char op1, char op2)
{
    return (op_get_precedence(op1) > op_get_precedence(op2));
}

bool op_greater_equal_precedence(char op1, char op2)
{
    return (op_get_precedence(op1) >= op_get_precedence(op2));
}

bool op_left_assoc(char op)
{
    return !(op == '^');
}

error_t shunting_yard(token_t tokens[], size_t count, queue_t *rpn)
{
    stack_t op_stack;
    stack_init(&op_stack);

    queue_t res;
    queue_init(&res);

    error_t e;

    for (size_t i = 0; i < count; i++) {
        token_t t = tokens[i];

        switch (t.t) {
            case T_NUMBER:
                e = queue_enqueue(&res, t);
                log_on_error(e);
            break;
            
            case T_OPERATOR:
                while (!stack_is_empty(&op_stack) && stack_peek(&op_stack).t == T_OPERATOR &&
                       ((op_left_assoc(t.c) && op_greater_equal_precedence(stack_peek(&op_stack).c, t.c)) ||
                       (!op_left_assoc(t.c) && op_greater_precedence(stack_peek(&op_stack).c, t.c)))) {
                    e = queue_enqueue(&res, stack_pop(&op_stack));
                    log_on_error(e);
                }
                e = stack_push(&op_stack, t);
                log_on_error(e);
            break;

            case T_LBRACKET:
                e = stack_push(&op_stack, t);
                log_on_error(e);
            break;

            case T_RBRACKET:
                while (!stack_is_empty(&op_stack) && stack_peek(&op_stack).t != T_LBRACKET) {
                    e = queue_enqueue(&res, stack_pop(&op_stack));
                    log_on_error(e);
                }
                stack_pop(&op_stack);
            break;
        }
    }
    
    while (!stack_is_empty(&op_stack)) {
        e = queue_enqueue(&res, stack_pop(&op_stack));
        log_on_error(e);
    }

    *rpn = res;

    stack_free(&op_stack);

    return error(ERROR_NO_ERROR, NULL);
}

error_t eval_rpn(queue_t rpn, double *result)
{
    stack_t res;
    stack_init(&res);

    error_t e;

    while(!queue_is_empty(&rpn)) {
        token_t t = queue_dequeue(&rpn);

        if (t.t == T_NUMBER) {
            e = stack_push(&res, t);
            log_on_error(e);
        } else if (t.t == T_OPERATOR) {
            double val2 = stack_pop(&res).n;
            double val1 = stack_pop(&res).n;

            switch (t.c) {
                case '+':
                    t.n = val1 + val2;
                break;

                case '-':
                    t.n = val1 - val2;
                break;

                case '*':
                    t.n = val1 * val2;
                break;

                case '/':
                    t.n = val1 / val2;
                break;

                case '%':
                    t.n = fmod(val1, val2);
                break;

                case '^':
                    t.n = pow(val1, val2);
                break;
            }

            e = stack_push(&res, t);
            log_on_error(e);
        }
    }

    if (res.c != 0) {
        exit(-10);
    }

    *result = stack_peek(&res).n;

    stack_free(&res);
    queue_free(&rpn);

    return error(ERROR_NO_ERROR, NULL);
}

double evaluate(char exp[])
{
    token_t *toks;
    size_t count;
    queue_t rpn;
    double result;
    error_t e;

    e = tokenize(exp, &toks, &count);
    log_on_error(e);

    e = shunting_yard(toks, count, &rpn);
    log_on_error(e);

    e = eval_rpn(rpn, &result);
    log_on_error(e);

    return result;
}
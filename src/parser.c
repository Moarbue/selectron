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

int shunting_yard(token_t tokens[], size_t count, queue_t *rpn)
{
    stack_t op_stack;
    stack_init(&op_stack);

    queue_t res;
    queue_init(&res);

    for (size_t i = 0; i < count; i++) {
        token_t t = tokens[i];

        switch (t.t) {
            case T_NUMBER:
                queue_enqueue(&res, t);
            break;
            
            case T_OPERATOR:
                while (!stack_is_empty(&op_stack) && stack_peek(&op_stack).t == T_OPERATOR &&
                       ((op_left_assoc(t.c) && op_greater_equal_precedence(stack_peek(&op_stack).c, t.c)) ||
                       (!op_left_assoc(t.c) && op_greater_precedence(stack_peek(&op_stack).c, t.c)))) {
                    queue_enqueue(&res, stack_pop(&op_stack));
                }
                stack_push(&op_stack, t);
            break;

            case T_LBRACKET:
                stack_push(&op_stack, t);
            break;

            case T_RBRACKET:
                while (!stack_is_empty(&op_stack) && stack_peek(&op_stack).t != T_LBRACKET) {
                    queue_enqueue(&res, stack_pop(&op_stack));
                }
                stack_pop(&op_stack);
            break;
        }
    }
    
    while (!stack_is_empty(&op_stack)) {
        queue_enqueue(&res, stack_pop(&op_stack));
    }

    *rpn = res;

    stack_free(&op_stack);

    return 0;
}

int eval_rpn(queue_t rpn, double *result)
{
    stack_t res;
    stack_init(&res);

    while(!queue_is_empty(&rpn)) {
        token_t t = queue_dequeue(&rpn);

        if (t.t == T_NUMBER) {
            stack_push(&res, t);
        } else if (t.t == T_OPERATOR) {
            double val2 = stack_pop(&res).n;
            double val1 = stack_pop(&res).n;

            switch (t.c) {
                case '+':
                    t.n = val1 + val2;
                    stack_push(&res, t);
                break;

                case '-':
                    t.n = val1 - val2;
                    stack_push(&res, t);
                break;

                case '*':
                    t.n = val1 * val2;
                    stack_push(&res, t);
                break;

                case '/':
                    t.n = val1 / val2;
                    stack_push(&res, t);
                break;

                case '%':
                    t.n = fmod(val1, val2);
                    stack_push(&res, t);
                break;

                case '^':
                    t.n = pow(val1, val2);
                    stack_push(&res, t);
                break;
            }
        }
    }

    if (res.c != 0) {
        exit(-10);
    }

    *result = stack_peek(&res).n;

    stack_free(&res);
    queue_free(&rpn);

    return 0;
}

double evaluate(char exp[])
{
    token_t *toks;
    size_t count;
    queue_t rpn;
    double result;

    int res = tokenize(exp, &toks, &count);
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

    res = shunting_yard(toks, count, &rpn);

    res = eval_rpn(rpn, &result);

    return result;
}
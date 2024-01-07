#include "parser.h"
#include "tokenizer.h"
#include "stack.h"
#include "queue.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

error_t shunting_yard(token_t tokens[], size_t count, queue_t *rpn)
{
    error_t e;

    stack_t op_stack;
    e = stack_init(&op_stack);
    log_on_error(e);

    queue_t res;
    e = queue_init(&res);
    log_on_error(e);

    for (size_t i = 0; i < count; i++) {
        token_t t = tokens[i];

        switch (t.t) {
            case T_NUMBER:
                e = queue_enqueue(&res, t);
                log_on_error(e);
            break;

            case T_FUNCTION:
                e = stack_push(&op_stack, t);
                log_on_error(e);
            break;
            
            case T_OPERATOR:
                token_t t2 = stack_peek(&op_stack);

                while (!stack_is_empty(&op_stack) && t2.t == T_OPERATOR &&
                       ((t.op.a == ASSOC_LEFT  && (t2.op.p >= t.op.p)) ||
                        (t.op.a == ASSOC_RIGHT && (t2.op.p >  t.op.p)))) {
                    e = queue_enqueue(&res, stack_pop(&op_stack));
                    log_on_error(e);

                    t2 = stack_peek(&op_stack);
                }
                e = stack_push(&op_stack, t);
                log_on_error(e);
            break;

            case T_COMMA:
                while (stack_peek(&op_stack).t != T_LBRACKET) {
                    e = queue_enqueue(&res, stack_pop(&op_stack));
                    log_on_error(e);
                }
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
                if (stack_is_empty(&op_stack))
                    return error(ERROR_MISMATCHED_PARENTHESES, "There are mismatched parentheses in the expression");

                stack_pop(&op_stack);
                if (stack_peek(&op_stack).t == T_FUNCTION) {
                    e = queue_enqueue(&res, stack_pop(&op_stack));
                    log_on_error(e);
                }
            break;
        }
    }
    
    while (!stack_is_empty(&op_stack)) {
        if (stack_peek(&op_stack).t == T_LBRACKET)
            return error(ERROR_MISMATCHED_PARENTHESES, "There are mismatched parentheses in the expression");

        e = queue_enqueue(&res, stack_pop(&op_stack));
        log_on_error(e);
    }

    *rpn = res;

    free(tokens);
    stack_free(&op_stack);

    return error(ERROR_NO_ERROR, NULL);
}

error_t eval_rpn(queue_t rpn, double *result)
{
    error_t e;

    stack_t res;
    e = stack_init(&res);
    log_on_error(e);

    while(!queue_is_empty(&rpn)) {
        token_t t = queue_dequeue(&rpn);

        if (t.t == T_NUMBER) {
            e = stack_push(&res, t);
            log_on_error(e);
        } else if (t.t == T_OPERATOR || t.t == T_FUNCTION) {
            double n2, n1;

            if (t.t == T_FUNCTION ||
                (t.t == T_OPERATOR && (t.op.c[0] == 'p' || t.op.c[0] == 'm')))
                n1 = stack_pop(&res).n;
            else {
                n2 = stack_pop(&res).n;
                n1 = stack_pop(&res).n;
            }
            
            t.n = (*t.op.o)(n1, n2);

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
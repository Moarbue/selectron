#include "parser.h"
#include "token.h"
#include "tokenizer.h"
#include "stack.h"
#include "queue.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

error_t shunting_yard(tokens_t tokens, queue_t *rpn)
{
    stack_t op_stack = stack_init();
    queue_t res = queue_init();

    for (size_t i = 0; i < tokens.s; i++) {
        _token t = tokens.e[i];

        switch (t.t) {
            case T_NUMBER:
                queue_enqueue(&res, t);
            break;

            case T_FUNCTION:
            case T_LBRACKET:
                stack_push(&op_stack, t);
            break;
            
            case T_OPERATOR:
                _token t2 = stack_peek(&op_stack);

                while (!stack_is_empty(op_stack) && t2.t == T_OPERATOR &&
                       ((t.op.a == ASSOC_LEFT  && (t2.op.p >= t.op.p)) ||
                        (t.op.a == ASSOC_RIGHT && (t2.op.p >  t.op.p)))) {

                    queue_enqueue(&res, stack_pop(&op_stack));
                    t2 = stack_peek(&op_stack);
                }

                stack_push(&op_stack, t);
            break;

            case T_COMMA:
                while (stack_peek(&op_stack).t != T_LBRACKET) {
                    queue_enqueue(&res, stack_pop(&op_stack));
                }
            break;

            case T_RBRACKET:
                while (!stack_is_empty(op_stack) && stack_peek(&op_stack).t != T_LBRACKET) {
                    queue_enqueue(&res, stack_pop(&op_stack));
                }
                if (stack_is_empty(op_stack))
                    return error(ERROR_MISMATCHED_PARENTHESES, "There are mismatched parentheses in the expression");

                stack_pop(&op_stack);
                if (stack_peek(&op_stack).t == T_FUNCTION) {
                    queue_enqueue(&res, stack_pop(&op_stack));
                }
            break;
        }
    }
    
    while (!stack_is_empty(op_stack)) {
        if (stack_peek(&op_stack).t == T_LBRACKET)
            return error(ERROR_MISMATCHED_PARENTHESES, "There are mismatched parentheses in the expression");

        queue_enqueue(&res, stack_pop(&op_stack));
    }

    *rpn = res;

    tokens_free(tokens);
    stack_free(&op_stack);

    return error(ERROR_NO_ERROR, NULL);
}

error_t eval_rpn(queue_t rpn, double *result)
{
    stack_t res = stack_init();

    while(!queue_is_empty(rpn)) {
        _token t = queue_dequeue(&rpn);

        switch (t.t) {
            case T_NUMBER:
                stack_push(&res, t);
            break;

            case T_OPERATOR:
            case T_FUNCTION:
                double *nums = (double *) malloc(t.op.argc * sizeof (double));
                double ans;

                if (nums == NULL && t.op.argc != 0)
                    exit_on_error(error(ERROR_NO_MEMORY, "Failed to allocate memory for function parameters of \'%s\'", t.op.name));
                
                for (size_t i = t.op.argc - 1; i < t.op.argc; i--) {
                    nums[i] = stack_pop(&res).n;
                }
                
                ans = (*t.op.o)(nums);
                free(nums);

                if (isnan(ans)) {
                    return error(ERROR_CALCULATION, "Failed to calculate \'%s()\'", t.op.name);
                }

                t.n = ans;
                stack_push(&res, t);
            break;

            default:
            return error(ERROR_EVALUATING_EXPRESSION, "Found unallowed token");
        }
    }

    if (res.s != 1) {
        return error(ERROR_EVALUATING_EXPRESSION, "Found tokens in stack");
    }

    *result = stack_peek(&res).n;

    stack_free(&res);
    queue_free(&rpn);

    return error(ERROR_NO_ERROR, NULL);
}

double evaluate(char exp[])
{
    tokens_t toks;
    queue_t rpn;
    double result;
    error_t e;

    e = tokenize(exp, &toks);
    exit_on_error(e);

    e = shunting_yard(toks, &rpn);
    exit_on_error(e);

    e = eval_rpn(rpn, &result);
    exit_on_error(e);

    return result;
}
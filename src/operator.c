#include "operator.h"
#include <stddef.h>
#include <math.h>

double op_add(double n1, double n2);
double op_sub(double n1, double n2);
double op_mul(double n1, double n2);
double op_div(double n1, double n2);
double op_mod(double n1, double n2);
double op_pow(double n1, double n2);
double op_kep(double n1, double n2);
double op_neg(double n1, double n2);

#define op_init(ch, pr, as, op) ((operator_t){.c = (ch), .p = (pr), .a = (as), .o = (op)})

operator_t ops[] = {
    [OP_ADD] = op_init('+', PREC_0, ASSOC_LEFT,  op_add),
    [OP_SUB] = op_init('-', PREC_0, ASSOC_LEFT,  op_sub),
    [OP_MUL] = op_init('*', PREC_1, ASSOC_LEFT,  op_mul),
    [OP_DIV] = op_init('/', PREC_1, ASSOC_LEFT,  op_div),
    [OP_MOD] = op_init('%', PREC_1, ASSOC_LEFT,  op_mod),
    [OP_POW] = op_init('^', PREC_2, ASSOC_RIGHT, op_pow),
    [OP_KEP] = op_init('p', PREC_3, ASSOC_RIGHT, op_kep),
    [OP_NEG] = op_init('m', PREC_3, ASSOC_RIGHT, op_neg),
};

int isoperator(const char c)
{
    for (size_t i = 0; i < OP_COUNT; i++) {
        if (c == ops[i].c) return 1;
    }

    return 0;
}

operator_t char_to_op(const char c)
{
    size_t i = 0;
    for (i = 0; i < OP_COUNT; i++) {
        if (c == ops[i].c) break;
    }

    return ops[i];
}

double op_add(double n1, double n2)
{
    return n1 + n2;
}

double op_sub(double n1, double n2)
{
    return n1 - n2;
}

double op_mul(double n1, double n2)
{
    return n1 * n2;
}

double op_div(double n1, double n2)
{
    return n1 / n2;
}

double op_mod(double n1, double n2)
{
    return fmod(n1, n2);
}

double op_pow(double n1, double n2)
{
    return pow(n1, n2);
}

double op_kep(double n1, double n2)
{
    (void) n2;
    return n1;
}

double op_neg(double n1, double n2)
{
    (void) n2;
    return -n1;
}
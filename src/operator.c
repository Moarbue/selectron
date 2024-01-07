#include "operator.h"
#include <stddef.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

double op_add(double n1, double n2);
double op_sub(double n1, double n2);
double op_mul(double n1, double n2);
double op_div(double n1, double n2);
double op_mod(double n1, double n2);
double op_pow(double n1, double n2);
double op_kep(double n1, double n2);
double op_neg(double n1, double n2);

double fun_sin(double n1, double n2);
double fun_cos(double n1, double n2);
double fun_tan(double n1, double n2);

#define op_init(ch, pr, as, op) ((operator_t){.c = (ch), .p = (pr), .a = (as), .o = (op)})
#define func_init(ch, op) ((operator_t){.c = (ch), .p = (0), .a = (0), .o = (op)})

operator_t ops[] = {
    [OP_ADD] = op_init("+",   PREC_0, ASSOC_LEFT,  op_add),
    [OP_SUB] = op_init("-",   PREC_0, ASSOC_LEFT,  op_sub),
    [OP_MUL] = op_init("*",   PREC_1, ASSOC_LEFT,  op_mul),
    [OP_DIV] = op_init("/",   PREC_1, ASSOC_LEFT,  op_div),
    [OP_MOD] = op_init("%",   PREC_1, ASSOC_LEFT,  op_mod),
    [OP_POW] = op_init("^",   PREC_2, ASSOC_RIGHT, op_pow),
    [OP_KEP] = op_init("p",   PREC_3, ASSOC_RIGHT, op_kep),
    [OP_NEG] = op_init("m",   PREC_3, ASSOC_RIGHT, op_neg),
};

operator_t funcs[] = {
    [FUN_SIN] = func_init("sin", fun_sin),
    [FUN_COS] = func_init("cos", fun_cos),
    [FUN_TAN] = func_init("tan", fun_tan),
};

int isoperator(const char c)
{
    for (size_t i = 0; i < OP_COUNT; i++) {
        if (c == ops[i].c[0]) return 1;
    }

    return 0;
}

operator_t char_to_op(const char c)
{
    size_t i = 0;
    for (i = 0; i < OP_COUNT; i++) {
        if (c == ops[i].c[0]) break;
    }

    return ops[i];
}

int isfunction(char *s)
{
    size_t i = 0;
    for (i = 0; i < FUN_COUNT; i++) {
        if (strcmp(s, funcs[i].c) == 0) return 1;
    }

    return 0;
}

operator_t str_to_op(char *s)
{
    size_t i = 0;
    for (i = 0; i < FUN_COUNT; i++) {
        if (strcmp(s, funcs[i].c) == 0) return funcs[i];
    }

    return funcs[i];
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

double fun_sin(double n1, double n2)
{
    (void) n2;
    if (USE_DEGREES) {
        n1 *= M_PI / 180;
    }
    return sin(n1);
}

double fun_cos(double n1, double n2)
{
    (void) n2;
    return cos(n1);
}

double fun_tan(double n1, double n2)
{
    (void) n2;
    return tan(n1);
}

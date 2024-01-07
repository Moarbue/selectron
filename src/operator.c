#include "operator.h"
#include <stddef.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

double op_add(double nums[]);
double op_sub(double nums[]);
double op_mul(double nums[]);
double op_div(double nums[]);
double op_mod(double nums[]);
double op_pow(double nums[]);
double op_kep(double nums[]);
double op_neg(double nums[]);

double fun_sin(double nums[]);
double fun_cos(double nums[]);
double fun_tan(double nums[]);
double fun_asin(double nums[]);
double fun_acos(double nums[]);
double fun_atan(double nums[]);

double fun_sinh(double nums[]);
double fun_cosh(double nums[]);
double fun_tanh(double nums[]);
double fun_asinh(double nums[]);
double fun_acosh(double nums[]);
double fun_atanh(double nums[]);

double fun_max(double nums[]);
double fun_min(double nums[]);
double fun_abs(double nums[]);
double fun_floor(double nums[]);
double fun_ceil(double nums[]);

double fun_e(double nums[]);
double fun_pi(double nums[]);

#define op_init(ch, pr, as, op, count) ((operator_t){.c = (ch), .p = (pr), .a = (as), .o = (op), .argc = (count)})
#define func_init(ch, op, count) ((operator_t){.c = (ch), .p = (0), .a = (0), .o = (op), .argc = (count)})

operator_t ops[] = {
    [OP_ADD] = op_init("+",   PREC_0, ASSOC_LEFT,  op_add, 2),
    [OP_SUB] = op_init("-",   PREC_0, ASSOC_LEFT,  op_sub, 2),
    [OP_MUL] = op_init("*",   PREC_1, ASSOC_LEFT,  op_mul, 2),
    [OP_DIV] = op_init("/",   PREC_1, ASSOC_LEFT,  op_div, 2),
    [OP_MOD] = op_init("%",   PREC_1, ASSOC_LEFT,  op_mod, 2),
    [OP_POW] = op_init("^",   PREC_2, ASSOC_RIGHT, op_pow, 2),
    [OP_KEP] = op_init("p",   PREC_2, ASSOC_RIGHT, op_kep, 1),
    [OP_NEG] = op_init("m",   PREC_2, ASSOC_RIGHT, op_neg, 1),
};

operator_t funcs[] = {
    [FUN_SIN]   = func_init("sin",   fun_sin,   1),
    [FUN_COS]   = func_init("cos",   fun_cos,   1),
    [FUN_TAN]   = func_init("tan",   fun_tan,   1),
    [FUN_ASIN]  = func_init("asin",  fun_asin,  1),
    [FUN_ACOS]  = func_init("acos",  fun_acos,  1),
    [FUN_ATAN]  = func_init("atan",  fun_atan,  1),
    [FUN_SINH]  = func_init("sinh",  fun_sinh,  1),
    [FUN_COSH]  = func_init("cosh",  fun_cosh,  1),
    [FUN_TANH]  = func_init("tanh",  fun_tanh,  1),
    [FUN_ASINH] = func_init("asinh", fun_asinh, 1),
    [FUN_ACOSH] = func_init("acosh", fun_acosh, 1),
    [FUN_ATANH] = func_init("atanh", fun_atanh, 1),
    [FUN_MAX]   = func_init("max",   fun_max,   2),
    [FUN_MIN]   = func_init("min",   fun_min,   2),
    [FUN_ABS]   = func_init("abs",   fun_abs,   1),
    [FUN_FLOOR] = func_init("floor", fun_floor, 1),
    [FUN_CEIL]  = func_init("ceil",  fun_ceil,  1),
    [FUN_E]     = func_init("e",     fun_e,     0),
    [FUN_PI]    = func_init("pi",    fun_pi,    0),
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


double op_add(double nums[])
{
    return nums[0] + nums[1];
}

double op_sub(double nums[])
{
    return nums[0] - nums[1];
}

double op_mul(double nums[])
{
    return nums[0] * nums[1];
}

double op_div(double nums[])
{
    return nums[0] / nums[1];
}

double op_mod(double nums[])
{
    return fmod(nums[0], nums[1]);
}

double op_pow(double nums[])
{
    return pow(nums[0], nums[1]);
}

double op_kep(double nums[])
{
    return nums[0];
}

double op_neg(double nums[])
{
    return -nums[0];
}

double fun_sin(double nums[])
{
    if (USE_DEGREES) nums[0] *= M_PI / 180.0;
    return sin(nums[0]);
}

double fun_cos(double nums[])
{
    if (USE_DEGREES) nums[0] *= M_PI / 180.0;
    return cos(nums[0]);
}

double fun_tan(double nums[])
{
    if (USE_DEGREES) nums[0] *= M_PI / 180.0;
    return tan(nums[0]);
}

double fun_asin(double nums[])
{
    return asin(nums[0]) * USE_DEGREES * 180.0 / M_PI;
}

double fun_acos(double nums[])
{
    return acos(nums[0]) * USE_DEGREES * 180.0 / M_PI;
}

double fun_atan(double nums[])
{
    return atan(nums[0]) * USE_DEGREES * 180.0 / M_PI;
}

double fun_sinh(double nums[])
{
    return sinh(nums[0]);
}

double fun_cosh(double nums[])
{
    return cosh(nums[0]);
}

double fun_tanh(double nums[])
{
    return tanh(nums[0]);
}

double fun_asinh(double nums[])
{
    return asinh(nums[0]);
}

double fun_acosh(double nums[])
{
    return acosh(nums[0]);
}

double fun_atanh(double nums[])
{
    return atanh(nums[0]);
}

double fun_max(double nums[])
{
    return fmax(nums[0], nums[1]);
}

double fun_min(double nums[])
{
    return fmin(nums[0], nums[1]);
}

double fun_abs(double nums[])
{
    return fabs(nums[0]);
}

double fun_floor(double nums[])
{
    return floor(nums[0]);
}

double fun_ceil(double nums[])
{
    return ceil(nums[0]);
}

double fun_e(double nums[])
{
    (void) nums;
    return M_E;
}

double fun_pi(double nums[])
{
    (void) nums;
    return M_PI;
}

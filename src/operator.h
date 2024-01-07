#ifndef OPERATOR_H_
#define OPERATOR_H_

#define USE_DEGREES 1

typedef double (*operation)(double n1, double n2);

typedef enum {
    PREC_0,
    PREC_1,
    PREC_2,
    PREC_3,
    PREC_4,
} op_precedence;

typedef enum {
    ASSOC_LEFT,
    ASSOC_RIGHT,
} op_associativity;

enum operation_t {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_KEP,
    OP_NEG,
    OP_COUNT,
};

enum function_t {
    FUN_SIN,
    FUN_COS,
    FUN_TAN,
    FUN_COUNT,
};

typedef struct {
    char *c;             // ASCII representation
    op_precedence p;
    op_associativity a;
    operation o;
} operator_t;

extern operator_t ops[];

int isoperator(const char c);
operator_t char_to_op(const char c);
int isfunction(char *s);
operator_t str_to_op(char *s);

#endif // OPERATOR_H_
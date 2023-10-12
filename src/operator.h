#ifndef OPERATOR_H_
#define OPERATOR_H_

typedef double (*operation)(double n1, double n2);

typedef enum {
    PREC_0,
    PREC_1,
    PREC_2,
    PREC_3,
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
    OP_COUNT,
};

typedef struct {
    char c;             // ASCII representation
    op_precedence p;
    op_associativity a;
    operation o;
} operator_t;

#define op_init(ch, pr, as, op) ((operator_t){.c = (ch), .p = (pr), .a = (as), .o = (op)})

int isoperator(const char c);
operator_t char_to_op(const char c);

#endif // OPERATOR_H_
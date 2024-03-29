#ifndef ERROR_H_
#define ERROR_H_

#define ERROR_MSG_MAX_LEN 1024

enum error_codes_t {
    ERROR_NO_ERROR = 0,
    ERROR_NO_MEMORY,
    ERROR_EXPRESSION_EMTPY,
    ERROR_EXCESS_DECIMAL_DELIM,
    ERROR_UNKNOWN_CHARACTER,
    ERROR_UNKNOWN_EXPRESSION,
    ERROR_STACK_EMPTY,
    ERROR_QUEUE_EMPTY,
    ERROR_MISMATCHED_PARENTHESES,
    ERROR_OPERATOR_PLACEMENT,
    ERROR_MISSING_OPERATOR,
    ERROR_FUNCTION_PLACEMENT,
    ERROR_COMMA_PLACEMENT,
    ERROR_RBRACKET_PLACEMENT,
    ERROR_EVALUATING_EXPRESSION,
    ERROR_CALCULATION,
};

typedef struct {
    int code;
    char msg[1024];
} error_t;


#define return_on_error(e) if ((e).code != ERROR_NO_ERROR) return e
error_t error(int code, const char fmt[], ...);
void exit_on_error(error_t e);

#endif // ERROR_H_
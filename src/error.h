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
    ERROR_NO_OPERATOR,
    ERROR_MISMATCHED_PARENTHESES,
    ERROR_SYNTAX_ERROR,
};

typedef struct {
    int code;
    char msg[1024];
} error_t;


error_t error(int code, const char fmt[], ...);
void log_on_error(error_t e);

#endif // ERROR_H_
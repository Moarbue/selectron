#include <stddef.h>
#include "error.h"
#include "operator.h"
#include "token.h"

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#ifndef DECIMAL_DELIMITER
#define DECIMAL_DELIMITER '.'
#endif

error_t tokenize(const char exp[], tokens_t *tokens);

#endif // TOKENIZER_H_
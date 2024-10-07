#include "common.h"

typedef struct {
    TokenType type;
    char *lexeme;
    char *literal;
    int line;
} Token;


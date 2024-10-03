#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h" // Ensure TokenType is defined

typedef struct {
    TokenType type;
    const char* lexeme;
    void *literal;
    int line;
} Token;

#endif // TOKEN_H

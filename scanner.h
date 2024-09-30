#include <stdlib.h>
#include <string.h>

#include "TokenType.h"
#include "Token.h"

Token add_token(TokenType type, int line);
Token scan_token(char c, int line);

void scan_tokens(char *source)
{
    int start = 0;
    int current = 0;
    int line = 1;
    int capacity = 8;
    int token_count = 0;


    Token **tokens = (Token**)malloc(capacity * sizeof(Token*));
        if (tokens == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    while (current <= strlen(source))
    {
        if (token_count >= capacity) {
            capacity *= 2;
            tokens = (Token**)realloc(tokens, capacity * sizeof(Token*));
            if (tokens == NULL) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        char c = source[current];
        if (isspace(c))
        {
            current++;
            continue;
        }
        if (c != '\0') 
        {
            tokens[token_count] = (Token*)malloc(sizeof(Token)); // Allocate memory for each token
            if (tokens[token_count] == NULL) {
                perror("Failed to allocate memory for token");
                exit(EXIT_FAILURE);
            }
            Token temp = scan_token(c, line);
            if (temp.type != 38) {
                *tokens[token_count] = temp;
                printf("Type: %d\n", temp.type);
                token_count++;
            }
        }
        current++;
    }

    // Free allocated memory
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

Token scan_token(char c, int line)
{
    Token result;
    switch (c)
    {
    case '(':
        return add_token(LEFT_PAREN, line);
        break;
    case ')':
        return add_token(RIGHT_PAREN, line);
        break;
    case '{':
        return add_token(LEFT_BRACE, line);
        break;
    case '}':
        return add_token(RIGHT_BRACE, line);
        break;
    case ',':
        return add_token(COMMA, line);
        break;
    case '.':
        return add_token(DOT, line);
        break;
    case '-':
        return add_token(MINUS, line);
        break;
    case '+':
        return add_token(PLUS, line);
        break;
    case ';':
        return add_token(SEMICOLON, line);
        break;
    case '*':
        return add_token(STAR, line);
        break;
    default:
        fprintf(stderr, "Unexpected character '%c'\n", c);
    }
    return add_token(ERROR, line);
}

Token add_token(TokenType type, int line)
{
    Token token;
    token.type = type;
    token.lexeme = NULL;
    token.literal = NULL;
    token.line = line;

    return token;
}
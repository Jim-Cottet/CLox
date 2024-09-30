#include "scanner.h"

int line_number;

void scan_file(FILE *file)
{
    char line[256];

    line_number = 0; // make the global variable 0 before the beginning of the loop

    while (fgets(line, sizeof(line), file))
    {
        line_number++;
        printf("Results for the line #%d \n", line_number);
        scan_tokens(line);
    }
    
    fclose(file);
}

void scan_tokens(char *source)
{
    int token_count = 0;
    int token_capacity = 8; 
    int start = 0;
    int current = 0;

    Token **tokens = (Token**)malloc(token_capacity * sizeof(Token*));
    if (tokens == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    while (current <= strlen(source))
    {
        if (token_count >= token_capacity) {
            token_capacity *= 2;
            tokens = (Token**)realloc(tokens, token_capacity * sizeof(Token*));
            if (tokens == NULL) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        char c = source[current];
        if (isspace(c))
        {   
            current++;
            start = current;
            printf("' ' at col: %d\n", start);
            continue;
        }
        if (c != '\0') 
        {
            tokens[token_count] = (Token*)malloc(sizeof(Token)); // Allocate memory for each token
            if (tokens[token_count] == NULL) {
                perror("Failed to allocate memory for token");
                exit(EXIT_FAILURE);
            }
            Token temp = scan_token(c);
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

Token scan_token(char c)
{
    Token result;
    switch (c)
    {
    case '(':
        return add_token(LEFT_PAREN);
        break;
    case ')':
        return add_token(RIGHT_PAREN);
        break;
    case '{':
        return add_token(LEFT_BRACE);
        break;
    case '}':
        return add_token(RIGHT_BRACE);
        break;
    case ',':
        return add_token(COMMA);
        break;
    case '.':
        return add_token(DOT);
        break;
    case '-':
        return add_token(MINUS);
        break;
    case '+':
        return add_token(PLUS);
        break;
    case ';':
        return add_token(SEMICOLON);
        break;
    case '*':
        return add_token(STAR);
        break;
    default:
        fprintf(stderr, "Unexpected character '%c'\n", c);
    }
    return add_token(ERROR);
}

Token add_token(TokenType type)
{
    Token token;
    token.type = type;
    token.lexeme = NULL;
    token.literal = NULL;
    token.line = line_number;

    return token;
}
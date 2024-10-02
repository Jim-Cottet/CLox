#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Global structs import
#include "TokenType.h"
#include "Token.h"

// Declare global variables
Token *tokens;

// Declare properties
typedef struct {
    int token_count;
    int token_capacity; 
    int start;
    int current;
    int line_number;
    char line[256];
} Scanner;

// Function to initialize the Scanner instance
Scanner* initialize_scanner() {
    Scanner *self = (Scanner*)malloc(sizeof(Scanner));
    if (self == NULL) {
        perror("Failed to allocate memory for scanner");
        exit(EXIT_FAILURE);
    }
    self->token_count = 0;
    self->token_capacity = 8;
    self->start = 0;
    self->current = 0;
    self->line_number = 0;

    return self;
}

// top functions declaration
// scan_file -> entry function
void scan_file(FILE *file);

void scan_tokens(Scanner *self);
Token add_token(TokenType type);
Token scan_token(char c, Scanner *self);
bool is_at_end(Scanner *self);
bool match(char expected, Scanner *self);

void scan_file(FILE *file)
{
    Scanner *self = initialize_scanner();
    self->line_number = 0;

    while (fgets(self->line, sizeof(self->line), file))
    {
        self->line_number++;
        printf("Results for the line #%d \n", self->line_number);
        scan_tokens(self);
    }
    fclose(file);
    free(self);
}

void scan_tokens(Scanner *self)
{   
    // Initialize properties at each iteration
    self->current = 0;
    self->start = 0;
    self->token_count= 0;
    // Token capacity can't be equal to zero
    if (self->token_capacity == 0) 
        self->token_capacity = 8;

    Token **tokens = (Token**)malloc(self->token_capacity * sizeof(Token*));
    if (tokens == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    while (is_at_end(self))
    {
        if (self->token_count >= self->token_capacity) {
            self->token_capacity *= 2;
            tokens = (Token**)realloc(tokens, self->token_capacity * sizeof(Token*));
            if (tokens == NULL) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        char c = self->line[self->current];

        if (isspace(c))
        {   
            self->current++;
            self->start = self->current;
            printf("' ' at col: %d\n", self->start);
            continue;
        }
        if (c != '\0') 
        {
            tokens[self->token_count] = (Token*)malloc(sizeof(Token)); // Allocate memory for each token
            if (tokens[self->token_count] == NULL) {
                perror("Failed to allocate memory for token");
                exit(EXIT_FAILURE);
            }
            Token temp = scan_token(c, self);
            if (temp.type != 38) {
                *tokens[self->token_count] = temp;
                printf("Type: %d\n", temp.type);
                self->token_count++;
            }
        }
        self->current++;
    }

    // Free allocated memory
    for (int i = 0; i < self->token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

Token add_token(TokenType type)
{
    Token token;
    token.type = type;
    token.lexeme = NULL;
    token.literal = NULL;
    token.line =0;

    return token;
}

Token scan_token(char c, Scanner *self)
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
    case '!':
        add_token((match('=', self) ? BANG_EQUAL : BANG));
        break;
    case '=':
        add_token((match('=', self) ? EQUAL_EQUAL : EQUAL));
        break;
    case '<':
        add_token((match('=', self) ? LESS_EQUAL : LESS));
        break;
    case '>':
        add_token((match('=', self) ? GREATER_EQUAL : GREATER));
        break;          
    default:
        fprintf(stderr, "Unexpected character '%c'\n", c);
    }
    return add_token(ERROR);
}

bool is_at_end(Scanner *self) 
{
    return self->current <= strlen(self->line);
}

bool match(char expected, Scanner *self)
{
    if (is_at_end(self)) return false;
    if (self->line[self->current + 1] != expected) return false; 
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Global structs import
#include "TokenType.h"
#include "Token.h"

// Declare properties
typedef struct {
    int token_count;
    int token_capacity; 
    int start;
    int current;
    int line_number;
    char line[256];
    Token *tokens;
} Scanner;

// top functions declaration
// scan_file -> entry function
void scan_file(FILE *file);
void scan_tokens(Scanner *self);
void add_token(TokenType type, Scanner *self, char *string);
Token scan_token(char c, Scanner *self);
bool is_at_end(Scanner *self);
bool match(char expected, Scanner *self);
char peek(Scanner *self);
void string(Scanner *self);
void number(Scanner *self);
bool is_digit(char c);
char peek_next(Scanner *self);

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
        self->tokens = (Token*)malloc(self->token_capacity * sizeof(Token));
    if (self->tokens == NULL) {
        perror("Failed to allocate memory for tokens");
        exit(EXIT_FAILURE);
    }
    return self;
}

void scan_file(FILE *file)
{   
    // "Constructor" of the scanner.c
    Scanner *self = initialize_scanner();
    self->line_number = 0;
    // Go through the file line by line
    while (fgets(self->line, sizeof(self->line), file))
    {
        self->line_number++;
        scan_tokens(self);
    }
    // Append an End OF File token at the end of the list
    add_token(TOKEN_EOF, self, NULL);
    // Display all the tokens in the list
    printf("Token list:\n");
    for (int i = 0; i < self->token_count; i++) {
        printf("Token %d - Type %d - Literal %s\n ", i, self->tokens[i].type, self->tokens[i].literal);
    }
    //Freeing the memory
    fclose(file);
    free(self);
}

void scan_tokens(Scanner *self)
{   
    // Initialize properties at each iteration
    self->current = 0;
    self->start = 0;
    // Read each character of the line
    while (is_at_end(self) != true)
    {
        char c = self->line[self->current];
        scan_token(c, self);
        self->current++;    
    }
}

void add_token(TokenType type, Scanner *self, char *string)
{   
    // Verify the space on list and add some if not enough
    if (self->token_count >= self->token_capacity) {
        self->token_capacity *= 2;
        self->tokens = (Token*)realloc(self->tokens, self->token_capacity * sizeof(Token));
        if (self->tokens == NULL) {
            perror("Failed to reallocate memory for tokens");
            exit(EXIT_FAILURE);
        }
    }
    // Create a new token
    Token token;
    token.type = type;
    token.line = self->line_number;
    token.literal = string;
    // Append a token to the list
    self->tokens[self->token_count++] = token;
}

Token scan_token(char c, Scanner *self)
{
    Token result;
    switch (c)
    {
    case '(':
        add_token(LEFT_PAREN, self, NULL);
        break;
    case ')':
        add_token(RIGHT_PAREN, self, NULL);
        break;
    case '{':
        add_token(LEFT_BRACE, self, NULL);
        break;
    case '}':
        add_token(RIGHT_BRACE, self, NULL);
        break;
    case ',':
        add_token(COMMA, self, NULL);
        break;
    case '.':
        add_token(DOT, self, NULL);
        break;
    case '-':
        add_token(MINUS, self, NULL);
        break;
    case '+':
        add_token(PLUS, self, NULL);
        break;
    case ';':
        add_token(SEMICOLON, self, NULL);
        break;
    case '*':
        add_token(STAR, self, NULL);
        break;
    case '!':
        add_token((match('=', self) ? BANG_EQUAL : BANG), self, NULL);
        break;
    case '=':
        add_token((match('=', self) ? EQUAL_EQUAL : EQUAL), self, NULL);
        break;
    case '<':
        add_token((match('=', self) ? LESS_EQUAL : LESS), self, NULL);
        break;
    case '>':
        add_token((match('=', self) ? GREATER_EQUAL : GREATER), self, NULL);
        break;
    case '/':
        if (match('/', self))
            while (peek(self) != '\n' && is_at_end(self) != true) self->current++;
        else
            add_token(SLASH, self, NULL);
        break;
    case ' ':
    case '\r':
    case '\t':
        self->start = self->current + 1;
        break;
    case '\n':
        self->line_number++;
        break;
    case '"':
        string(self);
        break;
    default:
        if (is_digit(c))
            number(self);
        else
            printf("Unexpected character '%c'\n", c);
        break;
    }
    //add_token(ERROR, self);
}

bool is_at_end(Scanner *self) 
{
    // Return true if end of file
    return self->current >= strlen(self->line);
}

bool match(char expected, Scanner *self)
{
    // Verify the next char correspond toe the expected char in argument
    if (is_at_end(self)) return false;
    if (self->line[self->current + 1] != expected) 
        return false;
    else
        self->current++; 
}

char peek(Scanner *self) 
{   
    // Quickly return the current char
    if (is_at_end(self)) return '\0';
    return self->line[self->current];
}

void string(Scanner *self)
{
    self->current++; //! I don't like that !
    while (peek(self) != '"' && !is_at_end(self))
    {
        if (peek(self) == '\n') self->line_number++;
        self->current++;
    }

    if (is_at_end(self))
    {
        // Handle unterminated string error
        // lox.error(self->line_number, "Unterminated string");
        return;
    }

    int string_size = (self->current) - (self->start + 1);
    int pseudo_start = self->start + 1; // Trim the first double quote
    char *value = (char*)malloc((string_size + 1) * sizeof(char));
    if (value == NULL) {
        perror("Failed to allocate memory for string value");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < string_size; i++)
    {
        value[i] = self->line[pseudo_start];
        pseudo_start++;
    } 
    value[string_size] = '\0';
    add_token(STRING, self, value);
}

void number(Scanner *self)
{
    while (is_digit(peek(self))) 
        self->current++;

    if (peek(self) == '.' && is_digit(peek_next(self)))
        self->current++;
        while (is_digit(peek(self))) 
            self->current++;

    int number_size = (self->current) - (self->start);
    int pseudo_start = self->start;
    char *value = (char*)malloc((number_size + 1) * sizeof(char));
    if (value == NULL) {
        perror("Failed to allocate memory for number value");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < number_size; i++)
    {
        value[i] = self->line[pseudo_start];
        pseudo_start++;
    } 
    value[number_size] = '\0';
    add_token(NUMBER, self, value);
}

char peek_next(Scanner *self)
{
    if (self-> current + 1 >= strlen(self->line)) return '\0';
    return self->line[self->current + 1];
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}
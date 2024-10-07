#include "common.h"

// top functions declaration
// scan_file -> entry function
Scanner* scan_file(FILE *file);
void scan_tokens(Scanner *self, HashTable *table);
void add_token(TokenType type, Scanner *self, char *string);
Token scan_token(char c, Scanner *self, HashTable *table);
bool is_at_end(Scanner *self);
bool match(char expected, Scanner *self);
char peek(Scanner *self);
void string(Scanner *self);
void number(Scanner *self);
bool is_digit(char c);
char peek_next(Scanner *self);
void identifier(Scanner *self, HashTable *table);
bool is_alpha(char c);
bool is_alphanumeric(char c);
void create_keyword_table(HashTable *table);

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

Scanner* scan_file(FILE *file)
{
    // "Constructor" of the scanner.c
    Scanner *self = initialize_scanner();
    // Insert the keywords in the table
    HashTable *keyword_table = create_table();
    create_keyword_table(keyword_table);
    self->line_number = 0;
    self->line_number = 0;
    // Go through the file line by line
    while (fgets(self->line, sizeof(self->line), file))
    {
        self->line_number++;
        scan_tokens(self, keyword_table);
    }
    // Append an End OF File token at the end of the list
    add_token(TOKEN_EOF, self, NULL);

    //Freeing the memory
    fclose(file);
    // Free the keyword table
    for (int i = 0; i < 16; i++) {
        if (keyword_table->items[i] != NULL) {
            free(keyword_table->items[i]->key);
            free(keyword_table->items[i]);
        }
    }
    free(keyword_table->items);
    free(keyword_table);   

    // Return the scanner results
    return self;
}

void scan_tokens(Scanner *self, HashTable *table)
{   
    // Initialize properties at each iteration
    self->current = 0;
    self->start = 0;
    // Read each character of the line
    while (is_at_end(self) != true)
    {
        char c = self->line[self->current];
        scan_token(c, self, table);
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

Token scan_token(char c, Scanner *self, HashTable *table)
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
        {
            number(self);
            self->start = self->current + 1;
        }
        else if (is_alpha(c))
        {
            identifier(self, table);
            self->start = self->current + 1;
        }
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
    printf("String literal: %s\n", value);
    add_token(STRING, self, value);
    free(value);
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
    printf("Number literal: %s\n", value);
    add_token(NUMBER, self, value);
    free(value);
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

bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
            c == '_';
}

bool is_alphanumeric(char c)
{
    return is_alpha(c) || is_digit(c);
}

void identifier(Scanner *self, HashTable *table)
{
    while (is_alphanumeric(peek(self)))
        self->current++;

    int identifier_size = (self->current) - (self->start);
    int pseudo_start = self->start;
    char *value = (char*)malloc((identifier_size) * sizeof(char));
    if (value == NULL) {
        perror("Failed to allocate memory for number value");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < identifier_size; i++)
    {
        value[i] = self->line[pseudo_start];
        pseudo_start++;
    } 
    value[identifier_size] = '\0';
    printf("Value to be searched %s\n", value);
    TokenType type = lookup(table, value);
    if (type == TOKEN_EOF)
        type = IDENTIFIER;
    // If matches keyword
    add_token(type, self, NULL);
    free(value);
}

void create_keyword_table(HashTable *table)
{   
    void insert(HashTable *table, const char *key, TokenType value);

    insert(table, "and", AND);
    insert(table, "class", CLASS);
    insert(table, "else", ELSE);
    insert(table, "false", FALSE);
    insert(table, "for", FOR);
    insert(table, "fun", FUN);
    insert(table, "if", IF);
    insert(table, "nil", NIL);
    insert(table, "or", OR);
    insert(table, "print", PRINT);
    insert(table, "return", RETURN);
    insert(table, "super", SUPER);
    insert(table, "this", THIS);
    insert(table, "true", TRUE);
    insert(table, "var", VAR);
    insert(table, "while", WHILE);
}




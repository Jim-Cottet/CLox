#include <stdio.h>

typedef struct {
    TokenType type;
    const char* lexeme;
    void *literal;
    int line;
} Token;

// Use case ...

//Token token_list[MAX_TOKENS];
//int token_count = 0;

//void add_token(Token token) {
//    if (token_count < MAX_TOKENS) {
//        token_list[token_count++] = token;
//    } else {
//        printf("Token list is full.\n");
//    }
//}

// Example of a Token declaration and adding it to the list
//void example_usage() {
//   Token example_token;
//    example_token.type = EOF; // Assuming TOKEN_IDENTIFIER is a valid TokenType
//    example_token.lexeme = "example";
//    example_token.literal = NULL;
//    example_token.line = 1;

//    add_token(example_token);
//}


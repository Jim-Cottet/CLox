#include "common.h"

typedef struct {
    int current;
} Parser;

// RDP
Expr* expression(Parser *parser, Token *tokens);
Expr* equality(Parser *parser, Token *tokens);
Expr* comparison(Parser *parser, Token *tokens);
Expr* term(Parser *parser, Token *tokens);
Expr* factor(Parser *parser, Token *tokens);
Expr* unary(Parser *parser, Token *tokens);
Expr* primary(Parser *parser, Token *tokens);

// RDG utils
Token previous(Token *tokens, Parser *parser);
Token parser_peek(Token *tokens, Parser *parser);
Token advance(Parser* parser, Token* tokens);
bool match_parser(Token *tokens, Parser *parser,int num, ...);
bool check(TokenType type, Parser *parser, Token *tokens);
bool parser_is_at_end(Parser *parser, Token *tokens);

// Learning
void print_tree(Expr *expr);

// Let's do it!
void parser(Scanner *scanner)
{
    // Declare the parser's structure
    // Declare the parser's structure
    Parser *parser_instance = (Parser*)malloc(sizeof(Parser));
    if (parser_instance == NULL) {
        perror("Failed to allocate memory for parser");
        exit(EXIT_FAILURE);
    }
    parser_instance->current = 0;
    // Print the retrieved List
    printf("Token list:\n");
    for (int i = 0; i < scanner->token_count; i++) {
        printf("Token %d - Type %d - Literal %s\n ", i, scanner->tokens[i].type, scanner->tokens[i].literal);
    }

    Expr *ast = expression(parser_instance, scanner->tokens);
    print_tree(ast);
    // Free the scanner results
    free(scanner->tokens);
    free(parser_instance);
    free(scanner);
    free(ast);
}

// RDP (Gold help us)
Expr* expression(Parser *parser, Token *tokens)
{
    return equality(parser, tokens);
}

Expr* equality(Parser *parser, Token *tokens) 
{
    Expr *expr = comparison(parser, tokens);
    while (match_parser(tokens ,parser, TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL, 2))
    {
        Expr *new_expr = malloc(sizeof(Expr));
        if (new_expr == NULL) {
            perror("Failed to allocate memory for expression");
            exit(EXIT_FAILURE);
        }
        new_expr->type = EXPR_BINARY;
        new_expr->as.binary.left = expr;
        new_expr->as.binary.token= previous(tokens, parser);
        new_expr->as.binary.right = comparison(parser, tokens);
        expr = new_expr;
    }
    return expr;
}

Expr* comparison(Parser *parser, Token *tokens)
{
    Expr *expr = term(parser, tokens);
    while (match_parser(tokens, parser, TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL, 4))
    {
        Expr *new_expr = malloc(sizeof(Expr));
        if (new_expr == NULL) {
            perror("Failed to allocate memory for expression");
            exit(EXIT_FAILURE);
        }
        new_expr->type = EXPR_BINARY;
        new_expr->as.binary.left = expr;
        new_expr->as.binary.token= previous(tokens, parser);
        new_expr->as.binary.right = term(parser, tokens);
        expr = new_expr;
    }
    return expr;
}

Expr* term(Parser *parser, Token *tokens)
{
    Expr *expr = malloc(sizeof(Expr));
    expr = factor(parser, tokens);
    while (match_parser(tokens, parser, TOKEN_MINUS, TOKEN_PLUS, 2))
    {
        Expr *new_expr = malloc(sizeof(Expr));
        if (new_expr == NULL) {
            perror("Failed to allocate memory for expression");
            exit(EXIT_FAILURE);
        }
        new_expr->type = EXPR_BINARY;
        new_expr->as.binary.left = expr;
        new_expr->as.binary.token= previous(tokens, parser);
        new_expr->as.binary.right = factor(parser, tokens);
        expr = new_expr;
    }
    return expr;
}

Expr* factor(Parser *parser, Token *tokens)
{
    Expr *expr = unary(parser, tokens);
    while (match_parser(tokens, parser, TOKEN_SLASH, TOKEN_STAR, 2))
    {
        Expr *new_expr = malloc(sizeof(Expr));
        if (new_expr == NULL) {
            perror("Failed to allocate memory for expression");
            exit(EXIT_FAILURE);
        }
        new_expr->type = EXPR_BINARY;
        new_expr->as.binary.left = expr;
        new_expr->as.binary.token= previous(tokens, parser);
        new_expr->as.binary.right = unary(parser, tokens);
        expr = new_expr;   
    }
    return expr;
}

Expr* unary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, TOKEN_BANG, TOKEN_MINUS, 2))
    {
        Expr *expr = malloc(sizeof(Expr));
        expr->as.unary.token = previous(tokens, parser);
        expr->as.unary.right = unary(parser, tokens);    
        return expr;
    }   
    return primary(parser, tokens);
}

Expr* primary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, TOKEN_FALSE, TOKEN_TRUE, TOKEN_NIL, TOKEN_NUMBER, TOKEN_STRING, 5)) 
    {
        Expr *expr = malloc(sizeof(Expr));
        expr->type = EXPR_LITERAL;
        expr->as.literal.value = previous(tokens, parser).literal;
        return expr;
    }
    if (match_parser(tokens, parser, TOKEN_LEFT_PAREN, 1)) {
        Expr *expr = expression(parser, tokens);
        match_parser(tokens, parser, TOKEN_RIGHT_PAREN, 1);
        printf("At %d got a primary\n", parser->current);  
        return expr;
    }
}

// Utils
// Learning Variadic Function
bool match_parser(Token *tokens, Parser *parser,int num, ...)
{  
    va_list args;
    va_start(args, num);
    for (int i = 0; i < num; i++)
    {
        TokenType type = va_arg(args, TokenType);
        if (check(type, parser, tokens))
        {   
            printf("At current: %d i found: %d\n", parser->current, type);
            advance(parser, tokens);
            va_end(args);
            return true;
        }
    }
    va_end(args);
    return false;
}

Token advance(Parser* parser, Token* tokens) {
    if (!parser_is_at_end(parser, tokens)) {
        parser->current++;
    }
}

bool check(TokenType type, Parser *parser, Token *tokens)
{
    if (parser_is_at_end(parser, tokens)) return false;
    return parser_peek(tokens, parser).type == type;
}

bool parser_is_at_end(Parser *parser, Token *tokens)
{
    return parser_peek(tokens, parser).type == TOKEN_EOF;
}

Token parser_peek(Token *tokens, Parser *parser)
{
    return tokens[parser->current];
}

Token previous(Token *tokens, Parser *parser)
{
    return tokens[parser->current - 1];
}

// Learning utils
// Learning utils
void print_tree(Expr *expr) {
    if (expr == NULL) {
        return;
    }
    switch (expr->type) {
        case EXPR_LITERAL:
            printf("Literal: %s\n", expr->as.literal.value);
            break;
        case EXPR_BINARY:
            printf("Binary Expression:\n");
            printf("Left: ");
            print_tree(expr->as.binary.left);
            printf("Operator: %d\n", expr->as.binary.token.type);
            printf("Right: ");
            print_tree(expr->as.binary.right);
            break;
        case EXPR_UNARY:
            printf("Unary Expression:\n");
            printf("Operator: %d\n", expr->as.unary.token.type);
            printf("Right: ");
            print_tree(expr->as.unary.right);
            break;
        // Add cases for other expression types
        default: {
            printf("Unknown expression type\n");
            break;
        }
    }
}





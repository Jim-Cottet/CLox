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
Token parser_peek(Token *tokens, Parser *parser);
bool match_parser(Token *tokens, Parser *parser,int num, ...);
bool check(TokenType type, Parser *parser, Token *tokens);
bool parser_is_at_end(TokenType type, Parser *parser, Token *tokens);
Token previous(Token *tokens, Parser *parser);
void print_tree(Expr* root);
Expr* return_binary_expr(Expr *expr, Parser *parser, Token *tokens, Expr *right);

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
}

// RDP (Gold help us)
Expr* expression(Parser *parser, Token *tokens)
{
    return equality(parser, tokens);
}

Expr* equality(Parser *parser, Token *tokens) 
{
    Expr *expr = comparison(parser, tokens);
    while (match_parser(tokens ,parser, BANG_EQUAL, EQUAL_EQUAL, 2))
    {
        Expr *right = comparison(parser, tokens);
        expr = return_binary_expr(expr, parser, tokens, right);
        printf("At %d got an equality\n", parser->current);    
    }
    return expr;
}

Expr* comparison(Parser *parser, Token *tokens)
{
    Expr *expr = term(parser, tokens);
    while (match_parser(tokens, parser, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, 4))
    {
        Expr *right = term(parser, tokens);
        expr = return_binary_expr(expr, parser, tokens, right);
        printf("At %d got a comparison\n", parser->current);   
    }
    return expr;
}

Expr* term(Parser *parser, Token *tokens)
{
    Expr *expr = factor(parser, tokens);
    while (match_parser(tokens, parser, MINUS, PLUS, 2))
    {
        Expr *right = factor(parser, tokens);
        expr = return_binary_expr(expr, parser, tokens, right);
        printf("At %d got a term\n", parser->current);        
    }
    return expr;
}

Expr* factor(Parser *parser, Token *tokens)
{
    Expr *expr = unary(parser, tokens);
    while (match_parser(tokens, parser, SLASH, STAR, 2))
    {
        Expr *right = unary(parser, tokens);
        expr = return_binary_expr(expr, parser, tokens, right);
        printf("At %d got a factor\n", parser->current);           
    }
    return expr;
}

Expr* unary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, BANG, MINUS, 2))
    {
        Token operator = previous(tokens, parser);
        Expr *right = unary(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->as.unary.op = EXPR_UNARY;
        new_expr->as.unary.right = right;
        printf("At %d got a unary\n", parser->current);       
        return new_expr;
    }   
    return primary(parser, tokens);
}

Expr* primary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, FALSE, 1)) 
    {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = EXPR_LITERAL;
        new_expr->as.literal.value = previous(tokens, parser).literal;
        printf("At %d got a primary\n", parser->current);   
        return new_expr;
    }
    if (match_parser(tokens, parser, TRUE, 1)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = EXPR_LITERAL;
        new_expr->as.literal.value = previous(tokens, parser).literal;
        printf("At %d got a primary\n", parser->current);  
        return new_expr;
    }
    if (match_parser(tokens, parser, NIL, 1)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = EXPR_LITERAL;
        new_expr->as.literal.value = previous(tokens, parser).literal;
        printf("At %d got a primary\n", parser->current);  
        return new_expr;
    }
    if (match_parser(tokens, parser, NUMBER, STRING, 2)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = EXPR_LITERAL;
        new_expr->as.literal.value = previous(tokens, parser).literal;
        printf("At %d got a primary\n", parser->current);   
        return new_expr;
    }

    if (match_parser(tokens, parser, LEFT_PAREN, 1)) {
        Expr *expr = expression(parser, tokens);
        // Consume
        match_parser(tokens, parser, RIGHT_PAREN, 1);
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
            parser->current++;
            return true;
        }
    }
    return false;
}

bool check(TokenType type, Parser *parser, Token *tokens)
{
    if (parser_is_at_end(type, parser, tokens)) return false;
    return parser_peek(tokens, parser).type == type;
}

bool parser_is_at_end(TokenType type, Parser *parser, Token *tokens)
{
    return parser_peek(tokens, parser).type == EOF;
}

Token parser_peek(Token *tokens, Parser *parser)
{
    return tokens[parser->current];
}

Token previous(Token *tokens, Parser *parser)
{
    return tokens[parser->current - 1];
}

Expr* return_binary_expr(Expr *expr, Parser *parser, Token *tokens, Expr *right)
{
    Token operator = previous(tokens, parser);
    Expr *new_expr = malloc(sizeof(Expr));
    new_expr->type = EXPR_BINARY;
    new_expr->as.binary.left = expr;
    new_expr->as.binary.op = operator.type;
    new_expr->as.binary.right= right;
    return new_expr; 
}

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
            printf("Operator: %d\n", expr->as.binary.op);
            printf("Right: ");
            print_tree(expr->as.binary.right);
            break;
        case EXPR_UNARY:
            printf("Unary Expression:\n");
            printf("Operator: %d\n", expr->as.unary.op);
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





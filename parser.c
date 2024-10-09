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
    expression(parser_instance, scanner->tokens);
    // Free the scanner results
    free(scanner->tokens);
    free(parser_instance);
    free(scanner);
}

// RDP (Gold help us)
Expr* expression(Parser *parser, Token *tokens)
{
    printf("Saddam was here!");
    return equality(parser, tokens);
}

Expr* equality(Parser *parser, Token *tokens) 
{
    Expr *expr = comparison(parser, tokens);
    while (match_parser(tokens ,parser, BANG_EQUAL, EQUAL_EQUAL))
    {
        Token operator = previous(tokens, parser);
        Expr *right = comparison(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr;
        printf("Got a comparison");     
    }
    return expr;
}

Expr* comparison(Parser *parser, Token *tokens)
{
    Expr *expr = term(parser, tokens);
    while (match_parser(tokens, parser, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {   
        // Can I refactor this?
        Token operator = previous(tokens, parser);
        Expr *right = term(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr;
        printf("Got a term");     
    }
    return expr;
}

Expr* term(Parser *parser, Token *tokens)
{
    Expr *expr = factor(parser, tokens);
    while (match_parser(tokens, parser, MINUS, PLUS))
    {
        Token operator = previous(tokens, parser);
        Expr *right = factor(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right = right;
        expr = new_expr;
        printf("Got a Factor");       
    }
    return expr;
}

Expr* factor(Parser *parser, Token *tokens)
{
    Expr *expr = unary(parser, tokens);
    while (match_parser(tokens, parser, SLASH, STAR))
    {
        Token operator = previous(tokens, parser);
        Expr *right = unary(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr; 
        printf("Got a Unary");           
    }
    return expr;
}

Expr* unary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, BANG, MINUS))
    {
        Token operator = previous(tokens, parser);
        Expr *right = unary(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->right = right;
        printf("Got a Unary");     
        return new_expr;
    }   
    return primary(parser, tokens);
}

Expr* primary(Parser *parser, Token *tokens)
{
    if (match_parser(tokens, parser, FALSE)) 
    {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = FALSE;
        printf("Got a primary");  
        return new_expr;
    }
    if (match_parser(tokens, parser, TRUE)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = TRUE;
        printf("Got a primary");  
        return new_expr;
    }
    if (match_parser(tokens, parser, NIL)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = NIL;
        printf("Got a primary");  
        return new_expr;
    }
    if (match_parser(tokens, parser, NUMBER, STRING)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = NUMBER;
        printf("Got a primary");  
        new_expr->op.literal = previous(tokens, parser).literal;
        return new_expr;
    }

    if (match_parser(tokens, parser, LEFT_PAREN)) {
        Expr *expr = expression(parser, tokens);
        // Consume
        match_parser(tokens, parser, RIGHT_PAREN);
        printf("Got a primary");  
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

// Learning tools
void print_tree(Expr* root) {
    if (root == NULL) {
        return;
    }
    // Traverse the left subtree
    print_tree(root->left);
    // Print the current node
    printf("Node: %d\n", root->op.type); // Adjust this line based on what you want to print
    // Traverse the right subtree
    print_tree(root->right);
}






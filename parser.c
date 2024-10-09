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
    printf("Display the RDG travel : \n");
    return equality(parser, tokens);
}

Expr* equality(Parser *parser, Token *tokens) 
{
    Expr *expr = comparison(parser, tokens);
    while (match_parser(tokens ,parser, BANG_EQUAL, EQUAL_EQUAL, 2))
    {
        Token operator = previous(tokens, parser);
        Expr *right = comparison(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr;
        printf("At %d got an equality\n", parser->current);    
    }
    return expr;
}

Expr* comparison(Parser *parser, Token *tokens)
{
    Expr *expr = term(parser, tokens);
    while (match_parser(tokens, parser, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, 4))
    {   
        // Can I refactor this?
        Token operator = previous(tokens, parser);
        Expr *right = term(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr;
        printf("At %d got a comparison\n", parser->current);   
    }
    return expr;
}

Expr* term(Parser *parser, Token *tokens)
{
    Expr *expr = factor(parser, tokens);
    while (match_parser(tokens, parser, MINUS, PLUS, 2))
    {
        Token operator = previous(tokens, parser);
        Expr *right = factor(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right = right;
        expr = new_expr;
        printf("At %d got a term\n", parser->current);        
    }
    return expr;
}

Expr* factor(Parser *parser, Token *tokens)
{
    Expr *expr = unary(parser, tokens);
    while (match_parser(tokens, parser, SLASH, STAR, 2))
    {
        Token operator = previous(tokens, parser);
        Expr *right = unary(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = expr;
        new_expr->right= right;
        expr = new_expr; 
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
        new_expr->type = operator.type;
        new_expr->right = right;
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
        new_expr->type = FALSE;
        printf("At %d got a primary\n", parser->current);   
        return new_expr;
    }
    if (match_parser(tokens, parser, TRUE, 1)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = TRUE;
        printf("At %d got a primary\n", parser->current);  
        return new_expr;
    }
    if (match_parser(tokens, parser, NIL, 1)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = NIL;
        printf("At %d got a primary\n", parser->current);  
        return new_expr;
    }
    if (match_parser(tokens, parser, NUMBER, STRING, 2)) {
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = NUMBER;
        printf("At %d got a primary\n", parser->current);   
        new_expr->op.literal = previous(tokens, parser).literal;
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






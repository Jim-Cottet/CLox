#include "common.h"

typedef struct {
    int current;
} Parser;

Expr expression();
Expr equality();
Expr comparison();
Token parser_peek(Token *tokens, Parser parser);
bool match_parser(Parser parser,int num, ...);
bool check(TokenType type, Parser parser);
bool parser_is_at_end(TokenType type, Parser parser);
Token parser_peek(Token *tokens, Parser parser);
Token previous(Token *tokens, Parser parser);

// Let's do it!
void parser(Scanner *scanner)
{
    // Get the Token list from the scanner

    // Declare the parser's structure

    // Print the retrieved List
    printf("Token list:\n");
    for (int i = 0; i < scanner->token_count; i++) {
        printf("Token %d - Type %d - Literal %s\n ", i, scanner->tokens[i].type, scanner->tokens[i].literal);
    }
    // Free the scanner results
    free(scanner);
}

// RDP (Gold help us)
Expr expression(Parser parser, Token *tokens)
{
    return equality(parser, tokens);
}

Expr equality(Parser parser, Token *tokens) 
{
    Expr expr = comparison();
    while (match_parser(parser, BANG_EQUAL, EQUAL_EQUAL))
    {
        Token operator = previous(tokens, parser);
        Expr right = comparison(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = struct expr;
        new_expr->right= struct right;
        expr = *new_expr;
    }
    return expr;
}

Expr comparison(Parser parser, Token *tokens)
{
    Expr expr = term();
    while (match_parser(parser, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {   
        // Can I refactor this?
        Token operator = previous(tokens, parser);
        Expr right = comparison(parser, tokens);
        Expr *new_expr = malloc(sizeof(Expr));
        new_expr->type = operator.type;
        new_expr->left = struct expr;
        new_expr->right= struct right;
        expr = *new_expr;
    }
}

// Utils
// Learning Variadic Functions
bool match_parser(Parser parser,int num, ...)
{  
    va_list args;
    va_start(args, num);
    for (int i = 0; i < num; i++)
    {
        TokenType type = va_arg(args, TokenType);
        if (check(type, parser))
        {
            advance_parser();
            return true;
        }
    }
    return false;
}

bool check(TokenType type, Parser parser)
{
    if (parser_is_at_end(type, parser)) return false;
    return parser_peek(type, parser).type == type;
}

bool parser_is_at_end(TokenType type, Parser parser)
{
    return parser_peek(type, parser).type == EOF;
}

Token parser_peek(Token *tokens, Parser parser)
{
    return tokens[parser.current];
}

Token previous(Token *tokens, Parser parser)
{
    return tokens[parser.current - 1];
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






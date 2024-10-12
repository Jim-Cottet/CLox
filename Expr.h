#include "common.h"

typedef enum {
    EXPR_UNARY,
    EXPR_GROUPING,
    EXPR_LITERAL,
    EXPR_BINARY
} ExprType;

typedef struct Expr {
    ExprType type;
    union {
        // For literal expression types as needed
        struct {
            char *value;
        } literal;
        // For binary expressions
        struct {
            struct Expr *left;
            TokenType op;
            struct Expr *right;
        } binary;
        // For unary expressions
        struct {
            TokenType op;
            struct Expr *right;
        } unary;
    } as;
} Expr;

#include "common.h"

typedef enum {
    EXPR_UNARY,
    EXPR_GROUPING,
    EXPR_LITERAL,
    EXPR_BINARY
} ExprType;

typedef struct Expr {
    ExprType type;
    struct Expr* left;
    Token op;
    struct Expr* right;
} Expr;

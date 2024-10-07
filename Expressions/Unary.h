#include "../Tokens/Token.h"
#include "Expr.h" // Ensure Expr is defined

typedef struct {
    Expr right;
    Token op;
} Unary;


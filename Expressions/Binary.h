#include "Expr.h"
#include "../Tokens/Token.h"

typedef struct {
    Expr left;
    Token op;
    Expr right;
} Binary;
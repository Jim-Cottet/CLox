// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

// Global structs import
#include "TokenType.h"
#include "Token.h"
#include "Expr.h"
#include "Scanner.h"
#include "KeyWordHash.h"

#include "scanner.c"
#include "parser.c"

#endif // COMMON_H
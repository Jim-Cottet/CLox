#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "TokenType.h"
#include "Token.h"

// Declare global variables
extern Token *tokens;
extern int line_number;

Token add_token(TokenType type);
Token scan_token(char c);
void scan_file(FILE *file);
void scan_tokens(char *source);
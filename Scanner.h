#include "common.h"

// Declare properties
typedef struct {
    int token_count;
    int token_capacity; 
    int start;
    int current;
    int line_number;
    char line[256];
    Token *tokens;
} Scanner;
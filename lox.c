#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.c"

//* Function declaration
void run_prompt();
void run_file();

int main(int argc, char *argv[]) 
{
    if (argc > 2) 
    {
        printf("Usage %s <filename>\n", argv[0]);
        return 1;
    }
    if (argc == 2)
        run_file(argv[1]);
    else
        run_prompt();
    return 0;
}

void run_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    scan_file(file);
}

void run_prompt()
{
    char line[256];

    for(;;)
    {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        line[strcspn(line, "\n")] = '\0';
        //Need something to run here but what?
    }
}

void report();

void error(int line_number, char *message)
{
    report(line_number, "", message);
}

void report(int line_number, char *where, char* message)
{
    printf("[line %d] Error %s: %s", line_number, where, message);
}


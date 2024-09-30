//* Std
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//* Perso
#include "scanner.h"

//* Function declaration
void run_prompt();
void run_file();

int main(int argc, char *argv[]) {

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
    //! Loop to put in the scanner.h
    char line[256];
    int line_number = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_number++;
        printf("Results for the line #%d \n", line_number);
        scan_tokens(line);
    }
    
    fclose(file);
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

        run(line, 1);
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


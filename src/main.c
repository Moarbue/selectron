#include <stdio.h>

#include "parser.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./selectron <expression>\n");
        return -1; 
    }

    printf("%s = %.7lg\n", argv[1], evaluate(argv[1]));

    return 0;
}
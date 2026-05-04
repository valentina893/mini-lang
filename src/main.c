#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

#define token_max 100

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: ./mini <filename>\n");
        return 0;
    }

    scanner *scanner = scanner_init(argv[1], token_max);
    scanner_get_tokens(scanner);
    _scanner_print_tokens(scanner);
    scanner_delete(scanner);

    return 0;
}
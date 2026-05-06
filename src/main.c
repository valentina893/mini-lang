#include "scanner.h"

#define token_amt 100

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: ./mini <filename>\n");
        return 0;
    }

    scanner *scanner = scanner_init(argv[1], token_amt);
    scanner_get_tokens(scanner);
    _scanner_print_tokens(scanner);
    scanner_delete(scanner);

    return 0;
}
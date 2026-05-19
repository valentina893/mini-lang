#include "evaluator.h"
#include "scanner.h"

#define token_amt 100

int read_args(int argc, char **argv, char *filename) {
    if (argc != 2) {
        printf("mini: no file specified\n");
        return 0;
    }
    if (strstr(argv[1], ".mini") == NULL) {
        printf("mini: file '%s' not of type .mini\n", argv[1]);
        return 0;
    }
    size_t len = strlen(argv[1]);
    strncpy(filename, argv[1], len);
    filename[len] = '\0';
    return 1;
}

int main(int argc, char **argv) {

    char filename[100];
    if (read_args(argc, argv, filename) == 1) {
        scanner *scanner = scanner_init(filename, token_amt);
        if (scanner != NULL) {
            scanner_get_tokens(scanner);
            evaluator *evaluator =  evaluator_init(scanner->tokens_amt, scanner->tokens);
            evaluator_run(evaluator);
            // free evaluator
            scanner_delete(scanner);
        }
    }

    return 0;
}
#include "evaluator.h"
#include "scanner.h"

#define token_amt 100

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: ./mini <filename>\n");
        return 0;
    }

    scanner *scanner = scanner_init(argv[1], token_amt);
    scanner_get_tokens(scanner);

    evaluator *evaluator =  evaluator_init(scanner->tokens_amt, scanner->tokens);
    evaluator_run(evaluator);

    // must free evaluator

    scanner_delete(scanner);

    return 0;
}
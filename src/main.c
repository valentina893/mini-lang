#include "args.h"
#include "evaluator.h"
#include "scanner.h"

#define token_amt 100

int main(int argc, char **argv) {

    args *args = args_init(argc, argv);
    if (args != NULL) {
        scanner *scanner = scanner_init(args, token_amt);
        if (scanner != NULL) {
            scanner_get_tokens(scanner);
            evaluator *evaluator =  evaluator_init(scanner->tokens_amt, scanner->tokens);
            evaluator_run(evaluator);
            // free evaluator
            //scanner_delete(scanner);
        }
    }

    return 0;
}
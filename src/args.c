// args.c

#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

args *args_init(int argc, char **argv) {
    args *args = (struct args*)malloc(sizeof(struct args));
    args->tokens_infix = 0;
    args->tokens_postfix = 0;
    args->filename = NULL;
    int successful = 1;
    for (int i = 1; i < argc; i++) {
        // argument
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-tokens_infix") == 0) {
                args->tokens_infix = 1;
            } else if (strcmp(argv[i], "-tokens_postfix") == 0) {
                args->tokens_postfix = 1;
            } else {
                printf("mini: unknown argument %s\n", argv[i]);
                successful = 0;
            }
        } 
        // filename
        else {
            if (strstr(argv[i], ".mini") == NULL) {
                printf("mini: file '%s' not of type .mini\n", argv[i]);
                successful = 0;
            } else {
                args->filename = argv[i];
            }
        }
    }
    if (args->filename == NULL) {
        printf("mini: no file specified\n");
        successful = 0;
    }
    if (successful == 0) {
        args_delete(&args);
    }
    return args;
}

void args_delete(args **args) {
    if (*args != NULL) {
        free(*args);
        *args = NULL;
    }
}
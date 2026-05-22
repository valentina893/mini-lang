// args.h

#ifndef ARGS_H
#define ARGS_H

typedef struct args {
    int tokens_infix;
    int tokens_postfix;
    char *filename;
} args;

args *args_init(int argc, char **argv);

void args_delete(args *args);

#endif
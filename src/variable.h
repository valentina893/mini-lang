// variable.h

#ifndef VARIABLE_H
#define VARIABLE_H

/*
Data type of the variable.
*/
typedef enum variable_type {
    vINTEGER, vSTRING
} variable_type;

/*
Data payload for variables.
*/
typedef union variable_data {
    int integer;
    char *string;
} variable_data;

/*
Variable class for storing needed data.
*/
typedef struct variable {
    variable_type type;
    int data_size, id_size;
    char *id;
    variable_data data;
} variable;

/*
Initializes variable with a type, id, and stored data.
*/
variable *variable_init(variable_type type, int data_size, int id_size, char *id, variable_data data);

/*
Cleans up variable.
*/
void variable_delete(variable *variable);

#endif
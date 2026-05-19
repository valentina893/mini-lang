// variable.c

#include <stdlib.h>
#include <string.h>

#include "variable.h"

variable *variable_init(variable_type type, int data_size, int id_size, char *id, variable_data data) {

    variable *variable = (struct variable*)malloc(sizeof(struct variable));
    variable->type = type;
    variable->data_size = data_size;
    variable->id_size = id_size;
    variable->id = (char*)malloc(sizeof(char) * id_size);
    strncpy(variable->id, id, id_size);
    variable->id[id_size] = '\0';
    variable->data = data;
    return variable;

}

void variable_delete(variable *variable) {
    if (variable != NULL) {
        free(variable->id);
        if (variable->type == vSTRING) {
            free(variable->data.string);
        }
        free(variable);
    }
}
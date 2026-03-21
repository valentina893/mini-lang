#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max_vars 10
#define buf_capacity 100

typedef struct variable {
    char *name, *value;
} variable;

variable variable_arr[max_vars];
int variable_amt = 0;

void print_variables() {

    for (int i = 0; i < variable_amt; i++) {
        printf("%s = %s\n", variable_arr[i].name, variable_arr[i].value);
    }

}

void variable_assignment_parse_name(long equals_pos, char *line, int *existing_var, variable *variable) {

    char buf[buf_capacity];
    int buf_size = 0;

    int reading = 0;

    for (int i = 0; i < equals_pos; i++) { // parse variable name

        if (line[i] != ' ') {
            if (reading == 0) reading = 1;
                buf[buf_size] = line[i];
                buf_size++;
        } else {
            if (reading == 1) {
                buf[buf_size] = '\0';
                reading = 0;
                break;
            }
        }

    }

    variable->name = (char*)malloc(sizeof(char) * buf_size);
    strcpy(variable->name, buf);

    for (int i = 0; i < variable_amt; i++) { // check if variable already exists
        if (strcmp(buf, variable_arr[i].name) == 0) {
            *existing_var = i; // equals -1 if new, equals i to indicate existing position
            break;
        }
    }

}

void variable_assignment_parse_operation(int line_len, long equals_pos, char *line, char *strchr_op, variable *variable) {

    char first_buf[100], second_buf[100];
    int first_size = 0, second_size = 0;

    long op_pos = strchr_op - line;

    int reading = 0;

    for (int i = equals_pos + 1; i < op_pos; i++) { // parse first operand

        if (line[i] != ' ') {
            if (reading == 0) reading = 1;
                first_buf[first_size] = line[i];
                first_size++;
        } else {
            if (reading == 1) {
                first_buf[first_size] = '\0';
                reading = 0;
                break;
            }
        }

    }

    for (int i = op_pos + 1; i < line_len; i++) { // parse second operand

        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
            if (reading == 0) reading = 1;
                second_buf[second_size] = line[i];
                second_size++;
        } else {
            if (reading == 1) {
                second_buf[second_size] = '\0';
                reading = 0;
                break;
            }
        }

    }

    variable->value = (char*)malloc(sizeof(char) * buf_capacity);

    int op_res = 0;
    if (line[op_pos] == '+') {
        op_res = atoi(first_buf) + atoi(second_buf);
    }

    sprintf(variable->value, "%d", op_res);

}

void variable_assignment_parse_value(int line_len, long equals_pos, char *line, variable *variable) {

    char buf[buf_capacity];
    int buf_size = 0;

    int reading = 0;
    int variable_on_variable = 0;

    for (int i = equals_pos + 1; i < line_len; i++) { // parse single variable value

        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
            if (reading == 0) reading = 1;
                buf[buf_size] = line[i];
                buf_size++;
        } else {
            if (reading == 1) {
                buf[buf_size] = '\0';
                reading = 0;
                break;
            }
        }

    }

    // check if we are assigning variable to variable
    for (int i = 0; i < variable_amt; i++) {
        if (strcmp(buf, variable_arr[i].name) == 0) {
            variable->value = (char*)malloc(sizeof(char) * strlen(variable_arr[i].value));
            strcpy(variable->value, variable_arr[i].value);
            variable_on_variable = 1;
            break;
        }
    }

    if (variable_on_variable == 0) {
        variable->value = (char*)malloc(sizeof(char) * buf_size);
        strcpy(variable->value, buf);
    }


}

void variable_assignment(int line_len, char *line, char *strchr_res) {

    variable variable = {NULL, NULL};
    int existing_var = -1;

    long equals_pos = strchr_res - line;

    variable_assignment_parse_name(equals_pos, line, &existing_var, &variable);

    char operations[] = {'+', '-', '*', '/'};
    int operations_amt = 4;

    char *strchr_op = NULL;

    for (int i = 0; i < operations_amt; i++) {
        strchr_op = strchr(line, operations[i]);
        if (strchr_op != NULL) {
            break;
        }
    }

    if (strchr_op != NULL) {
        variable_assignment_parse_operation(line_len, equals_pos, line, strchr_op, &variable);
    } else {
        variable_assignment_parse_value(line_len, equals_pos, line, &variable);
    }

    if (existing_var >= 0) {
        variable_arr[existing_var].value = (char*)realloc(variable.value, strlen(variable.value));
    } else {
        variable_arr[variable_amt] = variable;
        variable_amt++;
    }

}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: ./mini <filename>\n");
        return 0;
    }

    FILE *fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Unknown file: %s\n", argv[1]);
        fclose(fptr);
        return 0;
    }

    char line[1024];

    while (fgets(line, 100, fptr)) {
        int line_len = strlen(line);
        if (line_len > 0) {
            if (line[0] != '\n') {

                char *strchr_res = strchr(line, '=');

                if (strchr_res != NULL) { // line assigns value to a variable

                    variable_assignment(line_len, line, strchr_res);

                }
            
            }
        }
    }

    print_variables();

    fclose(fptr);
    return 0;
}
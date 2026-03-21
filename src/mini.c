#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max_vars 10
#define buf_capacity 100

typedef enum type {
    integer,
    string,
    none
} type;

typedef struct variable {
    char *name, *value;
    size_t len;
    type _type;
} variable;

// struct for line as well?

int variable_amt = 0;
FILE *fptr = NULL;
char line[1024];
variable variable_arr[max_vars];

int existing_variable(char *name) {

    for (int i = 0; i < variable_amt; i++) {
        if (strcmp(name, variable_arr[i].name) == 0) {
            return i;
        }
    }

    return -1;

}

int parse_variable_name(long assignment_pos, variable *variable) {

    int reading = 0;

    int start = 0;
    int length = 1;

    // read from start to position of '=' char
    for (int i = 0; i < assignment_pos; i++) {

        if (line[i] != ' ' && reading == 0) { // read first character
            reading = 1;
            start = i;
        } else if (line[i] == ' ' && reading == 1) { // read whitespace after name
            break;
        } else if (line[i] != ' ' && reading == 1) { // read next character
            length++;
        }

    }

    variable->name = (char*)malloc(sizeof(char) * length);
    strncpy(variable->name, line + start, length);

    return 1;

}

int parse_variable_value(int line_len, long assignment_pos, variable *variable) {

    int reading = 0;

    int start = assignment_pos + 1;
    int length = 1;

    // read from position of '=' char to end of line
    for (int i = assignment_pos + 1; i < line_len; i++) {

        if (line[i] != ' ' && reading == 0) { // read first character
            reading = 1;
            start = i;
        } else if ((line[i] == ' ' || line[i] == '\n' || line[i] == '\0') && reading == 1) { // read whitespace, newline, or null char after value
            break;
        } else if (line[i] != ' ' && reading == 1) { // read next character
            length++;
        }

    }

    variable->value = (char*)malloc(sizeof(char) * length);
    variable->len = length;
    strncpy(variable->value, line + start, length);

    return 1;

}

void parse_line(int line_len) {

    variable variable = {NULL, NULL, 0, none};

    char *assignment = strchr(line, '=');

    if (assignment != NULL) { // line contains assignment operator

        long assignment_pos = assignment - line;

        if (parse_variable_name(assignment_pos, &variable) != 0) {
            if (parse_variable_value(line_len, assignment_pos, &variable) != 0) {

                int existing_pos = existing_variable(variable.name); // check if overwriting

                if (existing_pos < 0) { // new variable
                    variable_arr[variable_amt] = variable;
                    variable_amt++;
                } else { // existing variable
                    variable_arr[existing_pos].value = (char*)realloc(variable.value, variable.len);
                    free(variable.name);
                }

            } else {
                printf("error parsing value\n");
            }
        } else {
            printf("error parsing name\n");
        }
        
    }

}

void print_variables() {

    for (int i = 0; i < variable_amt; i++) {
        printf("%s = %s\n", variable_arr[i].name, variable_arr[i].value);
    }

}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: ./mini <filename>\n");
        return 0;
    }

    char *extension = strchr(argv[1], '.');

    if (extension == NULL) {
        printf("File %s isn't of type .mini\n", argv[1]);
        return 0;
    } else {
        int extension_pos = extension - argv[1];
        char file_extension[5];
        strncpy(file_extension, argv[1] + extension_pos, 5);
        if (strcmp(file_extension, ".mini") < 0) {
            printf("File %s isn't of type .mini\n", argv[1]);
            return 0;
        }
    }

    fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Unknown file: %s\n", argv[1]);
        fclose(fptr);
        return 0;
    }

    while (fgets(line, 100, fptr)) {
        int line_len = strlen(line);
        if (line_len > 0) {
            if (line[0] != '\n') { // line isn't whitespace

                parse_line(line_len);
            
            }
        }
    }

    print_variables();

    fclose(fptr);
    return 0;
}
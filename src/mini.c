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

int parse_string(int start, int line_len, variable *variable) {

    int length = 0;
    int reading = 1;

    for (int i = start; i < line_len; i++) {
        if (line[i] != '\"') { // read character in string
            length++;
        } else if (line[i] == '\"') { // read end of string
            reading = 0;
            break;
        }
    }

    if (reading == 1) { // never saw an end quote
        printf("error: string not finished with final \"\n");
        return 0;
    }

    variable->value = (char*)malloc(sizeof(char) * length);
    variable->len = length;
    variable->_type = string;
    strncpy(variable->value, line + start, length);

    return 1;

}

int parse_integer(int start, int line_len, variable *variable) {

    int length = 1;

    for (int i = start + 1; i < line_len; i++) {
        if (isdigit(line[i])) { // read character in string
            length++;
        } else if (line[i] == '\n' || line[i] == ' ' || line[i] == '\0') { // read end of string
            break;
        } else { // read non-integer character
            printf("error: cannot have %c in an integer\n", line[i]);
            return 0;
        }
    }

    variable->value = (char*)malloc(sizeof(char) * length);
    variable->len = length;
    variable->_type = integer;
    strncpy(variable->value, line + start, length);

    return 1;

}

int parse_variable(int start, int line_len, variable *variable) {

    char variable_name[buf_capacity];
    strncpy(variable_name, line + start, line_len - start);

    for (int i = 0; i < line_len - start; i++) { // set all white spaces to 0

        if (variable_name[i] == ' ' || variable_name[i] == '\n') {
            variable_name[i] = 0;
        }

    }

    int existing_var = existing_variable(variable_name);

    if (existing_var == -1) {
        printf("error: %s is undefined\n", variable_name);
        return 0;
    }

    variable->value = (char*)malloc(sizeof(char) * variable_arr[existing_var].len);
    variable->len = variable_arr[existing_var].len;
    variable->_type = variable_arr[existing_var]._type;
    strncpy(variable->value, variable_arr[existing_var].value, variable_arr[existing_var].len);
    return 1;

}

int parse_variable_value(int line_len, long assignment_pos, variable *variable) {

    // read from position of '=' char to end of line
    for (int i = assignment_pos + 1; i < line_len; i++) {

        if (line[i] != ' ') { // read first character

            if (line[i] == '\"') { // reading string

                //if (parse_string(i + 1, line_len, variable) == 1) {
                    //break;
                //} else {
                    //return 0;
                //}

                return parse_string(i + 1, line_len, variable);

            } else if (isdigit(line[i])) { // reading integer

                //if (parse_integer(i, line_len, variable) == 1) {
                    //break;
                //} else {
                    //return 0;
                //}

                return parse_integer(i, line_len, variable);

            } else { // reading variable

                return parse_variable(i, line_len, variable);

            }

        }

    }

    return 0;

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
                //printf("error parsing value\n");
            }
        } else {
            //printf("error parsing name\n");
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
// scanner.c

#include <string.h>

#include "scanner.h"

void _scanner_postfix(scanner *scanner) {

    if (scanner != NULL) {
        // initialize stack
        int stack_top = -1;
        const int stack_size = scanner->tokens_amt;
        token *token_stack[stack_size];
        for (int i = 0; i < stack_size; i++) {
            token_stack[i] = NULL;
        }
        // initialize result
        int j = 0;
        token **result = (struct token**)malloc(sizeof(struct token*) * scanner->tokens_amt);
        for (int i = 0; i < scanner->tokens_amt; i++) {
            result[i] = NULL;
        }
        // start reading tokens array
        for (int i = 0; i < scanner->tokens_amt; i++) {
            if (scanner->tokens[i] != NULL) {
                token *curr_token = scanner->tokens[i];
                switch (curr_token->type) {
                    // operands get pushed to result
                    case ID:
                    case INTEGER:
                    case STRING:
                        result[j] = curr_token; j++; break;
                    // '(' gets pushed to stack immediately
                    case LEFT_PARENTHESES: stack_top++; token_stack[stack_top] = curr_token; break;
                    // pop from stack until we see a '('
                    case RIGHT_PARENTHESES:
                        while (stack_top != -1 && token_stack[stack_top]->type != LEFT_PARENTHESES) {
                            result[j] = token_stack[stack_top]; j++; stack_top--;
                        }
                        stack_top--; // discard '('
                        break;
                    // operators
                    case EQUAL:
                    case FUNCTION:
                    case MINUS:
                    case PLUS:
                    case SLASH:
                    case STAR:
                        // pop top token from stack if higher/equal precedence to curr_token
                        while (stack_top != -1 && token_stack[stack_top]->type != LEFT_PARENTHESES
                         && token_prec(token_stack[stack_top]) >= token_prec(curr_token)) {
                            result[j] = token_stack[stack_top]; j++; stack_top--;
                         }
                         // else push curr_token to stack
                         stack_top++; token_stack[stack_top] = curr_token;
                         break;
                    case SEMICOLON:
                         // pop remaining operators
                         while (stack_top != -1) {result[j] = token_stack[stack_top]; j++; stack_top--;}
                         break;
                    default:
                        break;
                }
            }
        }
    
        // set tokens array to result
        token **to_free = scanner->tokens;
        scanner->tokens = result;
        scanner->tokens_amt = j + 1;
        free(to_free);
    
    }

}

void _scanner_resize_tokens(scanner *scanner) {
    if (scanner != NULL) {
        scanner->tokens_max *= 2;
        scanner->tokens = (struct token**)realloc(scanner->tokens, scanner->tokens_max);
    }
}

void _scanner_print_tokens(scanner *scanner) {
    if (scanner != NULL) {
        for (int i = 0; i < scanner->tokens_amt-1; i++) {
            printf("%d -> ", i+1);
            token_print(scanner->tokens[i]);
        }
    }
}

int _scanner_at_end(scanner *scanner) {
    if (scanner != NULL) {
        if (scanner->current >= scanner->src_size) return 1;
    }
    return 0;
}

char _scanner_advance(scanner *scanner) {
    if (scanner != NULL) return scanner->src[scanner->current++];
    return '\0';
}

void _scanner_add_token(scanner *scanner, token_type type, char *value_start, const int length, int literal) {
    if (scanner != NULL) {
        if (scanner->tokens_amt == scanner->tokens_max) {
            _scanner_resize_tokens(scanner);
        }
        scanner->tokens[scanner->tokens_amt] = token_init(type, value_start, length, literal, scanner->line);
        scanner->tokens_amt++;
    }
}

int _scanner_match(scanner *scanner, char expected) {
    if (scanner != NULL) {
        if (_scanner_at_end(scanner) == 1) return 0;
        if (scanner->src[scanner->current] != expected) return 0;

        scanner->current++;
        return 1;
    }
    return 0;
}

char _scanner_peek(scanner *scanner) {
    if (scanner == NULL || _scanner_at_end(scanner) == 1) return '\0';
    return scanner->src[scanner->current];
}

void _scanner_string(scanner *scanner) {

    if (scanner != NULL) {

        // consume each character until current is next to terminating "
        while (_scanner_peek(scanner) != '\"' && _scanner_at_end(scanner) == 0) {
            if (_scanner_peek(scanner) == '\n') scanner->line++;
            _scanner_advance(scanner);
        }

        // check that if we are reading an unterminated string before adding token
        if (_scanner_at_end(scanner) == 0) {
            _scanner_advance(scanner);
            int length = (scanner->current - 1) - (scanner->start + 1);
            char *value_start = scanner->src + scanner->start + 1;
            _scanner_add_token(scanner, STRING, value_start, length, 0);
            return;
        }

        printf("Line: %d:%d, Unterminated string\n", scanner->line, scanner->current);

    }

    return;

}

int _scanner_is_digit(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

void _scanner_integer(scanner *scanner) {
    if (scanner != NULL) {
        // consume all digit characters
        while (_scanner_is_digit(_scanner_peek(scanner))) {
            _scanner_advance(scanner);
        }
        const int length = scanner->current - scanner->start;
        char *value_start = scanner->src + scanner->start;
        // evaluate the literal
        char literal_buf[length];
        strncpy(literal_buf, value_start, length);
        literal_buf[length] = '\0';
        _scanner_add_token(scanner, INTEGER, value_start, length, atoi(literal_buf));
    }
}

int _scanner_is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

int _scanner_is_alpha_numeric(char c) {
    return _scanner_is_alpha(c) || _scanner_is_digit(c);
}

void _scanner_identifier(scanner *scanner) {

    if (scanner != NULL) {
        while (_scanner_is_alpha_numeric(_scanner_peek(scanner))) _scanner_advance(scanner);
        int length = scanner->current - scanner->start;
        char *value_start = scanner->src + scanner->start;
        // check if identifier is function call
        if (_scanner_peek(scanner) == '(') {
            _scanner_add_token(scanner, FUNCTION, value_start, length, 0);
        } else {
            _scanner_add_token(scanner, ID, value_start, length, 0);
        }
    }

}

void _scanner_tokenize(scanner *scanner) {

    if (scanner != NULL) {
        char c = _scanner_advance(scanner);

        switch (c) {
            // single character tokens
            case '-': _scanner_add_token(scanner, MINUS, scanner->src + scanner->start, 1, 0); break;
            case '+': _scanner_add_token(scanner, PLUS, scanner->src + scanner->start, 1, 0); break;
            case '/': _scanner_add_token(scanner, SLASH, scanner->src + scanner->start, 1, 0); break;
            case '*': _scanner_add_token(scanner, STAR, scanner->src + scanner->start, 1, 0); break;
            case ';': _scanner_add_token(scanner, SEMICOLON, scanner->src + scanner->start, 1, 0); break;
            case ')': _scanner_add_token(scanner, RIGHT_PARENTHESES, scanner->src + scanner->start, 1, 0); break;
            case '(': _scanner_add_token(scanner, LEFT_PARENTHESES, scanner->src + scanner->start, 1, 0); break;

            // one or two character tokens
            case '=':
                _scanner_add_token(scanner, _scanner_match(scanner, '=') ? EQUAL_EQUAL : EQUAL, scanner->src + scanner->start, 1, 0);
                break;

            // literals
            case '"':
                _scanner_string(scanner);
                break;

            // ignore whitespaces and newlines
            case ' ': break;
            case '\n': scanner->line++; break;
            
            default:
                if (_scanner_is_digit(c)) {
                    _scanner_integer(scanner);
                } else if (_scanner_is_alpha(c)) {
                    _scanner_identifier(scanner);
                } else {
                    printf("Line: %d, Unexpected character '%c'\n", scanner->line, c);
                } 
                break;
        }
    }

}

scanner *scanner_init(char *src_path, int tokens_max) {

    scanner *scanner = NULL;
    FILE *fptr = NULL;

    if (src_path != NULL) {

        // open src file
        fptr = fopen(src_path, "rb");

        if (fptr != NULL) {

            scanner = (struct scanner*)malloc(sizeof(struct scanner));
            
            // get size of src file
            fseek(fptr, 0, SEEK_END);
            scanner->src_size = ftell(fptr);
            rewind(fptr);

            // allocate memory for char array and stuff with data
            scanner->src = (char*)malloc(sizeof(char) * scanner->src_size + 1);
            fread(scanner->src, 1, scanner->src_size, fptr);
            scanner->src[scanner->src_size] = '\0';

            // init other scanner attributes
            scanner->tokens = (struct token**)malloc(sizeof(struct token*) * tokens_max);
            scanner->tokens_amt = 0;
            scanner->tokens_max = tokens_max;
            scanner->start = 0;
            scanner->current = 0;
            scanner->line = 1;

            fclose(fptr);

        } else {
            printf("Unknown file '%s'\n", src_path);
        }

    }
    
    return scanner;

}

void scanner_get_tokens(scanner *scanner) {

    if (scanner != NULL) {

        while (_scanner_at_end(scanner) == 0) {
            scanner->start = scanner->current;
            _scanner_tokenize(scanner);
        }

        scanner->tokens[scanner->tokens_amt] = token_init(END, NULL, 0, 0, scanner->line);
        scanner->tokens_amt++;

        _scanner_postfix(scanner);

    }

}

void scanner_delete(scanner *scanner) {

    if (scanner != NULL) {

        // free each token in token array
        for (int i = 0; i < scanner->tokens_amt; i++) {
            token_delete(scanner->tokens[i]);
        }
        
        // free token array
        free(scanner->tokens);
        scanner->tokens_amt = 0;

        // free char* storing src
        free(scanner->src);
        scanner->src_size = 0;

        // free scanner class
        free(scanner);

    }

}
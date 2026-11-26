// scanner.c

#include <string.h>

#include "scanner.h"

char _scanner_prev(scanner *scanner) {
    if (scanner != NULL && scanner->current > 0) {
        return scanner->src[scanner->current - 2];
    }
    return '\0';
}

void _scanner_postfix(scanner *scanner) {
    if (scanner != NULL) {
        ststack token_stack = ststack_init(stvector_size(&scanner->tokens));
        //int j = 0;
        //token **result = (struct token**)malloc(sizeof(struct token*) * scanner->tokens_amt);
        stvector result = stvector_init(stvector_size(&scanner->tokens));
        for (size_t i = 0; i < result.m; i++) { result.data[i] = NULL;}
        for (size_t i = 0; i < stvector_size(&scanner->tokens); i++) {
            token *curr_token = NULL;
            if (stvector_at(&scanner->tokens, i, &curr_token) == 1) {
                if (curr_token != NULL) {
                    switch (curr_token->type) {
                        case ID: // operands and breaks get pushed to result
                        case INTEGER:
                        case BREAK:
                        case STRING: 
                        case RIGHT_CURLY: stvector_push(&result, curr_token); break; //result[j] = curr_token; j++; break;
                        case LEFT_PARENTHESES: ststack_push(&token_stack, curr_token); break; // push '(' to stack immediately
                        case RIGHT_PARENTHESES: _scanner_find_left_parentheses(&result, &token_stack); break; // pop until '(' is found
                        case EQUAL: // operators
                        case EQUAL_EQUAL:
                        case GREATER:
                        case GREATER_EQUAL:
                        case IF:
                        case ELIF:
                        case ELSE:
                        case NOT_EQUAL:
                        case FUNCTION:
                        case LESS:
                        case LESS_EQUAL:
                        case WHILE:
                        case MINUS:
                        case PLUS:
                        case SLASH:
                        case STAR: _scanner_handle_operator(scanner, &result, &token_stack, curr_token); break; // pop lower/equal tokens from stack 
                        case LEFT_CURLY:
                            if (scanner->reading_while == 1) scanner->reading_while = 0;
                        case SEMICOLON:
                            while (ststack_size(&token_stack) > 0) stvector_push(&result, ststack_pop(&token_stack)); 
                            break; // pop remaining operators
                        default: break;
                    }
                }
            }
        }
        // set tokens array to result
        token **to_free = stvector_ptr(&scanner->tokens);
        scanner->tokens = result;
        free(to_free);
    }
}

void _scanner_find_left_parentheses(stvector *result, ststack *token_stack) {
    while (ststack_size(token_stack) > 0 && 
            ststack_peek(token_stack)->type != LEFT_PARENTHESES &&
            ststack_peek(token_stack)->type != FUNCTION) {
        stvector_push(result, ststack_pop(token_stack));
    }
    if (ststack_peek(token_stack)->type != FUNCTION) {
        ststack_pop(token_stack);
    }
}

void _scanner_handle_operator(scanner *scanner, stvector *result, ststack *token_stack, token* curr_token) {
    if (curr_token->type == WHILE) {
        stvector_push(result, token_init(LEFT_CURLY, "{", 1, 0, 0));
        scanner->reading_while = 1;

    }
    while (ststack_size(token_stack) > 0 && ststack_peek(token_stack)->type != LEFT_PARENTHESES
            && token_prec(ststack_peek(token_stack)) >= token_prec(curr_token)) {
        stvector_push(result, ststack_pop(token_stack));
    }
    ststack_push(token_stack, curr_token);
}

void _scanner_resize_tokens(scanner *scanner) {
    if (scanner != NULL) {
        //scanner->tokens_max *= 2;
        //token **temp = (struct token**)realloc(scanner->tokens, sizeof(struct token*) * scanner->tokens_max);
        //scanner->tokens = temp;
    }
}

void _scanner_print_tokens(scanner *scanner) {
    if (scanner != NULL) {
        // iterate through all tokens excluding the END token
        for (size_t i = 0; i < stvector_size(&scanner->tokens)-1; i++) {
            printf("%zu -> ", i+1);
            token *t = NULL;
            if (stvector_at(&scanner->tokens, i, &t) == 1) {
                token_print(t);
            }
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
        stvector_push(&scanner->tokens, token_init(type, value_start, length, literal, scanner->line));
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

        int beginning = scanner->line;

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

        printf("mini: line %d, unterminated string\n", beginning);
        scanner->successful = 0;

    }

    return;

}

int _scanner_is_digit(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

void _scanner_integer(scanner *scanner, int negative) {
    if (scanner != NULL) {
        // consume all digit characters
        while (_scanner_is_digit(_scanner_peek(scanner))) {
            _scanner_advance(scanner);
        }
        if (negative == 1) scanner->start += 1;
        const int length = scanner->current - scanner->start;
        char *value_start = scanner->src + scanner->start;
        // evaluate the literal
        char literal_buf[length];
        strncpy(literal_buf, value_start, length);
        literal_buf[length] = '\0';
        int literal = atoi(literal_buf);
        if (negative == 1) literal *= -1;
        _scanner_add_token(scanner, INTEGER, value_start, length, literal);
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

void _scanner_keyword(scanner *scanner, char *value_start, int length) {
    if (scanner != NULL && value_start != NULL) {
        // two-length keywords
        if (length == 2) {
            if (strncmp(value_start, "if", 2) == 0) {
                _scanner_add_token(scanner, IF, value_start, length, scanner->depth++);
            } else {
                _scanner_add_token(scanner, ID, value_start, length, 0);
            }
        }
        // four-length keywords
        else if (length == 4) {
            if (strncmp(value_start, "elif", 4) == 0) {
                _scanner_add_token(scanner, ELIF, value_start, length, scanner->depth++);
            } else if (strncmp(value_start, "else", 4) == 0) {
                _scanner_add_token(scanner, ELSE, value_start, length, scanner->depth++);
            } else {
                _scanner_add_token(scanner, ID, value_start, length, 0);
            }
        }
        // five-length keywords
        else if (length == 5) {
            if (strncmp(value_start, "while", 5) == 0) {
                _scanner_add_token(scanner, WHILE, value_start, length, scanner->depth++);
            } else if (strncmp(value_start, "break", 5) == 0) {
                _scanner_add_token(scanner, BREAK, value_start, length, 0);
            } else {
                _scanner_add_token(scanner, ID, value_start, length, 0);
            }
        }
        // variables
        else {
            _scanner_add_token(scanner, ID, value_start, length, 0);
        }
    }
}

void _scanner_identifier(scanner *scanner) {

    if (scanner != NULL) {
        while (_scanner_is_alpha_numeric(_scanner_peek(scanner))) _scanner_advance(scanner);
        int length = scanner->current - scanner->start;
        char *value_start = scanner->src + scanner->start;
        // check if identifier is function call
        if (_scanner_peek(scanner) == '(') {
            _scanner_add_token(scanner, FUNCTION, value_start, length, 0);
        }
        // check if identifier is keyword or variable 
        else {
            _scanner_keyword(scanner, value_start, length);
        }
    }

}

void _scanner_tokenize(scanner *scanner) {

    if (scanner != NULL) {
        char c = _scanner_advance(scanner);

        switch (c) {
            // single character tokens
            case '+': _scanner_add_token(scanner, PLUS, scanner->src + scanner->start, 1, 0); break;
            case '/': _scanner_add_token(scanner, SLASH, scanner->src + scanner->start, 1, 0); break;
            case '*': _scanner_add_token(scanner, STAR, scanner->src + scanner->start, 1, 0); break;
            case '}': _scanner_add_token(scanner, RIGHT_CURLY, scanner->src + scanner->start, 1, --scanner->depth); break;
            case ')': _scanner_add_token(scanner, RIGHT_PARENTHESES, scanner->src + scanner->start, 1, 0); break;
            case '{': _scanner_add_token(scanner, LEFT_CURLY, scanner->src + scanner->start, 1, 0); break;
            case '(': _scanner_add_token(scanner, LEFT_PARENTHESES, scanner->src + scanner->start, 1, 0); break;
            case ';': _scanner_add_token(scanner, SEMICOLON, scanner->src + scanner->start, 1, 0); break;

            // one or two character tokens
            case '=':
                if (_scanner_match(scanner, '=') == 0) _scanner_add_token(scanner, EQUAL, scanner->src + scanner->start, 1, 0);
                else _scanner_add_token(scanner, EQUAL_EQUAL, scanner->src + scanner->start, 2, 0);
                break;
            case '!':
                 if (_scanner_match(scanner, '=') == 0) {
                    printf("mini: line %d, unexpected character '%c'\n", scanner->line, c);
                    scanner->successful = 0;
                 }
                 else _scanner_add_token(scanner, NOT_EQUAL, scanner->src + scanner->start, 2, 0);
                 break;
            case '<':
                if (_scanner_match(scanner, '=') == 0) _scanner_add_token(scanner, LESS, scanner->src + scanner->start, 1, 0);
                else _scanner_add_token(scanner, LESS_EQUAL, scanner->src + scanner->start, 2, 0);
                break;
            case '>':
                if (_scanner_match(scanner, '=') == 0) _scanner_add_token(scanner, GREATER, scanner->src + scanner->start, 1, 0);
                else _scanner_add_token(scanner, GREATER_EQUAL, scanner->src + scanner->start, 2, 0);
                break;

            // literals
            case '"': _scanner_string(scanner); break;
            case '-':
                 if (_scanner_is_digit(_scanner_peek(scanner)) == 1) _scanner_integer(scanner, 1);
                 else _scanner_add_token(scanner, MINUS, scanner->src + scanner->start, 1, 0);
                 break;

            // ignore whitespaces and newlines
            case ' ': break;
            case '\n': scanner->line++; break;
            case '#':
                 while (_scanner_peek(scanner) != '\n' && _scanner_at_end(scanner) != 1) {
                    _scanner_advance(scanner);
                 }
                 break;
            
            default:
                if (_scanner_is_digit(c)) {
                    _scanner_integer(scanner, 0);
                } else if (_scanner_is_alpha(c)) {
                    _scanner_identifier(scanner);
                } else {
                    printf("mini: line %d, unexpected character '%c'\n", scanner->line, c);
                    scanner->successful = 0;
                } 
                break;
        }
    }

}

scanner *scanner_init(args *args, size_t tokens_max) {

    scanner *scanner = NULL;
    FILE *fptr = NULL;

    if (args != NULL) {

        if (args->filename != NULL) {
            // open src file

            fptr = fopen(args->filename, "rb");

            if (fptr != NULL) {

                scanner = (struct scanner*)malloc(sizeof(struct scanner));
            
                // get size of src file
                fseek(fptr, 0, SEEK_END);
                scanner->src_size = ftell(fptr);
                rewind(fptr);

                // allocate memory for char array and stuff with data
                scanner->src = (char*)malloc(sizeof(char) * (scanner->src_size + 1));
                long res = (int)fread(scanner->src, 1, scanner->src_size, fptr);
                
                if (res == scanner->src_size) {
                    
                    scanner->src[scanner->src_size] = '\0';

                    // init other scanner attributes
                    scanner->tokens = stvector_init(tokens_max);
                    scanner->tokens_max = tokens_max;
                    scanner->start = 0;
                    scanner->current = 0;
                    scanner->line = 1;
                    scanner->depth = 0;
                    scanner->successful = 1;

                    scanner->reading_while = 0;

                    // init cmd line args
                    scanner->tokens_infix = args->tokens_infix;
                    scanner->tokens_postfix = args->tokens_postfix;

                } else {
                    printf("mini: error reading contents of %s into buffer\n", args->filename);
                    free(scanner->src);
                    scanner->src = NULL;
                    free(scanner);
                    scanner = NULL;
                }

                fclose(fptr);

            } else {
                printf("mini: unknown file: '%s'\n", args->filename);
            }

        }
    }
    
    return scanner;

}

int scanner_get_tokens(scanner *scanner) {

    if (scanner != NULL) {

        while (_scanner_at_end(scanner) == 0) {
            scanner->start = scanner->current;
            _scanner_tokenize(scanner);
        }

        stvector_push(&scanner->tokens, token_init(END, NULL, 0, 0, scanner->line));

        if (scanner->successful == 1) {
            if (scanner->tokens_infix == 1) {
                printf("Infix Tokens:\n");
                _scanner_print_tokens(scanner);
            }

            _scanner_postfix(scanner);

            if (scanner->tokens_postfix == 1) {
                printf("Postfix Tokens:\n");
                _scanner_print_tokens(scanner);
            }

            return 1;
        }

    }
    return 0;

}

void scanner_delete(scanner *scanner) {

    if (scanner != NULL) {

        // free each token in token array
        for (size_t i = 0; i < stvector_size(&scanner->tokens); i++) {
            token *t = NULL;
            if (stvector_at(&scanner->tokens, i, &t) == 1) {
                token_delete(&t);
            }
        }
        
        // free token array
        stvector_delete(&scanner->tokens);

        // free char* storing src
        free(scanner->src);
        scanner->src_size = 0;

        // free scanner class
        free(scanner);

    }

}
// scanner.c

#include "scanner.h"

void _scanner_print_tokens(scanner *scanner) {
    if (scanner != NULL) {
        for (int i = 0; i < scanner->tokens_amt-1; i++) {
            printf("%d: %s, len = %d\n", i+1, scanner->tokens[i].lexeme, scanner->tokens[i].size);
        }
    }
}

int _scanner_at_end(scanner *scanner) {
    if (scanner != NULL) {
        if (scanner->current >= scanner->size) return 1;
    }
    return 0;
}

char _scanner_advance(scanner *scanner) {
    if (scanner != NULL) return scanner->src[scanner->current++];
    return '\0';
}

void _scanner_add_token(scanner *scanner, token_type type, char *value_start, const int length) {
    if (scanner != NULL) {
        scanner->tokens[scanner->tokens_amt] = token_init(type, value_start, length, scanner->line);
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
            _scanner_add_token(scanner, STRING, value_start,  length);
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
        int length = scanner->current - scanner->start;
        char *value_start = scanner->src + scanner->start;
        _scanner_add_token(scanner, INTEGER, value_start, length);
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
        _scanner_add_token(scanner, ID, value_start, length);
    }

}

void _scanner_tokenize(scanner *scanner) {

    if (scanner != NULL) {
        char c = _scanner_advance(scanner);

        switch (c) {
            // single character tokens
            case '-': _scanner_add_token(scanner, MINUS, scanner->src + scanner->start, 1); break;
            case '+': _scanner_add_token(scanner, PLUS, scanner->src + scanner->start, 1); break;
            case '/': _scanner_add_token(scanner, SLASH, scanner->src + scanner->start, 1); break;
            case '*': _scanner_add_token(scanner, STAR, scanner->src + scanner->start, 1); break;

            // one or two character tokens
            case '=':
                _scanner_add_token(scanner, _scanner_match(scanner, '=') ? EQUAL_EQUAL : EQUAL, scanner->src + scanner->start, 1);
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

scanner *scanner_init(char *src_path) {

    scanner *scanner = NULL;
    FILE *fptr = NULL;

    if (src_path != NULL) {

        // open src file
        fptr = fopen(src_path, "rb");

        if (fptr != NULL) {

            scanner = (struct scanner*)malloc(sizeof(struct scanner));
            
            // get size of src file
            fseek(fptr, 0, SEEK_END);
            scanner->size = ftell(fptr);
            rewind(fptr);

            // allocate memory for char array and stuff with data
            scanner->src = (char*)malloc(sizeof(char) * scanner->size + 1);
            fread(scanner->src, 1, scanner->size, fptr);
            scanner->src[scanner->size] = '\0';

            // init other scanner attributes
            scanner->start = 0;
            scanner->current = 0;
            scanner->line = 1;
            scanner->tokens_amt = 0;

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

        scanner->tokens[scanner->tokens_amt] = token_init(END, NULL, 0, scanner->line);
        scanner->tokens_amt++;

    }

}
#include "include/lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


lexer_t* lexer_new(char* source, const unsigned long long length)
{
    const auto lexer = (lexer_t*)malloc(sizeof(lexer_t));

    lexer->source = source;
    lexer->i = 0;
    lexer->length = length;

    return lexer;
}

void lexer_advance(lexer_t* lexer)
{
    if (!lexer_can_advance(lexer))
    {
        fprintf(stderr, "EOF reached, can't advance");
        exit(EXIT_FAILURE);
    }

    lexer->i++;
}

bool lexer_can_advance(const lexer_t* lexer)
{
    return lexer->i < lexer->length;
}

char lexer_get_current_char(const lexer_t* lexer)
{
    return lexer->source[lexer->i];
}

token_t lexer_next_token(lexer_t* lexer)
{
    if (!lexer_can_advance(lexer)) return (token_t){
        .value = (token_value_t){ .as_number = 0 },
        .type = TOKEN_TYPE_EOF,
    };

    while (lexer_can_advance(lexer) && isspace(lexer_get_current_char(lexer))) lexer->i++;

    if (!lexer_can_advance(lexer)) return (token_t){
        .value = nullptr,
        .type = TOKEN_TYPE_EOF
    };

    if (isdigit(lexer_get_current_char(lexer)) || lexer_get_current_char(lexer) == '.') return lexer_next_number(lexer);
    if (isalpha(lexer_get_current_char(lexer)) || lexer_get_current_char(lexer) == '_') return lexer_next_identifier(lexer);
    if (lexer_get_current_char(lexer) == '"') return lexer_next_string(lexer);
    return lexer_next_operator(lexer);
}

token_t lexer_next_number(lexer_t* lexer)
{
    const unsigned long long begin = lexer->i;
    bool has_dot = false;

    while (lexer_can_advance(lexer) && (isdigit(lexer_get_current_char(lexer)) || lexer_get_current_char(lexer) == '.'))
    {
        if (lexer_get_current_char(lexer) == '.')
        {
            if (has_dot)
            {
                fprintf(stderr, "Invalid number");
                exit(EXIT_FAILURE);
            }
            has_dot = true;
        }
        lexer_advance(lexer);
    }

    const unsigned long long buffer_size = lexer->i - begin;
    const auto buffer = (char*)malloc(sizeof(char) * buffer_size + 1);
    memcpy(buffer, lexer->source + begin, buffer_size );
    buffer[buffer_size] = '\0';

    const long double value = strtold(buffer, nullptr);

    free(buffer);

    return (token_t){
        .value.as_number = value,
        .type = TOKEN_TYPE_NUMBER,
    };
}

token_t lexer_next_identifier(lexer_t* lexer)
{
    const unsigned long long begin = lexer->i;

    while (lexer_can_advance(lexer) && (
            isalnum(lexer_get_current_char(lexer)) ||
            lexer_get_current_char(lexer) == '#' ||
            lexer_get_current_char(lexer) == '_'
            )) lexer_advance(lexer);

    return (token_t){
        .value.as_string = {
            .string = lexer->source + begin,
            .length = lexer->i - begin,
        },
        .type = TOKEN_TYPE_IDENTIFIER,
    };
}

token_t lexer_next_string(lexer_t* lexer)
{
    lexer_advance(lexer);
    const unsigned long long begin = lexer->i;

    while (lexer_can_advance(lexer) && lexer_get_current_char(lexer) != '"') lexer_advance(lexer);

    const unsigned long long length = lexer->i - begin;

    lexer_advance(lexer);

    return (token_t){
        .value.as_string = {
            .string = lexer->source + begin,
            .length = length,
        },
        .type = TOKEN_TYPE_STRING,
    };
}

token_t lexer_next_operator(lexer_t* lexer)
{
    const char current_char = lexer_get_current_char(lexer);
    switch (current_char)
    {
        case ',': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from(","), .type = TOKEN_TYPE_COMMA };
        case ';': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from(";"), .type = TOKEN_TYPE_SEMICOLON };
        case '(': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("("), .type = TOKEN_TYPE_LPAREN };
        case ')': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from(")"), .type = TOKEN_TYPE_RPAREN };
        case '{': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("{"), .type = TOKEN_TYPE_LBRACE };
        case '}': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("}"), .type = TOKEN_TYPE_RBRACE };
        case '[': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("["), .type = TOKEN_TYPE_LBRACKET };
        case ']': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("]"), .type = TOKEN_TYPE_RBRACKET };

        case '=':
            lexer_advance(lexer);
            if (lexer_get_current_char(lexer) == '=')
            {
                lexer_advance(lexer);
                return (token_t){
                    .value.as_string = string_view_from("=="),
                    .type = TOKEN_TYPE_EQUALS_EQUALS
                };
            }
            return (token_t){
                .value.as_string = string_view_from("="),
                .type = TOKEN_TYPE_EQUALS
            };
        case '<':
            lexer_advance(lexer);
            if (lexer_get_current_char(lexer) == '=')
            {
                lexer_advance(lexer);
                return (token_t){
                    .value.as_string = string_view_from("<="),
                    .type = TOKEN_TYPE_LESS_EQUALS
                };
            }
            return (token_t){
                .value.as_string = string_view_from("<"),
                .type = TOKEN_TYPE_LESS
            };
        case '>':
            lexer_advance(lexer);
            if (lexer_get_current_char(lexer) == '=')
            {
                lexer_advance(lexer);
                return (token_t){
                    .value.as_string = string_view_from(">="),
                    .type = TOKEN_TYPE_GREATER_EQUALS
                };
            }
            return (token_t){
                .value.as_string = string_view_from(">"),
                .type = TOKEN_TYPE_GREATER
            };

        case '+': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("+"), .type = TOKEN_TYPE_PLUS };
        case '-': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("-"), .type = TOKEN_TYPE_MINUS };
        case '*': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("*"), .type = TOKEN_TYPE_ASTERISK };
        case '/': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("/"), .type = TOKEN_TYPE_SLASH };
        default:
            fprintf(stderr, "Unknown character: '%c'\n", current_char);
            exit(EXIT_FAILURE);
    }
}

void lexer_free(lexer_t* lexer)
{
    free(lexer);
}

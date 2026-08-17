#include "include/lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils/throw.h"


lexer_t* lexer_new(char* source, const unsigned long long length)
{
    const auto lexer = (lexer_t*)malloc(sizeof(lexer_t));

    lexer->source = source;
    lexer->i = 0;
    lexer->length = length;
    lexer->commenting = false;

    return lexer;
}

void lexer_advance(lexer_t* lexer)
{
    if (!lexer_can_advance(lexer))
    {
        THROW("EOF reached, can't advance");
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

void lexer_skip_every_unnecessary_shit(lexer_t* lexer)
{
    while (lexer_can_advance(lexer))
    {
        const char current = lexer_get_current_char(lexer);
        if (lexer->commenting)
        {
            if (current == '#') lexer->commenting = false;

            lexer->i++;
            continue;
        }

        if (isspace(current))
        {
            lexer->i++;
            continue;
        }

        if (current == '#')
        {
            lexer->i++;
            lexer->commenting = true;
            continue;
        }

        break;
    }
}

token_t lexer_next_token(lexer_t* lexer)
{
    if (!lexer_can_advance(lexer)) return (token_t){
        .value = (token_value_t){ .as_number = 0 },
        .type = TOKEN_TYPE_EOF,
    };

    lexer_skip_every_unnecessary_shit(lexer);

    if (!lexer_can_advance(lexer)) return (token_t){
        .value = nullptr,
        .type = TOKEN_TYPE_EOF
    };

    char current = lexer_get_current_char(lexer);

    if (isdigit(current)) return lexer_next_number(lexer);
    if (isalpha(current) || current == '_') return lexer_next_identifier(lexer);
    if (current == '"') return lexer_next_string(lexer);
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
                THROW("Invalid number\n");
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
            lexer_get_current_char(lexer) == '_'
            )) lexer_advance(lexer);

    const auto value = (string_view_t) {
        .string = lexer->source + begin,
        .length = lexer->i - begin,
    };
    token_type_t type = TOKEN_TYPE_IDENTIFIER;

    if (string_view_equals_cstr(value, "mutab") ||
        string_view_equals_cstr(value, "const") ||
        string_view_equals_cstr(value, "fn") ||
        string_view_equals_cstr(value, "if") ||
        string_view_equals_cstr(value, "else") ||
        string_view_equals_cstr(value, "while") ||
        string_view_equals_cstr(value, "break") ||
        string_view_equals_cstr(value, "struct") ||
        string_view_equals_cstr(value, "true") ||
        string_view_equals_cstr(value, "false")
        ) type = TOKEN_TYPE_KEYWORD;

    return (token_t){
        .value.as_string = value,
        .type = type,
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
        case '.': lexer_advance(lexer); return (token_t){ .value.as_string = string_view_from("."), .type = TOKEN_TYPE_DOT };
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
            THROW("Unknown character: '%c'\n", current_char);
    }
}

void lexer_free(lexer_t* lexer)
{
    free(lexer);
}

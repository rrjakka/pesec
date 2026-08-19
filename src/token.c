#include "include/token.h"

#include <stdio.h>

void token_print(FILE* stream, const token_t token)
{
    switch (token.type)
    {
        case TOKEN_TYPE_NUMBER:
            fprintf(stream, "number: %Lf", token.value.as_number); break;
        case TOKEN_TYPE_IDENTIFIER:
            fprintf(stream, "identifier: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_KEYWORD:
            fprintf(stream, "keyword: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_STRING:
            fprintf(stream, "string: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_EQUALS:
            fprintf(stream, "assign: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_EQUALS_EQUALS:
        case TOKEN_TYPE_LESS:
        case TOKEN_TYPE_LESS_EQUALS:
        case TOKEN_TYPE_GREATER:
        case TOKEN_TYPE_GREATER_EQUALS:
            fprintf(stream, "comparison: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_PLUS:
        case TOKEN_TYPE_MINUS:
        case TOKEN_TYPE_SLASH:
        case TOKEN_TYPE_ASTERISK:
            fprintf(stream, "operator: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_LPAREN:
        case TOKEN_TYPE_RPAREN:
            fprintf(stream, "paren: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_LBRACE:
        case TOKEN_TYPE_RBRACE:
            fprintf(stream, "brace: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_LBRACKET:
        case TOKEN_TYPE_RBRACKET:
            fprintf(stream, "bracket: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_COMMA:
            fprintf(stream, "comma: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_DOT:
            fprintf(stream, "dot: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_SEMICOLON:
            fprintf(stream, "semicolon: %.*s", token.value.as_string.length, token.value.as_string.data); break;
        case TOKEN_TYPE_EOF:
            fprintf(stream, "end of file"); break;
    }
}

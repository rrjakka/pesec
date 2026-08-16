#include "include/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "include/utils/throw.h"


parser_t *parser_new(lexer_t *lexer)
{
    const auto parser = (parser_t *) malloc(sizeof(parser_t));

    parser->lexer = lexer;
    parser->current_token = lexer_next_token(parser->lexer);

    return parser;
}

bool parser_match(const parser_t *parser, const token_type_t type)
{
    return parser->current_token.type == type;
}

token_t parser_eat(parser_t *parser, const token_type_t type)
{
    if (!parser_match(parser, type))
    {

        fprintf(stderr, "Unexpected token ");
        token_print(stderr, parser->current_token);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
    const token_t prev_token = parser->current_token;
    parser->current_token = lexer_next_token(parser->lexer);

    return prev_token;
}

ast_node_t* parser_check_and_parse_function_call(parser_t *parser, ast_node_t* callee)
{
    if (parser_match(parser, TOKEN_TYPE_LPAREN))
        return parser_parse_function_call(parser, callee);

    return callee;
}

ast_node_t *parser_parse(parser_t *parser)
{
    return parser_parse_statement_sequence(parser, false);
}

ast_node_t *parser_parse_statement_sequence(parser_t *parser, const bool between_braces)
{
    ast_node_t *statement_sequence = statement_sequence_node_new();

    if (between_braces) parser_eat(parser, TOKEN_TYPE_LBRACE);

    while (!parser_match(parser, TOKEN_TYPE_EOF) && (!between_braces || !parser_match(parser, TOKEN_TYPE_RBRACE)))
    {
        statement_sequence_node_push(statement_sequence->node.statement_sequence, parser_parse_statement(parser));
        parser_eat(parser, TOKEN_TYPE_SEMICOLON);
    }
    if (between_braces) parser_eat(parser, TOKEN_TYPE_RBRACE);

    return statement_sequence;
}

ast_node_t *parser_parse_identifier(parser_t *parser)
{
    const string_view_t name = parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string;

    ast_node_t* variable_node = parser_parse_variable(parser, name);

    if (parser_match(parser, TOKEN_TYPE_LPAREN)) return parser_parse_function_call(parser, variable_node);
    if (parser_match(parser, TOKEN_TYPE_EQUALS)) return parser_parse_variable_assignment(parser, name);
    if (parser_match(parser, TOKEN_TYPE_DOT))    return parser_parse_structure_field(parser, name);

    return parser_parse_variable(parser, name);
}

ast_node_t *parser_parse_keyword(parser_t *parser)
{
    const string_view_t name = parser_eat(parser, TOKEN_TYPE_KEYWORD).value.as_string;

    if (string_view_equals_cstr(name, "true")) return literal_node_new(MAKE_VAL_BOOL(true));
    if (string_view_equals_cstr(name, "false")) return literal_node_new(MAKE_VAL_BOOL(false));

    if (string_view_equals_cstr(name, "mutab")) return parser_parse_variable_definition(parser);
    if (string_view_equals_cstr(name, "fn")) return parser_parse_function_definition(parser);
    if (string_view_equals_cstr(name, "struct")) return parser_parse_structure_definition(parser);
    if (string_view_equals_cstr(name, "if")) return parser_parse_if(parser);
    if (string_view_equals_cstr(name, "while")) return parser_parse_while(parser);
    if (string_view_equals_cstr(name, "break")) return parser_parse_break(parser);

    THROW("unknown keyword\n");
}

ast_node_t *parser_parse_variable(parser_t *parser, const string_view_t name)
{
    return variable_node_new(name);
}

ast_node_t *parser_parse_variable_definition(parser_t *parser)
{
    const string_view_t name = parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string;

    ast_node_t *value = literal_node_new(MAKE_VAL_NUM(0));

    if (parser_match(parser, TOKEN_TYPE_EQUALS))
    {
        parser_eat(parser, TOKEN_TYPE_EQUALS);
        value = parser_parse_statement(parser);
    }

    return variable_definition_node_new(name, value);
}

ast_node_t *parser_parse_variable_assignment(parser_t *parser, string_view_t name)
{
    parser_eat(parser, TOKEN_TYPE_EQUALS);
    ast_node_t *value = parser_parse_statement(parser);

    return variable_assignment_node_new(name, value);
}

ast_node_t *parser_parse_function_call(parser_t *parser, ast_node_t* callee)
{
    parser_eat(parser, TOKEN_TYPE_LPAREN);

    ast_node_t *statement_sequence = statement_sequence_node_new();

    if (!parser_match(parser, TOKEN_TYPE_RPAREN))
    {
        statement_sequence_node_push(statement_sequence->node.statement_sequence, parser_parse_statement(parser));

        while (parser_match(parser, TOKEN_TYPE_COMMA))
        {
            parser_eat(parser, TOKEN_TYPE_COMMA);
            statement_sequence_node_push(statement_sequence->node.statement_sequence, parser_parse_statement(parser));
        }
    }
    parser_eat(parser, TOKEN_TYPE_RPAREN);

    return parser_check_and_parse_function_call(parser, function_call_node_new(callee, statement_sequence));
}

ast_node_t *parser_parse_function_definition(parser_t *parser)
{
    parameter_t* parameter = parameter_new();

    parser_eat(parser, TOKEN_TYPE_LPAREN);


    if (!parser_match(parser, TOKEN_TYPE_RPAREN))
    {
        parameter_push(parameter, parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string);

        while (parser_match(parser, TOKEN_TYPE_COMMA))
        {
            parser_eat(parser, TOKEN_TYPE_COMMA);
            parameter_push(parameter, parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string);
        }
    }

    parser_eat(parser, TOKEN_TYPE_RPAREN);

    ast_node_t *body = parser_parse_statement(parser);

    return parser_check_and_parse_function_call(parser, function_definition_node_new(parameter, body));
}

ast_node_t *parser_parse_structure_definition(parser_t *parser)
{
    parameter_t* parameter = parameter_new();
    ast_node_t* values = statement_sequence_node_new();

    parser_eat(parser, TOKEN_TYPE_LBRACE);

    if (!parser_match(parser, TOKEN_TYPE_RBRACE))
    {
        parameter_push(parameter, parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string);
        if (parser_match(parser, TOKEN_TYPE_EQUALS))
        {
            parser_eat(parser, TOKEN_TYPE_EQUALS);
            statement_sequence_node_push(values->node.statement_sequence, parser_parse_statement(parser));
        }
        else
        {
            statement_sequence_node_push(values->node.statement_sequence, literal_node_new(MAKE_VAL_NUM(0)));
        }

        while (parser_match(parser, TOKEN_TYPE_COMMA))
        {
            parser_eat(parser, TOKEN_TYPE_COMMA);
            parameter_push(parameter, parser_eat(parser, TOKEN_TYPE_IDENTIFIER).value.as_string);
            if (parser_match(parser, TOKEN_TYPE_EQUALS))
            {
                parser_eat(parser, TOKEN_TYPE_EQUALS);
                statement_sequence_node_push(values->node.statement_sequence, parser_parse_statement(parser));
            }
            else
            {
                statement_sequence_node_push(values->node.statement_sequence, literal_node_new(MAKE_VAL_NUM(0)));
            }
        }
    }

    parser_eat(parser, TOKEN_TYPE_RBRACE);

    return structure_definition_node_new(parameter, values);
}

ast_node_t *parser_parse_structure_field(parser_t *parser, const string_view_t name)
{
    parser_eat(parser, TOKEN_TYPE_DOT);

    const token_t field = parser_eat(parser, TOKEN_TYPE_IDENTIFIER);

    return structure_field_node_new(name, field.value.as_string);
}

ast_node_t *parser_parse_if(parser_t *parser)
{
    ast_node_t *condition = parser_parse_statement(parser);

    ast_node_t *if_body = parser_parse_statement(parser);
    ast_node_t *else_body = nullptr;

    if (parser_match(parser, TOKEN_TYPE_KEYWORD) &&
        string_view_equals_cstr(parser->current_token.value.as_string, "else"))
    {
        parser_eat(parser, TOKEN_TYPE_KEYWORD);
        else_body = parser_parse_statement(parser);
    }

    return condition_node_new(condition, if_body, else_body);
}

ast_node_t *parser_parse_while(parser_t *parser)
{
    ast_node_t *condition = parser_parse_statement(parser);

    ast_node_t *while_body = parser_parse_statement(parser);

    return while_loop_node_new(condition, while_body);
}

ast_node_t *parser_parse_break(parser_t *parser)
{
    ast_node_t* break_body = nullptr;

    if (!parser_match(parser, TOKEN_TYPE_SEMICOLON))
    {
        break_body = parser_parse_statement(parser);
    }

    return break_node_new(break_body);
}

ast_node_t* parser_parse_statement(parser_t* parser)
{
    return parser_parse_comparison(parser);
}

ast_node_t* parser_parse_comparison(parser_t* parser)
{
    ast_node_t *left = parser_parse_expression(parser);
    ast_node_t *right = nullptr;

    while (parser_match(parser, TOKEN_TYPE_EQUALS_EQUALS) ||
        parser_match(parser, TOKEN_TYPE_LESS_EQUALS) ||
        parser_match(parser, TOKEN_TYPE_LESS) ||
        parser_match(parser, TOKEN_TYPE_GREATER_EQUALS) ||
        parser_match(parser, TOKEN_TYPE_GREATER))
    {
        const token_t operation = parser_eat(parser, parser->current_token.type);

        right = parser_parse_expression(parser);
        left = binary_op_node_new(operation, left, right);
    }

    return left;
}

ast_node_t *parser_parse_expression(parser_t *parser)
{
    ast_node_t *left = parser_parse_term(parser);
    ast_node_t *right = nullptr;

    while (parser_match(parser, TOKEN_TYPE_PLUS) ||
           parser_match(parser, TOKEN_TYPE_MINUS))
    {
        const token_t operation = parser_eat(parser, parser->current_token.type);

        right = parser_parse_term(parser);
        left = binary_op_node_new(operation, left, right);
    }

    return left;
}

ast_node_t *parser_parse_term(parser_t *parser)
{
    ast_node_t *left = parser_parse_factor(parser);
    ast_node_t *right = nullptr;

    while (parser_match(parser, TOKEN_TYPE_ASTERISK) ||
           parser_match(parser, TOKEN_TYPE_SLASH))
    {
        const token_t operation = parser_eat(parser, parser->current_token.type);

        right = parser_parse_factor(parser);
        left = binary_op_node_new(operation, left, right);
    }

    return left;
}

ast_node_t *parser_parse_factor(parser_t *parser)
{
    ast_node_t *node = nullptr;

    switch (parser->current_token.type)
    {
        case TOKEN_TYPE_NUMBER:
            node = literal_node_new(MAKE_VAL_NUM(parser_eat(parser, TOKEN_TYPE_NUMBER).value.as_number));
            break;
        case TOKEN_TYPE_STRING:
            node = literal_node_new(MAKE_VAL_STR(parser_eat(parser, TOKEN_TYPE_STRING).value.as_string));
            break;
        case TOKEN_TYPE_IDENTIFIER:
            node = parser_parse_identifier(parser);
            break;
        case TOKEN_TYPE_KEYWORD:
            node = parser_parse_keyword(parser);
            break;
        case TOKEN_TYPE_LPAREN:
            parser_eat(parser, TOKEN_TYPE_LPAREN);
            node = parser_parse_statement(parser);
            parser_eat(parser, TOKEN_TYPE_RPAREN);

            node = parser_check_and_parse_function_call(parser, node);

            break;
        case TOKEN_TYPE_LBRACE:
            node = parser_parse_statement_sequence(parser, true);
            break;
        default:
            break;
    }

    if (!node)
    {
        THROW("unexpected token type '%d'", parser->current_token.type);
    }

    return node;
}

void parser_free(parser_t *parser)
{
    free(parser);
}

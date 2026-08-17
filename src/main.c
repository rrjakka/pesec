#include <stdio.h>
#include <stdlib.h>

#include "include/lexer.h"
#include "include/parser.h"
#include "include/context.h"
#include "include/function_value.h"
#include "include/utils/throw.h"




#define LAMBDA(return_type, function_body) ({ return_type __lambda_function__ function_body __lambda_function__; })


static value_t println(context_t* context)
{
    const context_item_t* value = context_get(context, string_view_from("value"));
    value_print(value->value);
    printf("\n");
    return MAKE_VAL_NUM(0);
}


static void init_io(const context_t* context)
{
    parameter_t* parameter = parameter_new();

    parameter_push_from_cstr(parameter, (const char*[]){ "value", nullptr });

    context_push(
        context,
        string_view_from("println"),
        MAKE_VAL_FUNC(
            function_value_new(
                parameter,
                (function_value_value_t) {
                    .as_c_function = println
                },
                FUNCTION_VALUE_TYPE_C_FUNCTION
            )
        ),
        false
        );
}

int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: pesec <file>\n");
        return EXIT_SUCCESS;
    }

    const char* filename = argv[1];
    char* source = nullptr;
    unsigned long long source_size = 0;

    FILE* file = fopen(filename, "r");

    if (!file)
    {
        THROW("Could not open file %s\n", filename);
    }

    fseek(file, 0, SEEK_END);
    source_size = ftell(file);
    fseek (file, 0, SEEK_SET);
    source = (char*)malloc(source_size);
    fread(source, 1, source_size, file);
    fclose(file);

    lexer_t* lexer = lexer_new(source, source_size);
    parser_t* parser = parser_new(lexer);
    ast_node_t* ast = parser_parse(parser);
    context_t* context = context_new(nullptr);

    init_io(context);

    const value_t result = ast_node_evaluate(ast, context);

    switch (result.control_flow)
    {
    case CONTROL_FLOW_BREAK:
        THROW("Break outside of loop\n");
        break;
    case CONTROL_FLOW_CONTINUE:
        THROW("Continue outside of loop\n");
        break;
    default:
        break;
    }

    context_free(context);
    ast_node_free(ast);
    lexer_free(lexer);
    parser_free(parser);

    return EXIT_SUCCESS;
}

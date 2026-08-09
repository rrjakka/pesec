cmake_minimum_required(VERSION 4.3)
project(pesec)

set(CMAKE_C_STANDARD 23)

add_executable(
        ${PROJECT_NAME}
        src/main.c
        src/lexer.c
        include/lexer.h
        src/token.c
        include/token.h
        src/parser.c
        include/parser.h
        src/ast/ast_node.c
        include/ast/ast_node.h
        src/ast/binary_op_node.c
        include/ast/binary_op_node.h
        src/ast/literal_node.c
        include/ast/literal_node.h
        include/context.h
        src/context.c
        include/utils/string_view.h
        include/ast/variable_node.h
        src/ast/variable_node.c
        include/ast/function_call_node.h
        src/ast/function_call_node.c
        src/ast/statement_sequence_node.c
        include/ast/statement_sequence_node.h
        src/utils/string_view.c
        src/value.c
        include/ast/variable_definition_node.h
        src/ast/variable_definition_node.c
        src/ast/variable_assignment_node.c
        include/ast/condition_node.h
        src/ast/condition_node.c
        src/ast/while_loop_node.c
        include/ast/while_loop_node.h
        include/ast/while_loop_node.h
        include/ast/break_node.h
        src/ast/break_node.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
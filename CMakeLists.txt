cmake_minimum_required(VERSION 4.3)
project(pesec)

set(CMAKE_C_STANDARD 23)

add_executable(
        ${PROJECT_NAME}
        src/main.c
        src/context.c                               include/context.h
        src/lexer.c                                 include/lexer.h
        src/token.c                                 include/token.h
        src/parser.c                                include/parser.h
        src/value.c                                 include/value.h
        src/function_value.c                        include/function_value.h
        src/control_flow.c                          include/control_flow.h

        src/utils/string_view.c                     include/utils/string_view.h

        src/ast/ast_node.c                          include/ast/ast_node.h
        src/ast/binary_op_node.c                    include/ast/binary_op_node.h
        src/ast/literal_node.c                      include/ast/literal_node.h
        src/ast/variable_node.c                     include/ast/variable_node.h
        src/ast/function_call_node.c                include/ast/function_call_node.h
        src/ast/function_definition_node.c          include/ast/function_definition_node.h
        src/ast/statement_sequence_node.c           include/ast/statement_sequence_node.h
        src/ast/variable_definition_node.c          include/ast/variable_definition_node.h
        src/ast/variable_assignment_node.c          include/ast/variable_assignment_node.h

        src/ast/condition_node.c                    include/ast/condition_node.h
        src/ast/while_loop_node.c                   include/ast/while_loop_node.h
        src/ast/break_node.c                        include/ast/break_node.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
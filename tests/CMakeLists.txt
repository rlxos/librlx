add_executable(exec_env exec_env.cc)
add_test(
    NAME "Test-Execution-Environment"
    COMMAND exec_env
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
include(GNUInstallDirs)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/rlx/path
    FILES_MATCHING PATTERN "*.hh*"
)

add_executable(test_has_ext tests/path-has-ext.cc)
add_test(NAME "Path-Has-Extension"
    COMMAND test_has_ext)
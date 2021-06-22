include(GNUInstallDirs)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/rlx/utils
    FILES_MATCHING PATTERN "*.hh*"
)
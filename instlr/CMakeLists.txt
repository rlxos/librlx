add_library(instlr install.cc verify.cc prepare.cc bootloader.cc)
target_link_libraries(instlr yaml-cpp)

install(FILES instlr.hh DESTINATION include/rlx/instlr)
install(TARGETS instlr
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

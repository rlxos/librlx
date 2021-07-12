add_library(curlcpp STATIC SHARED curl.cc)
target_link_libraries(curlcpp curl)

install(FILES curl.hh DESTINATION include/rlx/curl)
install(TARGETS curlcpp 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
add_library(cinnabar_lexer STATIC kw_id_scan.cpp scaner.cpp lexer.cpp utils.cpp)
target_include_directories(cinnabar_lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_lexer PRIVATE fmt::fmt)

install(TARGETS cinnabar_lexer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

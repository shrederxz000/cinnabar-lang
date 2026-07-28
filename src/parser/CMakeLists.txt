add_library(cinnabar_parser STATIC parser.cpp expr.cpp stmt.cpp utils.cpp)
target_include_directories(cinnabar_parser PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_parser PRIVATE cinnabar_lexer)

install(TARGETS cinnabar_parser
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

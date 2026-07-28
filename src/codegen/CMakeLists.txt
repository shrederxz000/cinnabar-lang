add_library(cinnabar_codegen STATIC serializer.cpp)
target_include_directories(cinnabar_codegen PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_codegen PRIVATE cinnabar_lexer cinnabar_parser)

install(TARGETS cinnabar_codegen
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

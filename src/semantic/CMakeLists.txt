add_library(cinnabar_semantic STATIC semantic.cpp expr.cpp scope.cpp stmt.cpp std.cpp typechecker.cpp)
target_include_directories(cinnabar_semantic PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_semantic PRIVATE cinnabar_lexer)

install(TARGETS cinnabar_semantic
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

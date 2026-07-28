```
# root CMakeLists.txt
cmake_minimum_required(VERSION 4.0)
project(Cinnabar)
set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/include)
include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.2.0)
FetchContent_MakeAvailable(fmt)
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)

# src/CMakeLists.txt
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantic)
add_subdirectory(codegen)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE cinnabar_parser cinnabar_lexer cinnabar_semantic cinnabar_codegen fmt::fmt) 

# tests/CMakeLists.txt
file(GLOB_RECURSE LEXER_TESTS CONFIGURE_DEPENDS lexer/*.cpp)
file(GLOB_RECURSE PARSER_TESTS CONFIGURE_DEPENDS parser/*.cpp)
add_executable(cinnabar_tests ${LEXER_TESTS})
target_link_libraries(cinnabar_tests PRIVATE cinnabar_lexer cinnabar_parser GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(cinnabar_tests)

# src/codogen/CMakeLists.txt
add_library(cinnabar_codegen STATIC serializer.cpp)
target_include_directories(cinnabar_codegen PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_codegen PRIVATE cinnabar_lexer cinnabar_parser)

# src/lexer/CMakeLists.txt
add_library(cinnabar_lexer STATIC kw_id_scan.cpp scaner.cpp lexer.cpp utils.cpp)
target_include_directories(cinnabar_lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_lexer PRIVATE fmt::fmt)

# src/parser/CMakeLists.txt
add_library(cinnabar_parser STATIC parser.cpp expr.cpp stmt.cpp utils.cpp)
target_include_directories(cinnabar_parser PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_parser PRIVATE cinnabar_lexer)

# src/semantic/CMakeLists.txt
add_library(cinnabar_semantic STATIC semantic.cpp expr.cpp scope.cpp stmt.cpp std.cpp typechecker.cpp)
target_include_directories(cinnabar_semantic PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cinnabar_semantic PRIVATE cinnabar_lexer)
```

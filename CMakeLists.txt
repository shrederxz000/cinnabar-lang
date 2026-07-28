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

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

add_subdirectory(src)
add_subdirectory(tests)

# --- Установка ассетов, если папка assets/ существует ---
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} 
    )
endif()

# --- Установка публичных заголовков ---
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)




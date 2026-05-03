cmake_minimum_required(VERSION 3.28)
project(RegexLibrary)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)
include_directories(headers)
include_directories(headers/AST)
include_directories(headers/DKA)
include_directories(headers/NKA)

add_library(regex_lib STATIC
    src/Lexer.cpp
        src/Parser.cpp
        src/Pattern.cpp
        src/Reg_Handler.cpp
        src/my_reg.cpp
        src/min_dka.cpp
        src/Automat_NKA.cpp
        src/Builder_NKA.cpp
        src/Builder_DKA.cpp
)

add_executable(main_reg main.cpp)
target_link_libraries(main_reg regex_lib)

add_executable(my_test tests/test.cpp)
enable_testing()
target_link_libraries(my_test PRIVATE
        regex_lib
        Catch2::Catch2WithMain
)
add_test(NAME MyTests COMMAND my_test)


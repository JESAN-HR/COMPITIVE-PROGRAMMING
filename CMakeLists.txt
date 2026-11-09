cmake_minimum_required(VERSION 3.25)
project(COMPITIVE-PROGRAMMING LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles([=[
    #include <algorithm>
    #include <array>
    #include <span>

    constexpr int language_mode() {
        if consteval { return 23; }
        else { return 0; }
    }
    static_assert(language_mode() == 23);

    int main() {
        std::array values{2, 1};
        std::ranges::sort(std::span{values});
        return values.front() - 1;
    }
]=] CP_HAS_REQUIRED_CXX23_SUPPORT)
if(NOT CP_HAS_REQUIRED_CXX23_SUPPORT)
    message(FATAL_ERROR "A C++23 toolchain with if consteval, std::span and ranges is required. See docs/getting-started.md.")
endif()

add_library(cp INTERFACE)
add_library(cp::cp ALIAS cp)
target_include_directories(cp INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_compile_features(cp INTERFACE cxx_std_23)

function(cp_add_program target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE cp::cp)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()
endfunction()

include(CTest)

cp_add_program(contest_template templates/contest.cpp)

if(BUILD_TESTING)
    function(cp_add_io_test name target expected_status)
        add_test(NAME ${name} COMMAND "${CMAKE_COMMAND}"
            "-DPROGRAM=$<TARGET_FILE:${target}>"
            "-DINPUT_FILE=${PROJECT_SOURCE_DIR}/tests/cases/${name}.in"
            "-DEXPECTED_FILE=${PROJECT_SOURCE_DIR}/tests/cases/${name}.out"
            "-DEXPECTED_STATUS=${expected_status}"
            -P "${PROJECT_SOURCE_DIR}/tests/check_program.cmake")
    endfunction()

    cp_add_program(utility_tests tests/utility_tests.cpp)
    add_test(NAME utility_tests COMMAND utility_tests)
    cp_add_io_test(contest_template_demo contest_template 0)
endif()

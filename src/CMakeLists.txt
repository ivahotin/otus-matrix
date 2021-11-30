cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

configure_file(
  versiondef.h.in 
  ${CMAKE_CURRENT_BINARY_DIR}/versiondef.h
)

add_executable(matrix main.cpp)
add_library(matrix_lib version.cpp)

set_target_properties(matrix matrix_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    matrix_lib
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(matrix PRIVATE
matrix_lib
)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    target_compile_options(matrix PRIVATE
        /W4
    )
    target_compile_options(matrix_lib PRIVATE
        /W4
    )
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror -Wshadow -fmax-errors=2
    )
    target_compile_options(matrix_lib PRIVATE
        -Wall -Wextra -pedantic -Werror -Wshadow -fmax-errors=2
    )
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
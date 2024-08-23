cmake_minimum_required( VERSION 3.10 )
project( homework7 VERSION 0.1.1 )

find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.0 REQUIRED)

set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(LEXER_OUT "${PARSER_DIR}/scan.cpp")
set(PARSER_OUT "${PARSER_DIR}/parser.cpp")

flex_target(LEXER "scan.l" "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/scan.h")
bison_target(PARSER "parser.y" "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/parser.h")
add_flex_bison_dependency(LEXER PARSER)

add_executable(bulk bulk.cpp "${LEXER_OUT}" "${PARSER_OUT}")
add_executable( bulk_test bulk_test.cpp "${LEXER_OUT}" "${PARSER_OUT}")

target_include_directories(bulk PRIVATE "${PARSER_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(bulk_test PRIVATE "${PARSER_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")


# package part which enables
# run: cmake --build . --target package
install(TARGETS bulk RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_CONTACT berrendej@gmail.com)
include(CPack)

#testing part which enables
# cmake --build . --target test
enable_testing()
add_test(bulk_test bulk_test) 
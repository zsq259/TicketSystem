cmake_minimum_required(VERSION 3.10.2)
project(bookstore)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast")

#add_executable(code ${src_dir} src/main.cpp src/parser.cpp src/user.cpp src/train.cpp src/ticket.cpp)
add_executable(code ${src_dir} b+tree/main.cpp)


set(test_prefix "${PROJECT_SOURCE_DIR}/data")
file(GLOB tests RELATIVE "${test_prefix}" data/*)

foreach (test-name IN ITEMS ${tests})
    if (NOT IS_DIRECTORY "${test_prefix}/${test-name}")
        continue()
    endif ()
    add_test(NAME "${test-name}"
            COMMAND bash ../run-test.sh "${test_prefix}" "${test-name}" "$<TARGET_FILE:code>"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/run
            )
endforeach ()

enable_testing()

# 'Google_tests' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(C:/Users/Pawel/Desktop/clion_projects/lib lib)
include_directories(lib/googletest/include/ lib/googletest)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests

add_executable(Task3_tests_run ../Task_3/trim.cpp Task3_testing.cpp)

target_link_libraries(Task3_tests_run gtest gtest_main)
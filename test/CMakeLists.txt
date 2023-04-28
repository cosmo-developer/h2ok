cmake_minimum_required(VERSION 3.10)
project(test)
add_executable(testh2ok "test.cc")
target_link_libraries(testh2ok h2ok)
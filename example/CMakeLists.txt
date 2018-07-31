cmake_minimum_required(VERSION 2.8.11)
project(big-number-example)

include_directories(${PROJECT_SOURCE_DIR}/../lib)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} big-number-lib)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

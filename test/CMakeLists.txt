cmake_minimum_required(VERSION 2.8.11)
project(big-number-test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Test REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/../lib
                    ${Qt5Test_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} big-number-lib Qt5::Test)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

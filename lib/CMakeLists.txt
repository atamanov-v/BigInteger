cmake_minimum_required(VERSION 2.8.11)
project (big-number-lib)

set (CMAKE_CXX_STANDARD 11)
set (PROJECT_NAME "big-number-lib")
set (VERSION "0.1")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -Wcast-align \
                                        -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
                                        -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs \
                                        -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
                                        -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 \
                                        -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses \
                                        -fdiagnostics-show-option")

include_directories(${PROJECT_BINARY_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} SRCS)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_link_libraries(${PROJECT_NAME} ${LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

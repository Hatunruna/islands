cmake_minimum_required(VERSION 3.0)

project(ISLANDS
  VERSION 0.1.0
)

find_package(gf REQUIRED)


set(ISLANDS_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/islands")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -O2 -std=c++11)
endif()

add_executable(islands
  code/islands.cc
  code/local/Sea.cc
)

target_include_directories(islands
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(islands gf::gf0)

install(
  TARGETS islands
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/islands"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

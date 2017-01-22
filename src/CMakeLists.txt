cmake_minimum_required(VERSION 3.0)

project(ISLANDS
  VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(gf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-audio>=2.1)

set(ISLANDS_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/islands")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -O2 -std=c++11)
endif()

add_executable(islands
  code/islands.cc
  code/local/Compass.cc
  code/local/Confetti.cc
  code/local/Decoration.cc
  code/local/DecorationManager.cc
  code/local/Gold.cc
  code/local/Hero.cc
  code/local/Sea.cc
  code/local/Singletons.cc
  code/local/Steam.cc
  code/local/Treasure.cc
  code/local/TreasureManager.cc
  code/local/TurretManager.cc
  code/local/WaveManager.cc
)

target_include_directories(islands
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(islands
  gf::gf0
  ${SFML2_LIBRARIES}
)

install(
  TARGETS islands
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/islands"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

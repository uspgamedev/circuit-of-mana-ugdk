
cmake_minimum_required(VERSION 2.8.11)
project(circuit-of-mana)

find_package(Git)
if (NOT GIT_FOUND)
  message (FATAL_ERROR "This project requires Git!")
endif (NOT GIT_FOUND)

if (EXISTS ${CMAKE_SOURCE_DIR}/local/ugdk)
  message (STATUS "Found local ugdk!")
  set (UGDK_SUBDIR ${CMAKE_SOURCE_DIR}/local/ugdk)
else (EXISTS ${CMAKE_SOURCE_DIR}/local/ugdk)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/externals/ugdk/CMakeLists.txt")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endif()
  set (UGDK_SUBDIR externals/ugdk)
endif ()

if (CMAKE_COMPILER_IS_GNUCC)
  add_definitions (-std=gnu++0x)
endif ()

add_custom_target (
  assets
  COMMAND ${GIT_EXECUTABLE} archive
          --remote=git://uspgamedev.org/circuit-ugdk-resources.git
          -o ${CMAKE_CURRENT_BINARY_DIR}/assets/resources.tar.gz
          54f04718d66a1fd8fa5ff76caa596edb5c4f2cf1 resources/
  COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_BINARY_DIR}/assets/resources.tar.gz
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/
)

add_subdirectory(${UGDK_SUBDIR})

add_subdirectory(src)



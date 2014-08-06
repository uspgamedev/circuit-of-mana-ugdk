
cmake_minimum_required(VERSION 2.8.11)
project(circuit-of-mana)

if (EXISTS ${CMAKE_SOURCE_DIR}/local/ugdk)
  message (STATUS "Found local ugdk!")
  set (UGDK_SUBDIR ${CMAKE_SOURCE_DIR}/local/ugdk)
else (EXISTS ${CMAKE_SOURCE_DIR}/local/ugdk)
  find_package(Git)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/externals/ugdk/CMakeLists.txt")
      if(GIT_FOUND)
          execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
      else(GIT_FOUND)
          message(FATAL_ERROR "Git submodule 'externals/ugdk' not initialized and git not found.")
      endif(GIT_FOUND)
  endif()
  set (UGDK_SUBDIR externals/ugdk)
endif ()

if (CMAKE_COMPILER_IS_GNUCC)
  add_definitions (-std=gnu++0x)
endif ()

add_subdirectory(${UGDK_SUBDIR})

add_subdirectory(src)


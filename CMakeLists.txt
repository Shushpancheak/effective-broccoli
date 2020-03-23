cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

project (
  "effective-brocolli"
  VERSION 1.0.0
  DESCRIPTION ""
)

include_directories("include")

set (
  SOURCES
  src/Entity.cpp
  src/ResourceManager.cpp
)
set (
  HEADERS
  include/Entity.hpp
  include/ResourceManager.hpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} src/main.cpp)

set(BUILD_TESTS OFF CACHE BOOL "Build tests")
if (BUILD_TESTS)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - GTEST INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(googletest)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - TESTS - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(UNIT_TESTS_NAME "run-tests-${PROJECT_NAME}")
set(UNIT_TESTS_FILE "test/test.cpp")

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE})

# Link test executable against gtest & gtest_main
target_link_libraries(${UNIT_TESTS_NAME} gtest_main ${LIBRARY_NAME})
add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next libraries.
set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

endif() # BUILD_TESTS

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - DEPENDENCIES- - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(submodules)

# - - - - - - - - - - - - - - - - -SFML - - - - - - - - - - - - - - - - - - - -

# Chose a way to find the SFML.
if (MSVC) # WINDOWS

target_link_libraries(
  ${PROJECT_NAME}
  sfml-graphics
  sfml-audio
  sfml-system
)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "submodules/sfml-compiled/lib/cmake/SFML")

include_directories(submodules/sfml-compiled/include)

else() # LINUX

target_link_libraries(
${PROJECT_NAME} sfml-graphics sfml-audio sfml-system
)

endif() # SFML

find_package(SFML 2.5 COMPONENTS graphics audio system REQUIRED)
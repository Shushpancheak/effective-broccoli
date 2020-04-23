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

include_directories(include)

set (
  SOURCES
  src/QuadTree.cpp
  src/ResourceManager.cpp
  src/GraphicalComponent.cpp
  src/TransformComponent.cpp
  src/ComponentManager.cpp
  src/EntityManager.cpp
  src/ObjectPool.cpp
  src/DataChunk.cpp
  include/constants/error.cpp
  include/support/result.cpp
  src/SystemManager.cpp
  src/GraphicalSystem.cpp
)
set (
  HEADERS
  include/components/PhysicalComponent.hpp
  include/components/TransformComponent.hpp
  include/systems/PhysicalSystem.hpp
  include/QuadTree.hpp
  include/Entity.hpp
  include/systems/System.hpp
  include/managers/SystemManager.hpp
  include/managers/ResourceManager.hpp
  include/components/Component.hpp
  include/components/GraphicalComponent.hpp
  include/managers/ComponentManager.hpp
  include/managers/EntityManager.hpp
  include/memory/ObjectPool.hpp
  include/memory/DataChunk.hpp
  include/constants/error.hpp
  include/support/result.hpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} src/main.cpp)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - GTEST INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(BUILD_TESTS OFF CACHE BOOL "Build tests")
if (BUILD_TESTS)

add_subdirectory(submodules/googletest)
include_directories(submodules/googletest/googletest/include)

enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR})

# - - - - - - - - - - - - - - - - - TESTS - - - - - - - - - - - - - - - - - - -

set(UNIT_TESTS_NAME "run-tests-${PROJECT_NAME}")
set(UNIT_TESTS_FILE
        "test/test.cpp"
        "test/QuadTreeTest.cpp"
)

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE} ${HEADERS} ${SOURCES})

# Link test executable against gtest & gtest_main & sfml
target_link_libraries(${UNIT_TESTS_NAME} gtest_main sfml-graphics sfml-audio sfml-system)

add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next_ libraries.
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
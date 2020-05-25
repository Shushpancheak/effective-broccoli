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
  src/GraphicalComponent.cpp
  src/TransformComponent.cpp
  src/PhysicalComponent.cpp
  
  src/ResourceManager.cpp
  src/ComponentManager.cpp
  src/EntityManager.cpp
  src/EventManager.cpp
  src/SystemManager.cpp

  src/DataChunk.cpp
  src/MapLoader.cpp
  
  src/GraphicalSystem.cpp
  src/TransformSystem.cpp
  src/PhysicalSystem.cpp
  src/ViewSystem.cpp
  src/AiSystem.cpp
)

set (
  HEADERS
  include/engine/core.hpp
  include/engine/main.hpp

  include/components/Component.hpp
  include/components/GraphicalComponent.hpp
  include/components/PhysicalComponent.hpp
  include/components/TransformComponent.hpp

  include/systems/System.hpp
  include/systems/GraphicalSystem.hpp
  include/systems/TransformSystem.hpp
  include/systems/PhysicalSystem.hpp
  include/systems/ViewSystem.hpp
  
  include/entities/Entity.hpp
  include/entities/Broccoli.hpp
  include/entities/Garnet.hpp
  include/entities/GraphicalPlaceholderEntity.hpp
  include/entities/Background.hpp

  include/events/Event.hpp
  include/events/graphic_events.hpp
  include/events/transform_events.hpp
  include/events/view_events.hpp
  
  include/managers/SystemManager.hpp
  include/managers/ResourceManager.hpp 
  include/managers/ComponentManager.hpp
  include/managers/EntityManager.hpp
  include/managers/EventManager.hpp
  
  include/memory/ObjectPool.hpp
  include/memory/DataChunk.hpp
  
  include/constants/error.hpp
  include/constants/error.cpp
  include/constants/defaults.hpp
  
  include/support/result.hpp
  include/support/result.cpp
  include/support/IntrusiveList.hpp
  include/support/IntrusiveNode.hpp
  include/support/QuadTree.hpp
  include/support/MapLoader.hpp
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
)

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE} ${HEADERS} ${SOURCES})

# Link test executable against gtest & gtest_main & sfml
target_link_libraries(${UNIT_TESTS_NAME} gtest_main sfml-graphics sfml-audio sfml-system yaml-cpp)

add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next_ libraries.
#set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

endif() # BUILD_TESTS

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - DEPENDENCIES- - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(submodules)

# - - - - - - - - - - - - - - - - -SFML - - - - - - - - - - - - - - - - - - - -

# Chose a way to find the SFML.
if (MSVC) # WINDOWS

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "submodules/sfml-compiled/lib/cmake/SFML")

include_directories(submodules/sfml-compiled/include)
include_directories(submodules/yaml-cpp/include)

else() # LINUX

endif()

target_link_libraries(
  ${PROJECT_NAME} sfml-graphics sfml-audio sfml-system
)
target_link_libraries(
  ${PROJECT_NAME} yaml-cpp
)

find_package(SFML 2.5 COMPONENTS graphics audio system REQUIRED)
find_package(yaml-cpp)


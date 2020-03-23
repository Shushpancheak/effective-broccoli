# @PROJECT_NAME@
@PROJECT_DESCRIPTION@

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## How to use THE LIBRARY
Download the repository and place it into your project directory. In your project's CMakeLists.txt file, insert the following lines:
```cmake
...
add_subdirectory(@PROJECT_NAME@)
...
target_link_libraries(${PROJECT_NAME} @PROJECT_NAME@)
...
```

## How to use THE EXECUTABLE
```shell
cd build
./@PROJECT_NAME@
```

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.

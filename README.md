# Effective Brocolli

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## Build documentation
```shell
doxygen
```
Documentation shall be generated in the directory named `docs`.

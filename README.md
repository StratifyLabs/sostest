# Stratify OS (SOS) Test Suite

The Stratify OS Test Suite includes Stratify OS applications that test each aspect of the Stratify API using three types of tests:

- API: Tests to see that the functions perform as described in the documentation
- Performance: Executes a deterministic test that can be compared across platforms and between API versions to monitor improvements
- Stress: Executes random use cases over a large number of iterations in order to find bugs in Stratify OS or the Stratify API

## The Tests

### Stratify API Tests

The API tests are broken down by namespace (one application for each namespace). For example, `api-sys-test` tests the [sys namespace](https://stratifylabs.co/StratifyAPI/html/namespacesys.html).

### Additional Tests

#### Posix Test

This directly tests the POSIX implementation on Stratify OS.

#### Math Test

This tests the underlying math operations which allows a comparison between using libgcc or clang's compiler RT implementation of math functions.

## The Results

The output is in JSON format that can be easily parsed.

## Building and Running the Tests

All tests can be built for the Stratify OS embedded applications using the following commands (api-inet-test as an example):

```
cd api-inet-test
mkdir cmake_arm
cd cmake_arm
cmake ..
cmake --build . --target all -- -j 8
```

The test can be run using Stratify Link on a target board running Stratify OS.

Some tests support building for the Link target. The Link target allows you to run the tests on a desktop computer. The api-inet-test
supports the link target as well. From within the sostest repository, you can build using the following commands:

```
cd api-inet-test
mkdir cmake_link
cd cmake_link
cmake ..
cmake --build . --target all -- -j 8
```

The output can be run using the following command (in the cmake_link directory).

```
../build_release_link/api-inet-test
```

### Building on Windows

When building on Windows, use the PowerShell program rather than the command prompt. The `cmake ..` line above needs to be `cmake -G "MinGW Makefiles" ..`.

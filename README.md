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

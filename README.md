# mc2-serialization

This repo contains shared serialization functionality across the
`mc2` ecosystem.

## Overview

`mc2-serialization` contains the following:
* [`src/serialization.{h/cpp}`](src/): Helper functions for serializing and deserializing flatbuffer objects
* [`src/flatbuffers`](src/flatbuffers): Flatbuffer schemas

## Building

### Building for an enclave environment

To build `mc2-serialization` for an enclave, add the following to your `CMakeLists.txt` file:

```CMake
include(FetchContent)
FetchContent_Declare(
  mc2_serialization
  GIT_REPOSITORY https//github.com/mc2-project/mc2-serialization.git
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(mc2_serialization)
```

### Building for a host environment

To build `mc2-serialization` for a host, add the following to your `CMakeLists.txt`:
```CMake
include(FetchContent)
FetchContent_Declare(
  mc2_serialization
  GIT_REPOSITORY https://github.com/mc2-project/mc2-serialization.git
)
set(FETCHCONTENT_QUIET OFF)
set(HOST ON CACHE BOOL "")
FetchContent_MakeAvailable(mc2_serialization)
```

## Usage

After building, the `mc2_serialization` target can be linked to, and `flattbuffers`/`flatbuffers_cpp_generation` can be added as dependencies.

Headers can be imported directly (e.g. `#include "serialization.h"`).  If you need to include the flatbuffers headers, add the following line to your
`CMakeLists.txt` file:
```CMake
include_directories(${mc2_serialization_BINARY_DIR}/flatbuffers/include)
```
and import headers as `#include flatbuffers/schema.h`.

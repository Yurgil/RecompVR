# RecompVR

A small C library of shared OpenXR helpers for native standalone VR ports.
It is used by the TimeSplitters Quest port and local Battlefront and King Kong
projects. Game adapters retain ownership of their instances, sessions, action
sets, swapchains, rendering, coordinate conversions and frame-submission policy.

## Interfaces

- `recompvr/openxr.h`: stereo-session startup, reference spaces, frame timing,
  stereo-view location and action-state queries.
- `recompvr/android.h`: Android OpenXR loader initialization.
- `recompvr/vulkan.h`: physical-device selection using
  `XR_KHR_vulkan_enable2`.

The caller supplies the OpenXR headers and loader. Vulkan helpers also require
Vulkan headers; Android helpers require the NDK/JNI headers.

## CMake integration

```cmake
set(RECOMPVR_OPENXR_INCLUDE_DIR "/path/to/openxr/include")
set(RECOMPVR_WITH_VULKAN ON) # Optional; defaults to OFF.
add_subdirectory(external/RecompVR recompvr)
target_link_libraries(your_target PRIVATE RecompVR::openxr)
```

If an `OpenXR::headers` target already exists, it supplies the OpenXR headers
instead of `RECOMPVR_OPENXR_INCLUDE_DIR`. The caller must link its OpenXR loader.
Android helpers are included automatically when CMake targets Android.

## Host contract test

```sh
cmake -S . -B build \
  -DRECOMPVR_OPENXR_INCLUDE_DIR=/path/to/openxr/include \
  -DRECOMPVR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The test uses OpenXR stubs to check frame timing, stereo-view validity and
inactive/error action states. It does not validate a real headset or runtime.

## Updating consumers

Consumers can pin this repository as a Git submodule and update their pinned
commit independently. Validate each consumer after updating: passing this
library's contract test does not establish that a game's VR integration works.

## License

MIT. See [LICENSE](LICENSE). OpenXR, Vulkan and Android dependencies retain their
own licenses and are supplied by the consuming project or toolchain.

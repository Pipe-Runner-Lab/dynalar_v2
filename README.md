# Dynalar Rendering Engine

# Setup

## Linux

Make sure `cmake` and `GCC` are installed. We are using `GDB` debugger.

```bash
sudo apt update
sudo apt install cmake
sudo apt-get install build-essential gdb
```

Vcpkg is used to manage dependencies. [Install](https://vcpkg.io/en/getting-started) it and install the dependencies.

```bash
vcpkg install glfw3 glew opengl glm
```

Other than the packages vckpg suggests to install, the following would also be needed:

```bash
sudo apt install libglfw3-dev
```

## Apple

Same as above. You may need to install the following before running `vcpkg install`:

```bash
brew install pkg-config
```

# Important Links

1. https://code.visualstudio.com/docs/languages/cpp
2. https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration

# FAQ

- Q. CMakeLists.txt throws some errors.
  - A. Start by deleting the `build` folder and try again.
- Q. Vcpkg installed libs throwing error in Cmake, looking for config.
  - Make sure to set the correct triplet for the vcpkg in settings.json. For example, `-DVCPKG_TARGET_TRIPLET=x64-linux`

# TODO

- [ ] Cross platform scaling
  - [ ] DPI & Scaling
- [ ] Remove VCPKG extension and use directly in CMake
- [ ] Figure out how to add new cpp files correctly without needing to delete the build folder.
- [ ] Lift mouse settings on app level instead of per-scene level

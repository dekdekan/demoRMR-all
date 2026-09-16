# Building demoRMR with CMake

The project is built from these CMake targets:

- `librobot` - shared robot communication library
- `demoRMR` - Qt Widgets application
- `QJoysticks` - optional static joystick library

## Qt Creator

Open the top-level `CMakeLists.txt`, select a Qt 5 or Qt 6 desktop kit, and
configure the project. If OpenCV is not discovered automatically, add its
package directory to the initial CMake configuration, for example:

```text
-DOpenCV_DIR=C:/opencv_vc17
```

## Command line

Run the commands from a Visual Studio developer shell:

```powershell
cmake -S . -B build-cmake -G Ninja `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_PREFIX_PATH="C:/Qt6/6.9.3/msvc2022_64;C:/opencv_vc17"
cmake --build build-cmake --parallel
```

The executable and runtime project DLLs are placed in `build-cmake/bin`.
Qt and OpenCV runtime DLLs still need to be available on `PATH` or deployed
with the usual Qt/OpenCV deployment tools.

## Feature options

The feature defaults are:

| CMake option | Default | Meaning |
| --- | --- | --- |
| `DISABLE_OPENCV` | `OFF` | Disable OpenCV support |
| `DISABLE_SKELETON` | `OFF` | Disable skeleton support |
| `DISABLE_JOYSTICK` | `ON` | Disable QJoysticks/SDL support |
| `DISABLE_AMCL` | `OFF` | Disable AMCL support |

For example, enable joystick support with:

```powershell
cmake -S . -B build-cmake -DDISABLE_JOYSTICK=OFF
```

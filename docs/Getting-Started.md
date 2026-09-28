# Instalation

Currently the only way to install Widget Shell in by building it from source and running install script.

## Building

Before you start building, make sure you have the following programs and libraries installed:

* C++ compiler with C++23 support
* CMake 3.19 or newer
* Qt 6.5 or newer
* LayerShellQt
* libpulse

1. Clone this repository by running:

```bash
git clone 
```

2. Enter the repository's root directory and run:

```bash
mkdir build && cd build
cmake ..
make
```

3. To install the application, run:

```bash
make install
```

4. This application uses the Qt platform theme. Make sure the `QT_QPA_PLATFORMTHEME` environment variable is set.

# Supported Compositors

# Basic Configuration


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

Widget Shell currently supports the following Wayland compositors:

* Hyprland
* Sway

It should work in other compositors bub with limited features.

# Basic Configuration

Configuration is loaded from the `~/.config/widgetshell/` directory.

The `widgetshell.conf` file contains the general configuration. Configuration options are divided into groups. 
For example, to set the height of the bar, set `barHeight` in `[Bar]` group:

```
[Bar]
barHeight=40
```

To set a wallpaper, set the `wallpaper` option in the `[Wallpaper]` group to the path of the wallpaper 
file. Both images and videos are supported. Set it to an empty string to disable the wallpaper.

The `bar.json` configures layout and widgets for bar. It uses json format. Some widgets can be configured
from this file. For more information see [Configuration](Configuration.md).
Layout is devited into 3 sections: left, center and right. Example configuration can look like this:

```json
{
    "left": [
        {
            "component": "openedapps"
        },
        {
            "component": "activeappname"
        }
    ],
    "center": [
        {
            "component": "workspaceswitcher"
        }
    ],
    "right": [
        {
            "component": "networkconnection",
            "unit": "kb"
        },
        {
            "component": "audiocontrol"
        },
        {
            "component": "datetime",
            "format": "hh::mm:ss | dd:MM:yyyy"
        },
        {
            "component": "controlmenu"
        }
    ]
}
```

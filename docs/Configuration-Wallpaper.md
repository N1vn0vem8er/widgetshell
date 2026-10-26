# Wallpapaer

Wallpaper setting are in `Wallpaper` section in `widgetshell.conf` file. Path to wallpaper can be set with `wallpaper`
value, which can be either an image or a video. For example:

```
[Wallpaper]
wallpaper=/path/to/wallpaper.png
```

You can set aspect ratio mode with `AspectRatioMode`. Possible option are as follow:

* IgnoreAspectRatio
* KeepAspectRatio
* KeepAspectRatioByExpanding

You can set transformation mode with `TransformationMode`. Possible option are:

* FastTransformation
* SmoothTransformation
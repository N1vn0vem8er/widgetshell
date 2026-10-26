# Configuration

## Themes

Widget Shell uses Qt Style Sheets (QSS) for theming. QSS is similar to CSS, but it does not support all of its features.

User-created themes should be placed in the `~/.config/widgetshell/themes/` directory. 
A theme can be selected using the `theme` option in the `[General]` section of the `widgetshell.conf` file. 
For example:

```
[General]
theme=customtheme.qss
```

This will load the `customtheme.qss` theme file from the `themes` directory. 
To use the built-in default theme, set `theme=default`. To use the system Qt theme, set `theme=system`.

## Bar

Bar layout can be configured with `bar.json` file. This is the list of all avaliable widgets that can be placed
in the bar:

* [Workspace Switcher](Configuration-WorkspaceSwitcher.md)
* [Opened Applications](Configuration-OpenedApps.md)
* [Active Application Name](Configuration-ActiveAppName.md)
* [Audio Volume Control](Configuration-AudioVolumeControlBar.md)
* [Control Menu Button](Configuration-ControlMenuButton.md)
* [Cpu Usage](Configuration-CpuUsage.md)
* [Date Time](Configuration-DateTime.md)
* [Disk Usage](Configuration-DiskUsage.md)
* [Memory Usage](Configuration-MemoryUsage.md)
* [Network Connection](Configuration-NetworkConnection.md)
* [Uptime](Configuration-Uptime.md)

## Control Menu

Control menu layout can be configured with `controlmenu.json` file. This is the list of all avaliable widgets that can be placed in the control menu:

* [System Actions](Configuration-SystemActions.md)
* [Audio Control](Configuration-MenuAudioControl.md)
* [Calendar](Configuration-MenuCalendar.md)
* [Notification History](Configuration-NotificationHistory.md)

## Network Menu

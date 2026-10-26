# Disk Usage

The Disk Usage widget displays current disk usage. You can open menu for selecting disk by clicking on this widget.
You can select default disk with `disk` value.

The appearance of this widget can be customized using `DiskUsageBarComponent`. For example:

```css
DiskUsageBarComponent {
    background-color: #38427f;
}
```

In order to add Cpu Usage to the bar use:

```json
{
    "component": "diskusage"
}
```

You can format what is displayed:

```json
{
    "component": "diskusage",
    "format": "%1 ↑ %2 %4 | ↓ %3 %4",
    "unit" "gb",
    "disk": "sda"
}
```

Disk name will be placed in the place of `%1`. Read will be placed in the place of `%2` and read in the place of
`%3`. Unit will be placed in the place of `%4`.
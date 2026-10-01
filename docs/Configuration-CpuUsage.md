# Cpu Usage

The Cpu Usage widget displays current cpu usage.

The appearance of this widget can be customized using `CpuUsageComponent`. For example:

```css
CpuUsageComponent {
    background-color: #38427f;
}
```

In order to add Cpu Usage to the bar use:

```json
{
    "component": "cpuusage"
}
```

You can format what is displayed:

```json
{
    "component": "cpuusage",
    "format": "%1%"
}
```

Cpu usage will be placed in place of `%1`.
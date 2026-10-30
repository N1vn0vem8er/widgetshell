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
    "format": "%0% | %1% | %2%"
}
```

Each subsequent number corresponds to the next CPU core. %0 represents the overall CPU usage, 
%1 represents the usage of core 0, %2 represents the usage of core 1, %3 represents the usage of core 2, 
and so on.
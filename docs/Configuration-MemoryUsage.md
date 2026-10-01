# Memory Usage

The Memory Usage widget displays current memory usage.

The appearance of this widget can be customized using `MemoryUsageComponent`. For example:

```css
MemoryUsageComponent {
    background-color: #38427f;
}
```

In order to add Cpu Usage to the bar use:

```json
{
    "component": "memoryusage"
}
```

You can format what is displayed:

```json
{
    "component": "memoryusage",
    "format": "%1 %3 / %2 %3",
    "unit" "gb"
}
```

Used memory will be placed in place of `%1`, total memory in place of `%2` and unit in place of `%3`.
# DateTime

The DateTime widget displays current date and time.

The appearance of this widget can be customized using `DateTimeComponent`. For example:

```css
DateTimeComponent {
    background-color: #38427f;
}
```

In order to add DateTime to the bar use:

```json
{
    "component": "datetime"
}
```

You can format what is displayed:

```json
{
    "component": "datetime",
    "format": "hh::mm:ss | dd:MM:yyyy"
}
```
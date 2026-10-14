# Network Connection

The Network Connection widget displays current network connection.

The appearance of this widget can be customized using `NetworkConnectionComponent`. For example:

```css
NetworkConnectionComponent {
    background-color: #38427f;
}
```

In order to add Network Connection to the bar use:

```json
{
    "component": "networkconnection"
}
```

You can format what is displayed:

```json
{
    "component": "networkconnection",
    "unit": "kb"
}
```
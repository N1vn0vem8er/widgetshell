# Opened Apps

The Opened Apps widget displays the opened apps and allows you to switch between them. 
This widget is compositor-dependent and will not work with unsupported compositors.

The appearance of this widget can be customized using `OpenedAppsComponent`. For example:

```css
OpenedAppsComponent {
    background-color: #38427f;
}

OpenedAppsComponent QPushButton {
    background-color: #6474e1;
}
```

In order to add Workspace Switcher to the bar use:

```json
{
    "component": "openedapps"
}
```

# Workspace Switcher

The Workspace Switcher displays the active workspaces and allows you to switch between them. 
This widget is compositor-dependent and will not work with unsupported compositors.

The appearance of the buttons in this widget can be customized using `WorkspaceButton`. For example:

```css
WorkspaceButton {
    background-color: #38427f;
}

WorkspaceButton:hover {
    background-color: #6474e1;
}

WorkspaceButton:checked {
    background-color: #b0b9e1;
}
```

In order to add Workspace Switcher to the bar use:

```json
{
    "component": "workspaceswitcher"
}
```

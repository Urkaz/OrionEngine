# VS Code setup

VS Code only reads keybindings from the user profile, not from the workspace. Paste the snippet below into your user `keybindings.json` (`Ctrl+Shift+P` > "Preferences: Open Keyboard Shortcuts (JSON)").

Required extensions (see `extensions.json`): clangd and C++ Helper (`amiralizadeh9480.cpp-helper`).

```json
[
  {
    // Switch between header and source (Visual Studio style). Overrides the default "Open Folder".
    "key": "ctrl+k ctrl+o",
    "command": "clangd.switchheadersource",
    "when": "editorTextFocus && (editorLangId == 'cpp' || editorLangId == 'c')"
  },
  {
    // Generate the function body in the .cpp from the declaration under the cursor.
    // Overrides the default "Fix Next Problem".
    "key": "ctrl+k ctrl+.",
    "command": "cpp-helper.create-implementation",
    "when": "editorTextFocus && editorLangId == 'cpp'"
  }
]
```

Other C++ Helper commands: `cpp-helper.create-implementation-here`, `cpp-helper.copy-implementation`, `cpp-helper.create-header-guard`.

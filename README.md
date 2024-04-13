# CanSat Code

This repo contains all the code related to the CanSat itself, as well as documents related to circuit design.

# Development notes

The [Arduino Extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) is required for editing this project with VSCode. After installing the extension, a C++ IntelliSense config file has to be generated. To do so, press Ctrl+Alt+I (or Cmd+Alt+I on Mac), or execute `Arduino: Rebuild IntelliSense Configuration` from the command pallete. Even though `/lib` is added as an extra include directory in the config of the Arduino extension, the autogen fails to recognize it, so it must be added manually. Open `.vscode/c_cpp_properties.json` and add `"${workspaceFolder}\\**"` to the end of the `includePath` list.

## Data format

Radio messages are formatted as `[timestamp]:[pressure]:[temperature];`

# ds4-wifi-link

Script which gathers controller data then sends over wifi connection to an esp32.

This project uses [vcpkg](https://github.com/microsoft/vcpkg) for managing packages

### Notes

If building, you need to add this script to `~/.bashrc`
```
# If using building with VCPKG
export VCPKG_ROOT="$HOME/vcpkg"
export PATH="$VCPKG_ROOT:$PATH"

```


### WSL setup

To get controller to register on WSL

Run powershell as administrator
```bash
usbipd list # then find the controller
usbipd bind --busid <busid> # busid is labelled on the `list` section
usbipd attach --wsl --busid <busid>
```

Then script must be run using `sudo` permissions to actually get access to the controller.

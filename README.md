# ArduController

Use an [ArduBoy](https://arduboy.com/) as a keypad.

# Usage

Flash `ArduController.ini` to an ArduBoy. This was tested on the Kickstarted Edition, but it should work on all of them.

To flash a custom profile, use

```
python3 ApplyProfile.py /dev/ttyACM1 Profiles/Sterdeen.acprofile
```

replacing `/dev/ttyACM1` with your ArduBoy's serial port and `Profiles/Sterdeen.acprofile` wiht the path to the `.acprofile` you wish to use.

Switching profiles: Press `A+B` on the ArduBoy to open the profile menu. To navigate the menu, use the `UP` and `DOWN` keys, and select a profile with `A`

# Writing profiles

The format for a `.acprofile` is:

```
LeftAction,RightAction,UpAction,DownAction,AAction,BAction
```

`Action` can be either:
* A character (Eg. `a`)
* A modifier (Eg. `RETURN`)
* A keycode (Eg. `176`) (You can find the codes [here](https://www.arduino.cc/en/Reference/KeyboardModifiers))

If you make any profiles, please open a PR so it can be added to the repository, or an issue if you don't want to use a PR.

# Showing information

ArduController can display 3 lines of custom text on screen.

Read `InfoDisplay.py` to get an idea of how to do that.

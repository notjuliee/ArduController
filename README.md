# ArduController

Use an [ArduBoy](https://arduboy.com/) as a keypad.

# Usage

Flash `ArduController.ini` to an ArduBoy. This was tested on the Kickstarter Edition, but it should work on all of them.

To flash a custom profile, use

```
python3 ApplyProfile.py /dev/ttyACM1 Profiles/Sterdeen.acprofile 1
```

replacing `/dev/ttyACM1` with your ArduBoy's serial port and `Profiles/Sterdeen.acprofile` with the path to the `.acprofile` you wish to use.

The last argument is optional and specifies which slot you want the profile to be saved on. (Can be 1-5)

Switching profiles: Press `A+B` on the ArduBoy to open the profile menu. To navigate the menu, use the `UP` and `DOWN` keys, and select a profile with `A`

To reset all profiles, use

```
python3 ResetProfiles.py /dev/ttyACM1
```

replacing `/dev/ttyACM1` with your ArduBoy's serial port, then reboot the ArduBoy.

# Writing profiles

The format for a `.acprofile` is:

```
{
	"name": "Profile Name", // Up to 12 letters
	"defaultSlot": 1, // The slot this profile will save to if one isn's explicitly specified. (1-5)
	"profile": "LeftAction,RightAction,UpAction,DownAction,AAction,BAction" // A profile string
}
```

`Action` can be either:
* A character (Eg. `a`)
* A modifier (Eg. `RETURN`)
* A keycode (Eg. `176`) (You can find the codes [here](https://www.arduino.cc/en/Reference/KeyboardModifiers))

If you make any profiles, please open a PR so it can be added to the repository, or an issue if you don't want to use a PR.

# Showing information

ArduController can display 3 lines of custom text on screen.

Read `InfoDisplay.py` to get an idea of how to do that.

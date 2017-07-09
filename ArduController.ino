#include <Keyboard.h>
#include <Arduboy2.h>

bool allowMod = true;
int currentProfile = 0;
int profiles [3][6] = { { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, 'a', 'b' }, { 'a', 'd', 'w', 's', ' ', KEY_RETURN }, { ' ', ' ', ' ', ' ', ' ', ' ' } };
int keys [6] = { LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON };

char* pnames [3] = { "Profile: Default", "Profile: WASD", "Profile: Custom" };

Arduboy2 arduboy;

void setup() {
  Serial.begin(9600);
  arduboy.begin();
  Keyboard.begin();
  arduboy.setFrameRate(1);
  arduboy.clear();
  arduboy.print(pnames[0]);
  arduboy.display();
  Serial.println("Hello, world!");
}

void loop() {
  if (Serial.available() > 0) {
    int instr = Serial.parseInt();
    if (instr == 0) {
      arduboy.clear();
      arduboy.setCursor(0, 0);
      arduboy.print(pnames[currentProfile]);
      arduboy.setCursor(0, 10);
      arduboy.print(Serial.readStringUntil(','));
      arduboy.setCursor(0, 20);
      arduboy.print(Serial.readStringUntil(','));
      arduboy.setCursor(0, 30);
      arduboy.print(Serial.readStringUntil('\r'));
      arduboy.display();
    }
    if (instr == 1) {
      for (int i = 0; i < 6; i++) {
        profiles[2][i] = Serial.parseInt();
      }
      Keyboard.releaseAll();
    }
  }
  if (arduboy.pressed(B_BUTTON + A_BUTTON)) {
    if (allowMod) {
      currentProfile += 1;
      if (currentProfile > 2) currentProfile = 0;
      Keyboard.releaseAll();
      arduboy.clear();
      arduboy.setCursor(0, 0);
      arduboy.print(pnames[currentProfile]);
      arduboy.display();
    }
    allowMod = false;
  } else {
    allowMod = true;
    for (int i = 0; i < 6; i++) {
      if (arduboy.pressed(keys[i])) {
        Keyboard.press(profiles[currentProfile][i]);
      } else {
        Keyboard.release(profiles[currentProfile][i]);
      }
    }
  }
}

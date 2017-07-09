#include <Keyboard.h>
#include <Arduboy2.h>

int currentProfile = 0;
int profiles [3][6] = { { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, 'a', 'b' }, { ' ', ' ', ' ', ' ', ' ', ' ' }, { ' ', ' ', ' ', ' ', ' ', ' ' } };
int keys [6] = { LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON };

bool inMenu = false;
bool keyPressed [3] = { false, false, false };

char* pnames [3] = { "Default", "Custom1", "Custom2" };

Arduboy2 arduboy;

void drawProfile(char* pname) {
  arduboy.clear();
  arduboy.setCursor(0,0);
  arduboy.print(F("Profile: "));
  arduboy.setCursor(6*9,0);
  arduboy.print(pname);
  arduboy.display();
}

void doKeys() {
  for (int i = 0; i < 6; i++) {
    if (arduboy.pressed(keys[i])) {
      Keyboard.press(profiles[currentProfile][i]);
    } else {
      Keyboard.release(profiles[currentProfile][i]);
    }
  }
}

void drawMenu() {
  Keyboard.releaseAll();
  arduboy.clear();
  for (int i = 0; i < 3; i++) {
    arduboy.setCursor(0, i*10);
    if (currentProfile == i) arduboy.print(F("> "));
    arduboy.setCursor(12, i*10);
    arduboy.print(pnames[i]);
  }
  arduboy.display();
}

void drawInput() {
  if (!inMenu) {
    arduboy.clear();
    arduboy.setCursor(0, 0);
    drawProfile(pnames[currentProfile]);
    arduboy.setCursor(0, 10);
    arduboy.print(Serial.readStringUntil(','));
    arduboy.setCursor(0, 20);
    arduboy.print(Serial.readStringUntil(','));
    arduboy.setCursor(0, 30);
    arduboy.print(Serial.readStringUntil('\r'));
    arduboy.display();
  } else {
    Serial.readStringUntil(',');
    Serial.readStringUntil(',');
    Serial.readStringUntil('\r');
  }
}

void setup() {
  Serial.begin(9600);
  arduboy.begin();
  Keyboard.begin();
  arduboy.clear();
  drawProfile(pnames[0]);
  arduboy.display();
}

void loop() {
  if (Serial.available() > 0) {
    int instr = Serial.parseInt();
    if (instr == 0) {
      drawInput();
    }
    if (instr == 1) {
      int slot = Serial.parseInt();
      for (int i = 0; i < 6; i++) {
        profiles[slot][i] = Serial.parseInt();
      }
      Keyboard.releaseAll();
    }
  }
  if (arduboy.pressed(B_BUTTON + A_BUTTON) && !inMenu) {
    inMenu = true;
    keyPressed[2] = true;
    drawMenu();
  }
  if (!inMenu) {
    doKeys();
  } else {
    if (arduboy.pressed(DOWN_BUTTON) && currentProfile < 2) {
      if (!keyPressed[0]) {
        currentProfile++;
        drawMenu();
      }
      keyPressed[0] = true;
    } else {
      keyPressed[0] = false;
    }
    if (arduboy.pressed(UP_BUTTON) && currentProfile > 0) {
      if (!keyPressed[1]) {
        currentProfile--;
        drawMenu();
      }
      keyPressed[1] = true;
    } else {
      keyPressed[1] = false;
    }
    if (arduboy.pressed(A_BUTTON)) {
      if (!keyPressed[2]) {
        drawProfile(pnames[currentProfile]);
        inMenu = false;
      }
    } else {
      keyPressed[2] = false;
    }
  }
}

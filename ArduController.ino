#include <Keyboard.h>
#include <Arduboy2.h>
#include <EEPROM.h>
#include <Joystick.h>

int currentProfile = 0;

int defaultProfileTypes [5][6] = { { 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } };
int profileTypes [5][6];
  
int defaultProfiles [5][6] = { { 2, 3, 4, 5, 0, 1 }, { 'a', 'd', 'w', 's', ' ', KEY_RETURN },
  { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, ' ', KEY_RETURN },
  { '1', '2', '3', '4', '5', '6' }, { 'l', 'r', 'u', 'd', 'a', 'b' }};  
int profiles [5][6];

int keys [6] = { LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON };
bool pressedKeys [6] = { false, false, false, false, false, false };

bool inMenu = false;
bool keyPressed [3] = { false, false, false };

char* defaultPnames [5] = { "Default", "WASD", "Platformer", "Numbers", "Letters" };
char* pnames [5];

Arduboy2 arduboy;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 4, 0, true, true, false, false, false, false, false, false, false, false, false);

void releaseAll() {
  Keyboard.releaseAll();
  digitalWrite(LED_BUILTIN_TX, HIGH);
  for (int i = 0; i < 6; i++) {
    pressedKeys [i] = false;
  }
}

void customBegin() {
  arduboy.boot();
  arduboy.blank();
  arduboy.flashlight();
  arduboy.systemButtons();
  do {
    delay(50);
  } while (arduboy.buttonsState());
}

void initEEPRom() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.write(0, 'h');
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      EEPROM.write(x * 6 + y + 1, defaultProfiles[x][y]);
    }
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      EEPROM.write(x * 6 + y + 48, defaultProfileTypes[x][y]);
    }
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 32; y++) {
      EEPROM.write(32 * x + y + 128, defaultPnames[x][y]);
    }
  }
}

void loadFromEEPRom() {
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      profiles[x][y] = EEPROM.read(x * 6 + y + 1);
    }
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      profileTypes[x][y] = EEPROM.read(x * 6 + y + 48);
    }
  }
  for (int x = 0; x < 5; x++) {
    pnames[x] = new char[32];
    for (int y = 0; y < 32; y++) {
      pnames[x][y] = EEPROM.read(32 * x + y + 128);
    }
  }
}

void writeToEEPRom() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.write(0, 'h');
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      EEPROM.write(x * 6 + y + 1, profiles[x][y]);
    }
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 6; y++) {
      EEPROM.write(x * 6 + y + 48, profileTypes[x][y]);
    }
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 32; y++) {
      EEPROM.write(32 * x + y + 128, pnames[x][y]);
    }
  }
}

void drawProfile(char* pname) {
  arduboy.clear();
  arduboy.setCursor(0, 0);
  arduboy.print(F("Profile: "));
  arduboy.setCursor(6 * 9, 0);
  arduboy.print(pname);
}

void doKeys() {
  for (int i = 0; i < 6; i++) {
    if (arduboy.pressed(keys[i])) {
      
      if (!pressedKeys[i]) {
        Keyboard.press(profiles[currentProfile][i]);
        digitalWrite(LED_BUILTIN_TX, HIGH);
        pressedKeys[i] = true;
      }
    } else if (pressedKeys[i]) {
      Keyboard.release(profiles[currentProfile][i]);
      digitalWrite(LED_BUILTIN_TX, HIGH);
      pressedKeys[i] = false;
    }
  }
}

void drawMenu() {
  arduboy.clear();
  for (int i = 0; i < 5; i++) {
    arduboy.setCursor(0, i * 10);
    if (currentProfile == i) arduboy.print(F("> "));
    arduboy.setCursor(12, i * 10);
    arduboy.print(pnames[i]);
  }
  arduboy.display();
}

void drawInput() {
  if (!inMenu) {
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
  customBegin();
  Keyboard.begin();
  arduboy.clear();
  drawProfile(pnames[0]);
  arduboy.display();
  if (EEPROM.read(0) != 'h') {
    initEEPRom();
  }
  loadFromEEPRom();
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
      int nameLen = Serial.parseInt();
      char* newNameChar = new char;
      Serial.readBytesUntil('\r', newNameChar, 32);
      newNameChar[nameLen + 1] = 0;
      pnames[slot] = newNameChar + 1;
      releaseAll();
      writeToEEPRom();
    }
    if (instr == 2) {
      EEPROM.write(0, 0);
    }
  }
  if (arduboy.pressed(B_BUTTON + A_BUTTON) && !inMenu) {
    inMenu = true;
    keyPressed[2] = true;
    drawMenu();
    releaseAll();
  }
  if (!inMenu) {
    doKeys();
  } else {
    if (arduboy.pressed(DOWN_BUTTON) && currentProfile < 4) {
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
        arduboy.display();
        inMenu = false;
      }
    } else {
      keyPressed[2] = false;
    }
  }
}

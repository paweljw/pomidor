#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd;

// === PINS
const int BTN_PIN = 3;
const int LED_PIN = 9;
const int SPK_PIN = 10;

// === LCD
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// === OTHER CONSTANTS
const int MINUTE = 60;
const int POMODORO = 25 * MINUTE;
const int SHORT_BREAK = 5 * MINUTE;
const int LONG_BREAK = 15 * MINUTE;
const int POMODOROS_TO_LONG_BREAK = 4;

// === STATES
const int STATE_INI = 0;
const int STATE_RUN = 2;
const int STATE_WAI = 4;
const int STATE_PSE = 8;
const int STATE_BRK = 16;

// === GLOBAL VARIABLES
int countingDownFrom = 0;
int seconds = 0;
int donePomodoros = 0;
int ledBrightness = 255;
bool ledRising = false;
int state = STATE_INI;
bool breakNext = false;

void setLed(int val) {
  analogWrite(LED_PIN, val);
}

void brap(int length) {
  tone(SPK_PIN, 440, length);
}

void processLedTick() {
  if(ledRising) {
    ledBrightness += 5;
  } else {
    ledBrightness -= 5;
  }

  setLed(ledBrightness);

  if(ledBrightness == 0) ledRising = true;
  if(ledBrightness == 255) ledRising = false;
}

bool isButtonPressed() {
  return digitalRead(BTN_PIN) == LOW;
}

void printSeconds() {
  lcd.setCursor(0, 1);
  if(seconds < 600) {
    lcd.print("0");
  }
  lcd.print(seconds / 60);
  lcd.print(":");
  if(seconds % 60 < 10) {
    lcd.print("0");
  }
  lcd.print(seconds % 60);
}

void runPomodoro() {
  countingDownFrom = POMODORO;
  seconds = POMODORO;
  state = STATE_RUN;

  lcd.clear();
  lcd.backlight();
  lcd.print("Pomodoro ");
  lcd.print(donePomodoros + 1);
  lcd.print("/");
  lcd.print(POMODOROS_TO_LONG_BREAK);
  printSeconds();
}

void pausePomodoro() {
  state = STATE_PSE;
  lcd.backlight();
  lcd.clear();
  lcd.print("Paused...");
  printSeconds();
}

void resumePomodoro() {
  state = STATE_RUN;
  countingDownFrom = seconds;
  lcd.backlight();
  lcd.clear();
  lcd.print("Pomodoro ");
  lcd.print(donePomodoros + 1);
  lcd.print("/");
  lcd.print(POMODOROS_TO_LONG_BREAK);
  printSeconds();
}

void finishPomodoro() {
  brap(1000);
  lcd.backlight();
  state = STATE_WAI;
  donePomodoros += 1;
  lcd.clear();
  lcd.print("Done! ");
  lcd.print(donePomodoros);
  lcd.print("/");
  lcd.print(POMODOROS_TO_LONG_BREAK);
  breakNext = true;
}

void startBreak(int length) {
  breakNext = false;
  lcd.backlight();
  countingDownFrom = length;
  seconds = length;
  lcd.clear();
  lcd.print("Break time!");
  printSeconds();
  state = STATE_BRK;
}

void finishBreak() {
  state = STATE_WAI;
  lcd.backlight();
  brap(1000);
  lcd.clear();
  lcd.print("Focus, darn ya!");
  if(donePomodoros == POMODOROS_TO_LONG_BREAK) {
    donePomodoros = 0;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  int status;
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) hd44780::fatalError(status);


  setLed(ledBrightness);
  lcd.print("Press to start");
}

// the loop routine runs over and over again forever:
void loop() {
  switch(state) {
    case STATE_INI:
      if(isButtonPressed()) {
        runPomodoro();
        brap(100);
        delay(250);
        break;
      }
      break;
    case STATE_RUN:
      printSeconds();
      for(int i = 0; i < 10; i++) {
        processLedTick();
        if(isButtonPressed()) {
          pausePomodoro();
          brap(100);
          delay(250);
          break;
        }
        delay(100);
      }
      seconds -= 1;
      if(seconds == countingDownFrom - 5) lcd.noBacklight();
      if(seconds == 0) {
        finishPomodoro();
      }
      break;
    case STATE_BRK:
      printSeconds();
      for(int i = 0; i < 10; i++) {
        processLedTick();
        delay(100);
      }
      seconds -= 1;
      if(seconds == countingDownFrom - 5) lcd.noBacklight();
      if(seconds == 0) {
        finishBreak();
        break;
      }
      break;
    case STATE_WAI:
      setLed(255);

      if(isButtonPressed()) {
        brap(100);
        if(breakNext) {
          if(donePomodoros == POMODOROS_TO_LONG_BREAK) {
            startBreak(LONG_BREAK);
          } else {
            startBreak(SHORT_BREAK);
          }
        } else {
          runPomodoro();
        }
        delay(250);
        break;
      }
      break;
    case STATE_PSE:
      setLed(255);
      if(isButtonPressed()) {
        brap(100);
        resumePomodoro();
        delay(250);
        break;
      }
      break;
  }
}

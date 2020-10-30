#include "config.h"
#include "locale.h"
#include "font.h"
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd;

// === GLOBAL VARIABLES
int POMODORO;
int SHORT_BREAK;
int LONG_BREAK;

int countingDownFrom = 0;
int seconds = 0;
int donePomodoros = 0;
int ledBrightness = 255;
int state = STATE_INI;

bool ledRising = false;
bool breakNext = false;

void setLed(int val) {
  analogWrite(LED_PIN, val);
}

void playNote(int length) {
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
  lcd.print(S_SEC_PRE);
  lcd.write(0);
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

void printPomodoro() {
  lcd.backlight();
  lcd.clear();
  lcd.print(S_POMODORO);
  lcd.print(donePomodoros + 1);
  lcd.print(S_SEP);
  lcd.print(POMODOROS_TO_LONG_BREAK);
  printSeconds();
}

void runPomodoro() {
  countingDownFrom = POMODORO;
  seconds = POMODORO;
  state = STATE_RUN;
  printPomodoro();
}

void pausePomodoro() {
  state = STATE_PSE;
  lcd.backlight();
  lcd.clear();
  lcd.print(S_PAUSED);
  printSeconds();
}

void resumePomodoro() {
  state = STATE_RUN;
  countingDownFrom = seconds;
  printPomodoro();
}

void finishPomodoro() {
  playNote(1000);
  lcd.backlight();
  state = STATE_WAI;
  donePomodoros += 1;
  lcd.clear();
  lcd.print(S_DONE);
  lcd.print(donePomodoros);
  lcd.print(S_SEP);
  lcd.print(POMODOROS_TO_LONG_BREAK);
  breakNext = true;
}

void startBreak(int length) {
  breakNext = false;
  lcd.backlight();
  countingDownFrom = length;
  seconds = length;
  lcd.clear();
  lcd.print(S_BREAK_TIME);
  printSeconds();
  state = STATE_BRK;
}

void finishBreak() {
  state = STATE_WAI;
  lcd.backlight();
  playNote(1000);
  lcd.clear();
  lcd.print(S_FOCUS_TIME);
  if(donePomodoros == POMODOROS_TO_LONG_BREAK) {
    donePomodoros = 0;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  pinMode(PML_PIN, INPUT);
  pinMode(SBL_PIN, INPUT);
  pinMode(LBL_PIN, INPUT);

  int status;
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) hd44780::fatalError(status);

  lcd.print(S_INITIALIZING);
  lcd.createChar(0, TIME_CHAR);
  delay(250); // wait for lines to settle

  if(digitalRead(PML_PIN) == LOW) {
    POMODORO = LOW_POMODORO;
  } else {
    POMODORO = HIG_POMODORO;
  }

  if(digitalRead(SBL_PIN) == LOW) {
    SHORT_BREAK = LOW_SHORT_BREAK;
  } else {
    SHORT_BREAK = HIG_SHORT_BREAK;
  }

  if(digitalRead(LBL_PIN) == LOW) {
    LONG_BREAK = LOW_LONG_BREAK;
  } else {
    LONG_BREAK = HIG_LONG_BREAK;
  }

  setLed(ledBrightness);

  lcd.clear();
  lcd.print(S_BANNER);
  lcd.setCursor(0, 1);
  lcd.print(S_PRESS_TO_START);
}

// the loop routine runs over and over again forever:
void loop() {
  switch(state) {
    case STATE_INI:
      if(isButtonPressed()) {
        runPomodoro();
        playNote(100);
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
          playNote(100);
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
        playNote(100);
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
        playNote(100);
        resumePomodoro();
        delay(250);
        break;
      }
      break;
  }
}

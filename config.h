#ifndef __CONFIG_H_
#define __CONFIG_H_

// === PINS (ARDUINO NOTATION)
#define BTN_PIN 3
#define LED_PIN 9
#define SPK_PIN 10

#define PML_PIN 13
#define SBL_PIN 12
#define LBL_PIN 11

// === LCD
#define LCD_COLS 16
#define LCD_ROWS 2

// === OTHER CONSTANTS
#define MINUTE 60

#define POMODOROS_TO_LONG_BREAK 4

// === POMODORO OPTS - READ FROM DIP
#define LOW_POMODORO 25 * MINUTE
#define HIG_POMODORO  50 * MINUTE

#define LOW_SHORT_BREAK 5 * MINUTE
#define HIG_SHORT_BREAK 10 * MINUTE

#define LOW_LONG_BREAK 15 * MINUTE
#define HIG_LONG_BREAK 30 * MINUTE

// === STATES
#define STATE_INI 0
#define STATE_RUN 2
#define STATE_WAI 4
#define STATE_PSE 8
#define STATE_BRK 16

#endif // __CONFIG_H_

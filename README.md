# pomidor v2.0

Software for a hardware Pomodoro timer, built on ATmega328p and assorted cruft I amassed over the years.

The joke is "pomidor" means "tomato" in Polish.

## Features

* Variable pomodoro length (25/50min)
* Variable short break length (5/10min)
* Variable long break length (15/30min)
* Audible cue on pomodoro end/break end
* Visual cue on pomodoro end (LCD backlight sleeps after 5s of inactivity, wakes up for break)
* Customization as easy as modifying `config.h`

## Usage

Assuming you have your avrispmkII attached to your ATmega328p breakout/dev board that you handwired yourself in _exactly_ the same way I did:

```
git submodule init
git submodule sync
make ispload
```

## Board schematic

![Board schematic](img/schematic.png)

### Board features

* USB-B power plug
* Power stabilization (5v)
* Physical sound and power switches - no software trickery, as it should be
* DIP switches for end-user configuration (well, not _really_ end user, but it's better than having to reprogram the chip for it every time)

## Possible board outline

![Board outline](img/board.png)

The schematic is verified now and I have manufactured a board from this circuit which audited correctly.

Idea for this outline is to expose the USB plug and switches off the side, where they can be incorporated into a case neatly.

An example Bill of Materials is available in [bom.csv](bom.csv).

## Credits

* hd44780 library from https://github.com/duinoWitchery/hd44780

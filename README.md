# pomidor

Software for a hardware Pomodoro timer, built on ATmega328p and assorted cruft I amassed over the years.

The joke is "pomidor" means "tomato" in Polish.

## Usage

Assuming you have your avrispmkII attached to your ATmega328p breakout/dev board that you handwired yourself in _exactly_ the same way I did:

```
git submodule init
git submodule sync
make ispload
```

## Credits

* hd44780 library from https://github.com/duinoWitchery/hd44780

# BlinkBuzz

Use this to control pinouts on an Arduino to easily make repetitive patterns.

Supports async patterns to prevent blocking `delay()` calls, if desired.

## Usage

```c++
#include <BlinkBuzz.h>

#define BUZZER 33;

int allowedPins[] = { LED_BUILTIN, BUZZER };
BlinkBuzz bb(allowedPins, 2, true);// allowed pins, number of pins, allow usage of async patterns

void loop() {
	bb.update();
}

void someOtherFunction() {
	//Async:
	bb.aonoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
	//OR
	bb.aonoff(BUZZER, 200, 5); // beep 5x, 200ms on, 200ms off
	//OR
	bb.aonoff(BUZZER, 200); // beep 1x, 200ms on

	//For non-async patterns, use the following:
	bb.onoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
	//OR
	bb.onoff(BUZZER, 200, 5); // beep 5x, 200ms on, 200ms off
	//OR
	bb.onoff(BUZZER, 200); // beep 1x, 200ms on

	//Can also simply hold the pin on/off:
	bb.on(BUZZER); // turn on
	bb.off(BUZZER); // turn off

	//Finally, can clear a pin's queue (in async mode only):
	bb.clearQueue(BUZZER);
	//or can tell it to clear at set itself to a specific state:
	bb.clearQueue(BUZZER, HIGH); // clear and set to HIGH
}
```

The code should be set up to run on a Windows machine or Arduino, although it has not actually been tested on an Arduino.

For use on Windows for testing, you can compile the example code with:

```powershell
g++ examples/main.cpp src/BlinkBuzz.cpp -o BlinkBuzz
```
Assuming you have the MinGW compiler installed.
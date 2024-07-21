# BlinkBuzz

Use this to control pinouts on an Arduino to easily make repetitive patterns.

Supports async patterns to prevent blocking `delay()` calls, if desired.

## Usage

```c++
#include <BlinkBuzz.h>

const int BUZZER  = 33; // defined in the BlinkBuzz.h file for easy access across files
const int LED = LED_BUILTIN;  // defined in the BlinkBuzz.h file for easy access across files

int allowedPins[] = { LED, BUZZER };
BlinkBuzz bb(allowedPins, 2, true);// allowed pins, number of pins, allow usage of async patterns

void loop() {
	bb.update();
}

void someOtherFunction() {

	//Simply hold the pin on/off:
	bb.on(BUZZER); // turn on
	bb.off(BUZZER); // turn off

	//For non-async patterns, use the following:
	bb.onoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
	//OR
	bb.onoff(BUZZER, 200, 5); // beep 5x, 200ms on, 200ms off
	//OR
	bb.onoff(BUZZER, 200); // beep 1x, 200ms on

	//Everything below here is for async patterns only:

	bb.aonoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
	//OR
	bb.aonoff(BUZZER, 200, 5); // beep 5x, 200ms on, 200ms off
	//OR
	bb.aonoff(BUZZER, 200); // beep 1x, 200ms on

	//Can clear a pin's queue:
	bb.clearQueue(BUZZER);
	//or can tell it to clear at set itself to a specific state:
	bb.clearQueue(BUZZER, HIGH); // clear and set to HIGH

	//If you make another "aonoff" call before the current queue for that pin has finished, you can choose to append the new action to the end of the queue (default) or overwrite the current queue with the new pattern:
	bb.aonoff(BUZZER, 200, 3, 100); // append to queue
	bb.aonoff(BUZZER, 200, 3, 100, false); // overwrite queue

	//Finally, by specifying "0" for the number of times to repeat, the pattern will repeat indefinitely until a new pattern is set or the queue is cleared (also appendable or overwritable):
	bb.aonoff(BUZZER, 200, 0); // beep indefinitely 200ms on, 200ms off
}
```

The code should be set up to run on a Windows machine or Arduino, although it has not actually been tested on an Arduino.

For use on Windows for testing, you can compile the example code with:

```powershell
g++ examples/main.cpp src/BlinkBuzz.cpp -o BlinkBuzz
```
Assuming you have the MinGW compiler installed.

## Changelog

### 1.2.0

- Added support for overwriting current queue with new pattern or appending new pattern to end of queue (async mode only)
- Added support for indefinite patterns (i.e. no end to the pattern, async mode only) These can be appended to the queue, but a new pattern will overwrite the indefinite pattern immediately

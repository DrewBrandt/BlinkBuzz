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
}
```
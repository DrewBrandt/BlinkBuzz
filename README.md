# BlinkBuzz

Use this to control pinouts on an Arduino to easily make repetitive patterns.

Supports async patterns to prevent blocking `delay()` calls, if desired.

## Usage

```c++
#include <BlinkBuzz.h>

const int BUZZER  = 33;       // declared in the BlinkBuzz.h file for easy access across files (pin #)
const int LED = LED_BUILTIN;  // declared in the BlinkBuzz.h file for easy access across files
const int BUZZER  = 33;       // declared in the BlinkBuzz.h file for easy access across files (pin #)
const int LED = LED_BUILTIN;  // declared in the BlinkBuzz.h file for easy access across files

int allowedPins[] = { LED, BUZZER };
BlinkBuzz bb(allowedPins, 2, true, 50); // allowed pins, number of pins, allow usage of async patterns, Max queue size per pin (async mode only)
//NOTE: Max queue size refers to the number of on and off toggles that are performed, so ON -> OFF is 2 spots in the queue.

void loop() {    // Must be called in the loop to update async pin states. Not required for non-async usage.
    bb.update(); // The more frequently this is called, the more accurate the timing will be.

    // I recommend not using a delay() call for loop iterations when using async patterns.
    // Instead, maybe try something like this:
    double time = millis();
    if(time - lastTime > DESIRED_INTERVAL) { // however much you would have otherwise delay()ed for
        lastTime = time; // lastTime = global variable declared elsewhere
        // the rest of your loop code here
    }
}

void someOtherFunction() {

    // Simply hold the pin on/off:
    bb.on(BUZZER);  // turn on
    bb.off(BUZZER); // turn off

    // For non-async patterns, use the following:
    bb.onoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
    // OR
    bb.onoff(BUZZER, 200, 5);      // beep 5x, 200ms on, 200ms off
    // OR
    bb.onoff(BUZZER, 200);         // beep 1x, 200ms on

    // Everything below here is for async patterns only:

    bb.aonoff(BUZZER, 200, 3, 100); // beep 3 times, 200ms on, 100ms off
    // OR
    bb.aonoff(BUZZER, 200, 5);      // beep 5x, 200ms on, 200ms off
    // OR
    bb.aonoff(BUZZER, 200);         // beep 1x, 200ms on

    // Can clear a pin's queue:
    bb.clearQueue(BUZZER);
    // or can tell it to clear at set itself to a specific state:
    bb.clearQueue(BUZZER, HIGH); // clear and set to HIGH

    // There is also a pattern builder for more complex patterns:
    BBPattern pattern = BBPattern(ON_DURATION, REPEATS, OFF_DURATION);
    // For example, to build an SOS pattern, build the S and O letters:
    BBPattern s(50, 3, 200);
	BBPattern o(500, 3, 200);
    // Then, append them to each other to build a single SOS pattern:
    BBPattern sos = s.a(o).a(s);
    // Then call it
    bb.aonoff(BUZZER, sos);

    // To repeat a pattern indefinitely, use the following:
    bb.aonoff(BUZZER, sos, true);

    // You can also append a "rest" to the end of a pattern, changing it's final duration.
    bb.aonoff(BUZZER, sos.r(1000), true); // Rest for 1 second between the SOS patterns

    // Appending and adding rests can be done in any order as many times as you wish (so long as you have the queue space to hold them)
}
```

The code should be set up to run on a Windows machine or Arduino, although it has not actually been tested on an Arduino.

For use on Windows for testing, you can compile the example code with:

```powershell
g++ examples/main.cpp src/BlinkBuzz.cpp -o BlinkBuzz
```
Assuming you have the MinGW compiler installed.

## TODO

- Document internal workings

## Changelog

### 2.0.0

- Added the BBPattern class to allow for more complex patterns to be built and displayed
- Added the ability to repeat patterns indefinitely
- Added the ability to append a rest to the end of a pattern
- Removed the ability to append a pattern to the end of the queue, as it was redundant with the new BBPattern class. Now, calling `aonoff` with a new pattern will overwrite the current queue with the new pattern.


### 1.2.0

- Added support for overwriting current queue with new pattern or appending new pattern to end of queue (async mode only)
- Added support for indefinite patterns (i.e. no end to the pattern, async mode only) These can be appended to the queue, but a new pattern will overwrite the indefinite pattern immediately

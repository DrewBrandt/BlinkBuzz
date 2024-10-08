#ifndef BLINKBUZZ_H
#define BLINKBUZZ_H

#ifndef ARDUINO
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <Windows.h>

const uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
enum val
{
	LOW,
	HIGH
};
inline uint64_t millis() { return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start); }
static double last = 0;
inline void digitalWrite(int pin, val value)
{
	int color;
	switch (pin)
	{
	case 13:
		color = 36;
		break;
	case 33:
		color = 33;
		break;
	case 32:
		color = 95;
		break;
	default:
		color = 0;
		break;
	}
	double time = millis();
	printf("[%d] \x1B[%dm%.3f - %d to \x1B[%dm%s\x1B[0m\n", int(time - last), color, time / 1000.0, pin, value == LOW ? 91 : 92, value == LOW ? "LOW" : "HIGH");
	last = time;
}
inline void delay(uint64_t ms)
{
	Sleep(ms);
}

#else

#include <Arduino.h>

#endif // !ARDUINO

#include "BBPattern.h"

class BlinkBuzz
{
public:
	BlinkBuzz(int *allowedPins, int numPins, bool enableAsync = true, int queueChanges = 50);
	// memory usage (heap):
	//
	// numPins * 4
	// +
	// numPins * maxQueueSize
	// ------------------
	// (4 * 2) + (2 * 50) + (10) = 118 bytes for 2 pins with a queue size of 50

	~BlinkBuzz();

	void update(int curMS = -1);

	void on(int pin);  // turn on a pin
	void off(int pin); // turn off a pin

	void onoff(int pin, int duration);						 // turn on a pin for a ms duration
	void onoff(int pin, int duration, int times);			 // turn on a pin for a ms duration, repeat times times with ms duration pause
	void onoff(int pin, int duration, int times, int pause); // turn on a pin for a ms duration, repeat times times with ms pause pause
	void onoff(int pin, BBPattern &pattern);				 // turn on a pin for a pattern duration, repeat indefinitely or not

	// turn on a pin for a ms duration asynchronously
	void aonoff(int pin, int duration);
	// turn on a pin for a ms duration, repeat `times` times with ms `duration` pause asynchronously, 0 to repeat indefinitely
	void aonoff(int pin, int duration, int times);
	// turn on a pin for a ms duration, repeat `times` times with ms `pause` pause asynchronously, 0 to repeat indefinitely
	void aonoff(int pin, int duration, int times, int pause);
	// turn on a pin for a pattern duration, repeat indefinitely or not asynchronously
	void aonoff(int pin, const BBPattern &pattern, bool indef = false);

	void clearQueue(int pin);
	void clearQueue(int pin, int resetTo);

private:
	int maxQueueSize; // max number of actions that can be queued (turning on is a separate action from turning off)
	int numPins;
	int *allowedPins; // list of pins that are allowed to be used (prevents misfiring pins on accident thru typos)

	// each pin has its own queue of actions
	bool *pinState;	  // each pin's current on/off state - len: numPins
	bool *pinIsIndef; // each pin's q restart spot for indefinite patterns (-1 for no indef state) - len: numPins
	int *pinQStart;	  // each pin's queue start index (i.e. the next timestamp to be executed) - len: numPins
	int *pinQEnd;	  // each pin's queue end index (i.e. the last timestamp to be executed) - len: numPins

	int **pinQ; // all pins' queue of timestamps - size: numPins x maxQueueSize

	//[p1] [time][time][time]...[time]
	//[p2] [time][time][time]...[time]
	//[p3] [time][time][time]...[time]
	// ...
	//[pn] [time][time][time]...[time]

	bool isAllowed(int pin);				   // check if a pin is allowed to be used
	int getPinIndex(int pin);				   // get the index of a pin in the allowedPins array
	void aonoffhelper(int pin, int timeStamp); // helper function to add a timestamp to a pin's queue
	int calcTimeStamp(int pin, int pinIndex);  // calculate the timestamp for a new action based on the current queue state
	void toggle(int pin);					   // toggle a pin's state
	void indefHelper(int pin, double curMS);

	bool enableAsync; // prevents initializing extra memory if you don't need it

	int queueSpacing = 100; // ms between each new action when you don't specify a # of repeats AND append a new action before the current one completes.
};

extern BlinkBuzz bb;
extern int BUZZER;
extern int LED;
#endif // !BLINKBUZZ_H

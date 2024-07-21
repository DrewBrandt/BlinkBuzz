
#include "BlinkBuzz.h"

BlinkBuzz::BlinkBuzz(int *allowedPins, const int numPins, bool enableAsync)
{
	this->enableAsync = enableAsync;
	this->numPins = numPins;
	this->allowedPins = allowedPins;
	pinState = new bool[numPins];
	if (enableAsync)
	{
		pinQStart = new int[numPins];
		pinQEnd = new int[numPins];
		pinQIndef = new int[numPins];
		pinQ = new int *[numPins];
		for (int i = 0; i < numPins; i++)
		{
			pinQStart[i] = 0;
			pinQEnd[i] = 0;
			pinQIndef[i] = -1; // -1 means no indefinite pattern
			pinQ[i] = new int[MAX_QUEUE];
		}
	}
	else
	{
		pinQ = nullptr;
		pinQStart = nullptr;
		pinQEnd = nullptr;
		pinQIndef = nullptr;
	}
	for (int i = 0; i < numPins; i++)
	{
		pinState[i] = false;
#ifdef ARDUINO
		pinMode(allowedPins[i], OUTPUT);
#endif // ARDUINO
	}
}
BlinkBuzz::~BlinkBuzz()
{
	delete[] pinState;
	if (enableAsync)
	{
		delete[] pinQStart;
		delete[] pinQEnd;
		delete[] pinQIndef;
		for (int i = 0; i < numPins; i++)
		{
			delete[] pinQ[i];
		}
		delete[] pinQ;
	}
}

#pragma region Synchronous and Helper functions

bool BlinkBuzz::isAllowed(int pin)
{
	if (pin < 0)
		return false;

	for (int i = 0; i < numPins; i++)
		if (allowedPins[i] == pin)
			return true;
	return false;
}
int BlinkBuzz::getPinIndex(int pin)
{
	for (int i = 0; i < numPins; i++)
		if (allowedPins[i] == pin)
			return i;
	return -1;
}
void BlinkBuzz::on(int pin)
{
	if (isAllowed(pin))
	{
		pinState[getPinIndex(pin)] = true;
		digitalWrite(pin, HIGH);
	}
}
void BlinkBuzz::off(int pin)
{
	if (isAllowed(pin))
	{
		pinState[getPinIndex(pin)] = false;
		digitalWrite(pin, LOW);
	}
}
void BlinkBuzz::onoff(int pin, int duration)
{
	if (isAllowed(pin))
	{
		on(pin);
		delay(duration);
		off(pin);
	}
}
void BlinkBuzz::onoff(int pin, int duration, int times)
{
	if (isAllowed(pin))
	{
		for (int i = 0; i < times; i++)
		{
			onoff(pin, duration);
			delay(duration);
		}
	}
}
void BlinkBuzz::onoff(int pin, int duration, int times, int pause)
{
	if (isAllowed(pin))
	{
		for (int i = 0; i < times; i++)
		{
			onoff(pin, duration);
			delay(pause);
		}
	}
}

#pragma endregion

#pragma region Asynchronous functions

void BlinkBuzz::update(int curMS)
{
	if (!enableAsync)
		return;

	if (curMS == -1)
		curMS = millis();

	for (int i = 0; i < numPins; i++)
	{
		if (curMS >= pinQ[i][pinQStart[i]])
		{
			if (pinQStart[i] != pinQEnd[i])
			{
				if (pinState[i])
					off(allowedPins[i]);

				else
					on(allowedPins[i]);

				pinQStart[i] = (pinQStart[i] + 1) % MAX_QUEUE;
			}
		}
	}
}

/*

1 2 3 4 5 6 7 8 9
|     |
I O ?

//TODO:
- add indefinite pattern support
- remove queue spacing in favor of using duration and pause when times > 0

*/

void BlinkBuzz::aonoffhelper(int idx, int timeStamp)
{
	pinQ[idx][pinQEnd[idx]] = timeStamp;
	pinQEnd[idx] = (pinQEnd[idx] + 1) % MAX_QUEUE;
}

// find the starting timestamp of an enquing action based on the current queue state
int BlinkBuzz::calcTimeStamp(int pin, int pinIndex)
{
	double timeStamp = millis();
	if (pinQStart[pinIndex] != pinQEnd[pinIndex])
	{ // if queue is not empty, add spacing
		timeStamp += pinQ[pinIndex][(pinQEnd[pinIndex] - 1) % MAX_QUEUE] + queueSpacing;
	}
	return timeStamp;
}

void BlinkBuzz::aonoff(int pin, int duration, bool overwrite)
{
	int pinIndex = getPinIndex(pin);
	if (isAllowed(pin) && enableAsync)
	{
		if (overwrite)
			clearQueue(pin, LOW);

		int t = calcTimeStamp(pin, pinIndex);
		aonoffhelper(pinIndex, t);
		aonoffhelper(pinIndex, t + duration);
	}
}

void BlinkBuzz::aonoff(int pin, int duration, int times, bool overwrite)
{
	aonoff(pin, duration, times, duration, overwrite);
}

void BlinkBuzz::aonoff(int pin, int duration, int times, int pause, bool overwrite)
{
	int pinIndex = getPinIndex(pin);
	if (isAllowed(pin) && enableAsync)
	{
		if (overwrite)
			clearQueue(pin, LOW);

		int timeStamp = calcTimeStamp(pin, pinIndex);
		aonoffhelper(pinIndex, timeStamp);
		for (int i = 0; i < times; i++)
		{
			aonoffhelper(pinIndex, timeStamp += duration);
			if (i != times - 1)
				aonoffhelper(pinIndex, timeStamp += pause);
		}
	}
}

void BlinkBuzz::clearQueue(int pin)
{
	if (isAllowed(pin) && enableAsync)
	{
		int idx = getPinIndex(pin);
		pinQStart[idx] = 0;
		pinQEnd[idx] = 0;
	}
}

void BlinkBuzz::clearQueue(int pin, int resetTo)
{
	if (isAllowed(pin) && enableAsync)
	{
		if (resetTo == LOW)
			off(pin);
		else if (resetTo == HIGH)
			on(pin);
		clearQueue(pin);
	}
}

#pragma endregion

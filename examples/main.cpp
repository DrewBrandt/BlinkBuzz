

#include "../src/BlinkBuzz.h"

#define BUZZER 33

int pins[] = {13, BUZZER};
BlinkBuzz bb = BlinkBuzz(pins, 2, true);

#ifndef ARDUINO
void loop(int = 0);
void setup();
int main()
{
	setup();
	loop(millis());
}
#endif // !ARDUINO

void setup()
{
	bb.aonoff(13, 200, 3, 1000); // blink pin 13 3 times, 200ms on, 1000ms off asynchronously
}

void loop(int start)
{
	int time = 0, last = 0;
	bool passed1500 = false;
	while (true)
	{
		time = millis();
		bb.update();
		if (time - last > 100)
		{
			last = time;
			if (!passed1500 && time - start > 1500)
			{ // turn on pin 33 after 1.5 seconds, for 1 second
				passed1500 = true;
				bb.aonoff(BUZZER, 1000);
			}

#ifndef ARDUINO
			printf("%i.%03i between\n", time / 1000, time % 1000); // demonstrate asynchronous blinking
#else
			char buf[50];
			snprintf(buf, 50, "%i.%03i between\n", time / 1000, time % 1000);
			Serial.println(buf);
#endif
			if (time - start > 5000)
				break;
		}
	}
}
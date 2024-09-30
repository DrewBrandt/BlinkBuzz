#include "../src/BlinkBuzz.h"

#define BUZZER 33

int pins[] = {13, BUZZER};
BlinkBuzz bb = BlinkBuzz(pins, 2, true, 50);

#ifndef ARDUINO
void loop();
void setup();
int main()
{
	setup();
	loop();
}
#endif // !ARDUINO

void setup()
{
	// bb.aonoff(13, 200, 3, 1000); // blink pin 13 3 times, 200ms on, 1000ms off asynchronously
	// bb.aonoff(13, 50, 3, 500);	 // blink pin 13 3 times, 50ms on, 500ms off asynchronously
	// BBPattern pattern(100, 3, 1000);
	// bb.aonoff(BUZZER, pattern); // blink pin 33 3 times, 100ms on, 1000ms off

	// test BBPattern
	BBPattern s(50, 3, 200);
	BBPattern o(500, 3, 200);
	BBPattern sos = s.a(o).a(s);

	bb.aonoff(13, sos, true); // infinitely repeat the SOS pattern
	// OR
	bb.aonoff(13, s.a(o).a(s), true); // infinitely repeat the SOS pattern
	// OR
	bb.aonoff(13, sos.r(1000), true); // infinitely repeat the SOS pattern, resting for 1 sec between repetitions
	// OR
	bb.aonoff(13, s.r(1000).a(o).r(1000).a(s).r(1000), true); // infinitely repeat the SOS pattern, resting for 1 sec between letters
}

void loop()
{
	int time = 0, last = 0;
	bool passed1500 = false;
	while (true)
	{
		time = millis();
		bb.update();
		// if (time > 5000)
		// break;
		if (time - last >= 100)
		{
			last = time;
			if (!passed1500 && time >= 1500)
			{ // turn on pin 33 after 1.5 seconds, for 1 second
				passed1500 = true;
				// bb.aonoff(BUZZER, 1000);
			}

#ifndef ARDUINO
			// printf("%i.%03i\n", time / 1000, time % 1000); // demonstrate asynchronous blinking
#else
			char buf[50];
			snprintf(buf, 50, "%i.%03i between\n", time / 1000, time % 1000);
			Serial.println(buf);
#endif
		}
	}
}
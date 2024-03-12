

#include "BlinkBuzz.h"

int pins[] = { 13, 33 };
BlinkBuzz bb = BlinkBuzz(pins, 2, true);
void loop(int);

int main()
{
	int start = millis();
	bb.aonoff(13, 200, 3, 1000);
	loop(start);
}

void loop(int start) {
	int time = 0, last = 0;
	bool passed1500 = false;
	while (true) {
		time = millis();
		bb.update();
		if (time - last > 100) {
			last = time;
			if (!passed1500 && time - start > 1500) {
				passed1500 = true;
				//bb.aonoff(33, 1000);
			}
			printf("%i.%03i between\n", time / 1000, time % 1000);
			if (time - start > 5000) {
				break;
			}
		}
	}
}
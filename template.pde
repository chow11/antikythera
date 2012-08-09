// increase the heap size from 2k to 8k.
#define CHANGE_HEAP_SIZE(size) __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")
CHANGE_HEAP_SIZE(0x2000);

#include "Antikythera.h"
#include <WS2801.h>
#include <HardwareSerial.h>

// device management
#define SERIAL_READ_TIMEOUT 1000
#define ATK_PROCESS_TIMEOUT 40        // 25 fps
#define GPS_READ_TIMEOUT 1000
#define COMPASS_READ_TIMEOUT 250
#define ACCELEROMETER_READ_TIMEOUT 250
#define TOUCH_READ_TIMEOUT 250

struct TIMERS {
	bool serialEnabled;
	long serialRead;
	bool atkEnabled;
	long atkProcess;
	bool gpsEnabled;
	long gpsRead;
	bool compassEnabled;
	long compassRead;
	bool accelerometerEnabled;
	long accelerometerRead;
	bool touchEnabled;
	long touchRead;
};

TIMERS timers;

void setup()
{
	Antikythera::numDisplays = 1;
	Antikythera::displays = ATKDisplay[1];
	displays[1] = new WS2801_SPI0;

	Serial.begin(115200);
	initializeSerial();
	timers.serialEnabled = true;
}

void loop()
{
	long now = millis();

	if (timers.gpsEnabled && (timers.gpsRead + GPS_READ_TIMEOUT) < now) {
		timers.gpsRead = now;
		now = millis();
	}

	if (timers.compassEnabled && (timers.compassRead + COMPASS_READ_TIMEOUT) < now) {
		timers.compassRead = now;
		now = millis();
	}

	if (timers.accelerometerEnabled && (timers.accelerometerRead + ACCELEROMETER_READ_TIMEOUT) < now) {
		timers.accelerometerRead = now;
		now = millis();
	}

	if (timers.atkEnabled && (timers.atkProcess + ATK_PROCESS_TIMEOUT) < now) {
		Antikythera::operators[0].process(now);
		timers.atkProcess = now;
		now = millis();
	}

	if (timers.serialEnabled && (timers.serialRead + SERIAL_READ_TIMEOUT) < now) {
		if (Serial.available() > 0) {
			ATK::load(&Serial);
			timers.atkEnabled = true;
		}

		timers.serialRead = now;
	}
}

void initializeSerial() {
	long timer;

	while (Serial.available() <= 0) {
		long now = millis();

		if ((timer + 5000) < now) {
			Serial.println("Send initial program.");
			timer = now;
		}
	}
}

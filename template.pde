// increase the heap size from 2k to 8k.
#define CHANGE_HEAP_SIZE(size) __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")
CHANGE_HEAP_SIZE(0x2000);
//extern __attribute__((section("linker_defined"))) char _heap;
//extern __attribute__((section("linker_defined"))) char _min_heap_size;

#include <WProgram.h>
#include <HardwareSerial.h>
#include <Antikythera.h>
#include <ATKColor.h>
#include <WS2801.h>

#define  NUM_DISPLAYS      1
#define  DISPLAY_WIDTH     8
#define  DISPLAY_HEIGHT    38
#define  DISPLAY_LAYERS    1

// device management
#define SERIAL_READ_TIMEOUT 1000
#define ANTIKYTHERA_TIMEOUT 25        // 40 fps
#define GPS_READ_TIMEOUT 1000
#define COMPASS_READ_TIMEOUT 250
#define ACCELEROMETER_READ_TIMEOUT 250
#define TOUCH_READ_TIMEOUT 250


struct TIMERS {
  bool serialEnabled;
  unsigned long serialRead;
  bool antikytheraEnabled;
  unsigned long antikythera;
  bool gpsEnabled;
  unsigned long gpsRead;
  bool compassEnabled;
  unsigned long compassRead;
  bool accelerometerEnabled;
  unsigned long accelerometerRead;
  bool touchEnabled;
  unsigned long touchRead;
};

TIMERS timers;

char out[128];

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println("randomSeed set");
//  WS2801 *ws2801 = new WS2801(_DSPI0_BASE, PIN_DSPI0_SS);

  WS2801 *ws2801 = new WS2801((p32_spi *) _SPI2_BASE_ADDRESS, PIN_DSPI0_SS);
  Serial.println("WS2801 object instantiated");
  ws2801->initialize(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_LAYERS);
  Serial.println("WS2801 initialized");
  Antikythera::numDisplays = NUM_DISPLAYS;
  Antikythera::displays = new ATKIDisplay*[NUM_DISPLAYS];
  Serial.println("ATK added display pointers");
  Antikythera::displays[0] = ws2801;
  Serial.println("set ATK display[0] = WS2801");

  Serial.println("Send initial program.");
  timers.serialEnabled = true;
}

void loop()
{
  unsigned long now = millis();

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

  if (timers.antikytheraEnabled && (timers.antikythera + ANTIKYTHERA_TIMEOUT) < now) {
    unsigned long t0 = millis();
#ifdef  ANTIKYTHERA_DEBUG
    if (!Antikythera::evaluate(now, &Serial)) {
#else
    if (!Antikythera::evaluate(now)) {
#endif
      timers.antikytheraEnabled = false;
      Serial.print("\nError running program: ");
#ifdef  ANTIKYTHERA_DEBUG
      Serial.println(Antikythera::lastErrorString);
#endif
    }
    unsigned long t1 = millis();
    sprintf(out, "time elapsed:%ul", (t1 - t0));
    Serial.println(out);
    timers.antikythera = now;
    now = millis();
  }

  if (timers.serialEnabled && (timers.serialRead + SERIAL_READ_TIMEOUT) < now) {
    if (Serial.available() > 0) {
      if (Antikythera::load(&Serial)) {
        Serial.println("\nProgram loaded and running.");
        timers.antikytheraEnabled = true;
      } else {
        Serial.print("\nError loading program: ");
#ifdef  ANTIKYTHERA_DEBUG
        Serial.println(Antikythera::lastErrorString);
#endif
      }
    }

    timers.serialRead = now;
  }
}

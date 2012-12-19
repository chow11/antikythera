// increase the heap size from 2k to 8k.
#define CHANGE_HEAP_SIZE(size) __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")
CHANGE_HEAP_SIZE(0x2000);

#define NOP __asm__ __volatile__ ("nop\n");
#define NOP_25NS NOP; NOP;
#define NOP_50NS NOP_25NS; NOP_25NS;
#define NOP_100NS NOP_50NS; NOP_50NS;
#define NOP_200NS NOP_100NS; NOP_100NS;
#define NOP_250NS NOP_200NS; NOP_50NS;

#include <WProgram.h>
#include <HardwareSerial.h>

    union RGBA {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        } color;
        uint32_t bits;

        RGBA() { bits = 0; }
        RGBA(uint32_t color) { bits = color; }
        RGBA(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) { color.r = cr; color.g = cg; color.b = cb; color.a = ca; }

        operator uint32_t() { return bits; }
    };

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 2
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT
#define NUM_WS2811_OUTPUTS 2
#define OUTPUT_PIN1 4
#define OUTPUT_PIN2 5

char out[128];
RGBA frame[DISPLAY_SIZE];

void send_frame() {
  register p32_ioport *iop1 = (p32_ioport *)portModeRegister(digitalPinToPort(OUTPUT_PIN1));
#if (NUM_WS2811_OUTPUTS > 1)
  register p32_ioport *iop2 = (p32_ioport *)portModeRegister(digitalPinToPort(OUTPUT_PIN2));
#endif
  register uint32_t bit1 = digitalPinToBitMask(OUTPUT_PIN1);
#if (NUM_WS2811_OUTPUTS > 1)
  register uint32_t bit2 = digitalPinToBitMask(OUTPUT_PIN2);
#endif
  register uint32_t c1;
#if (NUM_WS2811_OUTPUTS > 1)
  register uint32_t c2;
#endif
  register uint32_t mask;
  RGBA *p1 = frame;
#if (NUM_WS2811_OUTPUTS > 1)
  RGBA *p2 = p1 + DISPLAY_WIDTH;
#endif
  RGBA *e = p1 + DISPLAY_WIDTH;

  noInterrupts();
  while (p1 != e) {
    c1 = p1->color.g << 16 | p1->color.r << 8 | p1->color.b;
#if (NUM_WS2811_OUTPUTS > 1)
    c2 = p2->color.g << 16 | p2->color.r << 8 | p2->color.b;
#else
    NOP_50NS;
#endif
    mask = 0x00800000;
    while (mask > 1) {
      NOP_25NS;
      // 0 US
      iop1->lat.set = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      iop2->lat.set = bit2;
#else
      NOP;
#endif
      NOP_200NS;
      // .250 US
      if (~c1 & mask) iop1->lat.clr = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      if (~c2 & mask) iop2->lat.clr = bit2;
#else
      NOP_100NS;
#endif
      NOP_250NS;
      // .650 US
      if (c1 & mask) iop1->lat.clr = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      if (c2 & mask) iop2->lat.clr = bit2;
#else
      NOP_50NS;
#endif
      NOP_250NS;
      NOP_100NS;
      NOP_25NS;
      mask >>= 1;
      // ~1.200 US
    }
      iop1->lat.set = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      iop2->lat.set = bit2;
#else
      NOP;
#endif
      NOP_200NS;
      // .250 US
      if (~c1 & mask) iop1->lat.clr = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      if (~c2 & mask) iop2->lat.clr = bit2;
#else
      NOP_100NS;
#endif
      NOP_250NS;
      // .650 US
      if (c1 & mask) iop1->lat.clr = bit1;
#if (NUM_WS2811_OUTPUTS > 1)
      if (c2 & mask) iop2->lat.clr = bit2;
#else
      NOP_50NS;
#endif
    NOP_100NS;
    ++p1;
#if (NUM_WS2811_OUTPUTS > 1)
    ++p2;
#else
      NOP_25NS;
#endif
  }
  interrupts();

  delayMicroseconds(50);

  memset(frame, 0 , DISPLAY_SIZE * 4);
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN1, OUTPUT);
  pinMode(OUTPUT_PIN2, OUTPUT);
}

void loop() {
  uint16_t i, j;
  for (i = 0; i < 39; i++) {
    frame[i] = RGBA(7, 0, 0, 255);
    frame[DISPLAY_WIDTH + i] = RGBA(63, 0, 0, 255);
  }

  for (i = 40; i < 119; i++) {
    frame[i] = RGBA(0, 7, 0, 255);
    frame[DISPLAY_WIDTH + i] = RGBA(0, 63, 0, 255);
  }
  for (i = 120; i < DISPLAY_WIDTH; i++) {
    frame[i] = RGBA(0, 0, 7, 255);
    frame[DISPLAY_WIDTH + i] = RGBA(0, 0, 63, 255);
  }

  send_frame();
  Serial.println("frame sent.");

  delay(17);
}

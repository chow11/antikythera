#include <plib.h>

// increase the heap size from 2k to 8k.
#define CHANGE_HEAP_SIZE(size) __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")
CHANGE_HEAP_SIZE(0x2000);
#define NOP __asm__ __volatile__ ("nop\n");
#define NOP_25NS NOP; NOP;
#define NOP_50NS NOP_25NS; NOP_25NS;
#define NOP_100NS NOP_50NS; NOP_50NS;
#define NOP_150NS NOP_100NS; NOP_50NS;
#define NOP_200NS NOP_100NS; NOP_100NS;
#define NOP_250NS NOP_200NS; NOP_50NS;
#define NOP_300NS NOP_200NS; NOP_100NS;
#define NOP_400NS NOP_200NS; NOP_200NS;
#define NOP_500NS NOP_250NS; NOP_250NS;

#include <WProgram.h>
#include <HardwareSerial.h>
#include <ATKColor.h>

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 2
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT
#define OUTPUT_PIN1 4
#define OUTPUT_PIN2 5

char out[128];
ATKIColor::RGBA frame[DISPLAY_SIZE];
uint8_t hue;

void send_frame() {
uint32_t st;
uint32_t et;

  p32_ioport *iop1 = (p32_ioport *)portModeRegister(digitalPinToPort(OUTPUT_PIN1));
  p32_ioport *iop2 = (p32_ioport *)portModeRegister(digitalPinToPort(OUTPUT_PIN2));
  uint32_t bit1 = digitalPinToBitMask(OUTPUT_PIN1);
  uint32_t bit2 = digitalPinToBitMask(OUTPUT_PIN2);
  register ATKIColor::RGBA *p1 = frame;
  register ATKIColor::RGBA *p2 = p1 + DISPLAY_WIDTH;
  ATKIColor::RGBA *e = p2;
  uint8_t mask = 0x08;
  uint8_t c1;
  uint8_t c2;

  st = ReadCoreTimer();
  INTDisableInterrupts();
  while (p1 != e) {
    c1 = p1->color.g;
    c2 = p2->color.g;
    mask = 0x80;
    while (mask > 1) {
      /*
et = ReadCoreTimer();
sprintf(out, "span(%lu) next(%x/%x) mask %x", et - st - 4, c1, c2, mask);
Serial.println(out);
st = ReadCoreTimer();
*/
      iop1->lat.set = bit1;
      iop2->lat.set = bit2;
      NOP_100NS;
      NOP_50NS;
      NOP_25NS;
      if (c1 & mask) {
        if (c2 & mask) { // both on
          NOP_250NS;
          NOP_100NS;
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_300NS;
          NOP_100NS;
        } else {
          iop2->lat.clr = bit2;
          NOP_250NS;
          NOP_50NS;
          iop1->lat.clr = bit1;
          NOP_300NS;
          NOP_50NS;
          NOP_25NS;
        }
      } else {
        if (c2 & mask) {
          iop1->lat.clr = bit1;
          NOP_250NS;
          NOP_50NS;
          iop2->lat.clr = bit2;
          NOP_400NS;
          NOP_50NS;
        } else { // both off
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_500NS;
          NOP_200NS;
        }
      }
      mask >>= 1;
    }
    iop1->lat.set = bit1;
    iop2->lat.set = bit2;
    NOP_100NS;
    NOP_50NS;
    NOP_25NS;
    if (c1 & mask) {
      if (c2 & mask) { // both on
        NOP_400NS;
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_400NS;
        NOP_25NS;
      } else {
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_50NS;
        iop1->lat.clr = bit1;
        NOP_400NS;
        NOP_25NS;
        NOP;
      }
    } else {
      if (c2 & mask) {
        iop1->lat.clr = bit1;
        NOP_250NS;
        NOP_50NS;
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_200NS;
        NOP_25NS;
        NOP;
      } else { // both off
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_500NS;
        NOP_200NS;
        NOP_25NS;
      }
    }
    c1 = p1->color.r;
    c2 = p2->color.r;
    mask = 0x80;
    while (mask > 1) {
      iop1->lat.set = bit1;
      iop2->lat.set = bit2;
      NOP_100NS;
      NOP_50NS;
      NOP_25NS;
      if (c1 & mask) {
        if (c2 & mask) { // both on
          NOP_250NS;
          NOP_100NS;
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_300NS;
          NOP_100NS;
        } else {
          iop2->lat.clr = bit2;
          NOP_250NS;
          NOP_50NS;
          iop1->lat.clr = bit1;
          NOP_300NS;
          NOP_50NS;
          NOP_25NS;
        }
      } else {
        if (c2 & mask) {
          iop1->lat.clr = bit1;
          NOP_250NS;
          NOP_50NS;
          iop2->lat.clr = bit2;
          NOP_400NS;
          NOP_50NS;
        } else { // both off
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_500NS;
          NOP_200NS;
        }
      }
      mask >>= 1;
    }
    iop1->lat.set = bit1;
    iop2->lat.set = bit2;
    NOP_100NS;
    NOP_50NS;
    NOP_25NS;
    if (c1 & mask) {
      if (c2 & mask) { // both on
        NOP_400NS;
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_400NS;
        NOP_25NS;
      } else {
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_50NS;
        iop1->lat.clr = bit1;
        NOP_400NS;
        NOP_25NS;
        NOP;
      }
    } else {
      if (c2 & mask) {
        iop1->lat.clr = bit1;
        NOP_250NS;
        NOP_50NS;
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_200NS;
        NOP_25NS;
        NOP;
      } else { // both off
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_500NS;
        NOP_200NS;
        NOP_25NS;
      }
    }
    c1 = p1++->color.b;
    c2 = p2++->color.b;
    mask = 0x80;
    while (mask > 1) {
      iop1->lat.set = bit1;
      iop2->lat.set = bit2;
      NOP_100NS;
      NOP_50NS;
      NOP_25NS;
      if (c1 & mask) {
        if (c2 & mask) { // both on
          NOP_250NS;
          NOP_100NS;
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_300NS;
          NOP_100NS;
        } else {
          iop2->lat.clr = bit2;
          NOP_250NS;
          NOP_50NS;
          iop1->lat.clr = bit1;
          NOP_300NS;
          NOP_50NS;
          NOP_25NS;
        }
      } else {
        if (c2 & mask) {
          iop1->lat.clr = bit1;
          NOP_250NS;
          NOP_50NS;
          iop2->lat.clr = bit2;
          NOP_400NS;
          NOP_50NS;
        } else { // both off
          iop1->lat.clr = bit1;
          iop2->lat.clr = bit2;
          NOP_500NS;
          NOP_200NS;
        }
      }
      mask >>= 1;
    }
    iop1->lat.set = bit1;
    iop2->lat.set = bit2;
    NOP_100NS;
    NOP_50NS;
    NOP_25NS;
    if (c1 & mask) {
      if (c2 & mask) { // both on
        NOP_250NS;
        NOP_100NS;
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_25NS;
      } else {
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_50NS;
        iop1->lat.clr = bit1;
        NOP_200NS;
        NOP_25NS;
      }
    } else {
      if (c2 & mask) {
        iop1->lat.clr = bit1;
        NOP_250NS;
        NOP_50NS;
        iop2->lat.clr = bit2;
        NOP_250NS;
        NOP_50NS;
        NOP_25NS;
      } else { // both off
        iop1->lat.clr = bit1;
        iop2->lat.clr = bit2;
        NOP_500NS;
        NOP_50NS;
      }
    }
  }
  INTEnableInterrupts();

  et = ReadCoreTimer();
  delayMicroseconds(50);

  memset(frame, 0 , DISPLAY_SIZE * 4);
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN1, OUTPUT);
  pinMode(OUTPUT_PIN2, OUTPUT);
}

void loop() {
  uint16_t i;
  for (i = 0; i < 39; i++) {
//  for (i = 0; i < DISPLAY_WIDTH; i++) {
//    frame[i] = ATKIColor::HSVAtoRGBA(ATKIColor::HSVA((hue + i) % 252, 191, 15, 255));
//    frame[DISPLAY_SIZE - 1 - i] = ATKIColor::HSVAtoRGBA(ATKIColor::HSVA((hue + 240 + i) % 252, 191, 15, 255));
    frame[i] = ATKIColor::RGBA(63, 0, 0, 255);
    frame[DISPLAY_WIDTH + i] = ATKIColor::RGBA(63, 0, 0, 255);
  }

  for (i = 40; i < 119; i++) {
    frame[i] = ATKIColor::RGBA(0, 63, 0, 255);
    frame[DISPLAY_WIDTH + i] = ATKIColor::RGBA(0, 63, 0, 255);
  }
  for (i = 120; i < DISPLAY_WIDTH; i++) {
    frame[i] = ATKIColor::RGBA(0, 0, 63, 255);
    frame[DISPLAY_WIDTH + i] = ATKIColor::RGBA(0, 0, 63, 255);
  }

  send_frame();
  Serial.println("frame sent.");

  delay(17);
  
  hue = (++hue % 252);
}

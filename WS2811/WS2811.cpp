/*
 * WS2811.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: chow
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <math.h>
#include <WS2811.h>
#include <WProgram.h>


#define NOP __asm__ __volatile__ ("nop\n");
#define NOP_25NS NOP; NOP;
#define NOP_50NS NOP_25NS; NOP_25NS;
#define NOP_100NS NOP_50NS; NOP_50NS;
#define NOP_200NS NOP_100NS; NOP_100NS;
#define NOP_250NS NOP_200NS; NOP_50NS;


#if (NUM_WS2811_OUTPUTS > 1)
WS2811::WS2811(uint8_t pin1, uint8_t pin2) {
#else
WS2811::WS2811(uint8_t pin1) {
#endif
	m_pin1 = pin1;
#if (NUM_WS2811_OUTPUTS > 1)
	m_pin2 = pin2;
#endif
	m_iop1 = (p32_ioport *)portModeRegister(digitalPinToPort(pin1));
#if (NUM_WS2811_OUTPUTS > 1)
	m_iop2 = (p32_ioport *)portModeRegister(digitalPinToPort(pin2));
#endif
	m_bits1 = digitalPinToBitMask(pin1);
#if (NUM_WS2811_OUTPUTS > 1)
	m_bits2 = digitalPinToBitMask(pin2);
#endif

	m_displayWidth = 0;
	m_displayHeight = 0;
	m_frameWidth = 0;
	m_frameHeight = 0;
	m_frameSize = 0;
	m_numLayers = 0;
	m_frames = NULL;
}

WS2811::~WS2811() {
	if (m_frames != NULL) {
		delete[] m_frames;
	}
}

void WS2811::begin() {
	pinMode(m_pin1, OUTPUT);
#if (NUM_WS2811_OUTPUTS > 1)
	pinMode(m_pin2, OUTPUT);
#endif
}

void WS2811::initialize(int16_t displayWidth, int16_t displayHeight, int16_t frameWidth, int16_t frameHeight, int16_t numLayers) {
	m_displayWidth = displayWidth;
	m_displayHeight = displayHeight;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_numLayers = numLayers;
	m_frameSize = frameWidth * frameHeight;

	m_frames = new ATKIColor::RGBA[numLayers * m_frameWidth * m_frameHeight];
}

// blue t-> green -> red
// layer 0 is background drawn (0 or 255 alpha only)
// layer 1 is premultiplied alpha
void WS2811::render(int16_t frameX, int16_t frameY) {
	register p32_ioport *iop1 = m_iop1;
#if (NUM_WS2811_OUTPUTS > 1)
	register p32_ioport *iop2 = m_iop2;
#endif
	register uint32_t bits1 = m_bits1;
#if (NUM_WS2811_OUTPUTS > 1)
	register uint32_t bits2 = m_bits2;
#endif
	register uint32_t c1;
#if (NUM_WS2811_OUTPUTS > 1)
	register uint32_t c2;
#endif
	register uint32_t mask;
	ATKIColor::RGBA *p1 = m_frames;
#if (NUM_WS2811_OUTPUTS > 1)
	ATKIColor::RGBA *p2 = p1 + WS2811_STRIP_LENGTH;
#endif
	ATKIColor::RGBA *e = p1 + WS2811_STRIP_LENGTH;

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
			iop1->lat.set = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
			iop2->lat.set = bits2;
#else
			NOP;
#endif
			NOP_200NS;
			// .250 US
			if (~c1 & mask) iop1->lat.clr = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
			if (~c2 & mask) iop2->lat.clr = bits2;
#else
			NOP_100NS;
#endif
			NOP_250NS;
	  // .650 US
			if (c1 & mask) iop1->lat.clr = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
			if (c2 & mask) iop2->lat.clr = bits2;
#else
			NOP_50NS;
#endif
			NOP_250NS;
			NOP_100NS;
			NOP_25NS;
			mask >>= 1;
			// ~1.200 US
		}
		iop1->lat.set = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
		iop2->lat.set = bits2;
#else
		NOP;
#endif
		NOP_200NS;
		// .250 US
		if (~c1 & mask) iop1->lat.clr = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
		if (~c2 & mask) iop2->lat.clr = bits2;
#else
		NOP_100NS;
#endif
		NOP_250NS;
		// .650 US
		if (c1 & mask) iop1->lat.clr = bits1;
#if (NUM_WS2811_OUTPUTS > 1)
		if (c2 & mask) iop2->lat.clr = bits2;
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

//	delayMicroseconds(50);

	memset(m_frames, 0 , m_frameWidth * m_frameHeight * 4 * m_numLayers);
};

ATKIColor::RGBA* WS2811::framebuffer(int16_t layer) {
	return m_frames + layer * m_frameWidth * m_frameHeight;
}

void WS2811::circle(int16_t x, int16_t y, int16_t r, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
	circle(x, y, r, ATKIColor::HSVAtoRGBA(c), thickness, style, layer);
}

void WS2811::circle(int16_t x1, int16_t y1, int16_t r, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
	int16_t f = 1 - r;
	int16_t ddfx = 1;
	int16_t ddfy = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	point(x1, y1 + r, c, style, layer);
	point(x1, y1 - r, c, style, layer);
	point(x1 + r, y1, c, style, layer);
	point(x1 - r, y1, c, style, layer);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddfy += 2;
			f += ddfy;
		}
		x++;
		ddfx += 2;
		f += ddfx;

		point(x1 + x, y1 + y, c, style, layer);
		point(x1 - x, y1 + y, c, style, layer);
		point(x1 + x, y1 - y, c, style, layer);
		point(x1 - x, y1 - y, c, style, layer);
		point(x1 + y, y1 + x, c, style, layer);
		point(x1 - y, y1 + x, c, style, layer);
		point(x1 + y, y1 - x, c, style, layer);
		point(x1 - y, y1 - x, c, style, layer);
	}
}

void WS2811::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
	line(x1, y1, x2, y2, ATKIColor::HSVAtoRGBA(c), thickness, style, layer);
	/*
	 * Xiaolin Wu's Algorithm: http://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
	 * 						   http://www.codeproject.com/Articles/13360/Antialiasing-Wu-Algorithm#dwuln
	 */
/*
	if (y1 > y2) {
		int16_t t = y1; y1 = y2; y2 = t;
		t = x1; x1 = x2; x2 = t;
	}
	point(x1, y1, c, style, layer);

	int16_t dx = x2 - x1;
	int16_t sx = 1;
	if (dx < 0) {
		sx = -1;
		dx = -dx;
	}
	int16_t dy = y2 - y1;
	if (dy == 0) {
		// Horizontal line
		while (dx-- != 0) {
			x1 += sx;
			point(x1, y1, c, style, layer);
		}
		return;
	}
	if (dx == 0) {
		// Vertical line
		do {
	         y1++;
	         point(x1, y1, c, style, layer);
		} while (--dy != 0);
		return;
	}
	if (dx == dy) {
		// Diagonal line
		do {
			x1 += sx;
			y1++;
			point(x1, y1, c, style, layer);
		} while (--dy != 0);
		return;
	}
	uint16_t err = 0;
	if (dy > dx) {
		uint16_t errx = ((unsigned long) dx << 16) / (unsigned long) dy;
		while (--dy) {
			uint16_t errt = err;
			err += errx;
			if (err <= errt) {
				x1 += sx;
			}
			y1++;
			uint16_t w = err >> 8;
			point(x1, y1, c.brightness(w / 256.0), style, layer);
			point(x1 + sx, y1, c.brightness((w ^ 255) / 256.0), style, layer);
		}
		point(x2, y2, c, style, layer);
		return;
	}
	uint16_t errx = ((unsigned long) dy << 16) / (unsigned long) dx;
	while (--dx) {
		uint16_t errt = err;
		err += errx;
		if (err <= errt) {
			y1++;
		}
		x1 += sx;
		uint16_t w = err >> 8;
		point(x1, y1, c.brightness(w / 256.0), style, layer);
		point(x1, y1 + 1, c.brightness((w ^ 255) / 256.0), style, layer);
	}
	point(x2, y2, c, style, layer);
*/
}

/*
 * Bresenham's Algorithm: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
void WS2811::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
//	ATKColor::RGBA *p = &m_frames[layer * m_frameSize + x1 * m_frameHeight + y1];
	int16_t dx = x2 - x1;
	dx = abs(dx);
	int16_t dy = y2 - y1;
	dy = abs(dy);
	int16_t sx = (x1 < x2) ? 1 : -1;
	int16_t sy = (y1 < y2) ? 1 : -1;
	int16_t err = dx - dy;

	point(x1, y1, c, style, layer);
//	*p = c;
	while((x1 != x2) || (y1 != y2)) {
		int16_t e2 = err * 2;
		if (e2 > (-dy)) {
			err -= dy;
			x1 += sx;
//			p += sx * m_frameHeight;
		}
		if (e2 < (dx)) {
			err += dx;
			y1 += sy;
//			p += sy;
		}
		point(x1, y1, c, style, layer);
//		*p = c;
	}
}

void WS2811::point(int16_t x, int16_t y, ATKIColor::HSVA c, int16_t style, int16_t layer) {
	x %= m_frameWidth;
	if (x < 0) {
		x = x + m_frameWidth - 1;
	}
	if (y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + x * m_frameHeight + y] = ATKIColor::HSVAtoRGBA(c);
	}
}

void WS2811::point(int16_t x, int16_t y, ATKIColor::RGBA c, int16_t style, int16_t layer) {
	x %= m_frameWidth;
	if (x < 0) {
		x = x + m_frameWidth - 1;
	}
	if (y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + x * m_frameHeight + y] = c;
	}
}


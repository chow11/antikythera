/*
 * WS2801.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: chow
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <math.h>
#include <WS2801.h>
#include <WProgram.h>


WS2801::WS2801(p32_spi *pspi, uint8_t pinSS) {
	m_pspi = pspi;
	m_pinSS = pinSS;
	
	m_displayWidth = 0;
	m_displayHeight = 0;
	m_frameWidth = 0;
	m_frameHeight = 0;
	m_frameSize = 0;
	m_numLayers = 0;
	m_frames = NULL;
}

WS2801::~WS2801() {
	if (m_frames != NULL) {
		delete[] m_frames;
	}
}

void WS2801::begin() {
	uint8_t			bTmp;
	uint16_t		brg;

	/* Initialize the pins. The pin directions for SDO, SDI and SCK
	** are set automatically when the SPI controller is enabled. The
	** SS pin isn't explicitly used by the SPI controller when in
	** master mode, so we need to initialize it ourselves.
	*/
	setPinSelect(m_pinSS);

	/* Disable and reset the SPI controller.
	*/
	m_pspi->sxCon.reg = 0;

	/* Clear the receive buffer.
	*/
	bTmp = m_pspi->sxBuf.reg;

	/* Set the default baud rate.
	*/
	brg = (uint16_t)((F_CPU / (2 * _DSPI_SPD_DEFAULT)) - 1);
	m_pspi->sxBrg.reg = brg;

	/* Clear the receive overflow bit and receive overflow error flag
	*/
	m_pspi->sxStat.clr = (1 << _SPISTAT_SPIROV);

	/* Enable the SPI controller.
	** Warning: if the SS pin ever becomes a LOW INPUT then SPI 
	** automatically switches to Slave, so the data direction of 
	** the SS pin MUST be kept as OUTPUT.
	*/
	m_pspi->sxCon.reg = 0;
	m_pspi->sxCon.set = (1 << _SPICON_ON) + (1 << _SPICON_MSTEN) + DSPI_MODE0;
};

void WS2801::end() {
	m_pspi->sxCon.reg = 0;	
};


/* ------------------------------------------------------------ */
/***	DSPI::setSpeed
**
**	Parameters:
**		spd		- clock speed to set in HZ
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This sets the SPI clock speed to the highest supported
**		frequency that doesn't exceed the requested value. It computes
**		the appropriate value to load into the SPI baud register
**		based on requested clock speed and peripheral bus frequency.
*/

void WS2801::setSpeed(uint32_t speed) {
	uint16_t	brg;

	/* Compute the baud rate divider for this frequency.
	*/
	brg = (uint16_t)((F_CPU / (2 * speed)) - 1);

	/* That the baud rate value is in the correct range.
	*/
	if (brg == 0xFFFF) {
		/* The user tried to set a frequency that is too high to support.
		** Set it to the highest supported frequency.
		*/
		brg = 0;
	}

	if (brg > 0x1FF) {
		/* The user tried to set a frequency that is too low to support.
		** Set it to the lowest supported frequency.
		*/
		brg = 0x1FF;
	}

	/* Write the value to the SPI baud rate register. Section 23. SPI
	** of the PIC32 Family Reference Manual says to disable the SPI
	** controller before writing to the baud register
	*/
	m_pspi->sxCon.clr = (1 << _SPICON_ON);	// disable SPI
	m_pspi->sxBrg.reg = brg;
	m_pspi->sxCon.set = (1 << _SPICON_ON);	// enable SPI
}


/* ------------------------------------------------------------ */
/***	DSPI::setMode
**
**	Parameters:
**		mod		- requested SPI mode.
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the SPI controller to the requested data mode. This
**		should be one of the values:
**			DSPI_MODE0, DSPI_MODE1, DSPI_MODE2, DSPI_MODE3
*/

void WS2801::setMode(uint16_t mode) {
	if ((mode & ~((1 << _SPICON_CKP)|(1 << _SPICON_CKE))) != 0) {
		/* This is an invalid value.
		*/
		return;
	}

	m_pspi->sxCon.clr = (1 << _SPICON_ON);
	m_pspi->sxCon.clr =((1 << _SPICON_CKP)|(1 << _SPICON_CKE));	// force both mode bits to 0

	m_pspi->sxCon.set = mode;		// set the requested mode
	m_pspi->sxCon.set = (1 << _SPICON_ON);
}


/* ------------------------------------------------------------ */
/***	DSPI::setPinSelect
**
**	Parameters:
**		pin		- the pin to use as the slave select
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This sets the pin used for slave select. It will make the
**		pin be an output driving high. This pin will then be use
**		by the setSelect method.
*/

void
WS2801::setPinSelect(uint8_t pin) {
	m_pinSS = pin;

	pinMode(m_pinSS, OUTPUT);
	digitalWrite(m_pinSS, HIGH);
}


void WS2801::initialize(int16_t displayWidth, int16_t displayHeight, int16_t frameWidth, int16_t frameHeight, int16_t numLayers) {
	m_displayWidth = displayWidth;
	m_displayHeight = displayHeight;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_numLayers = numLayers;
	m_frameSize = frameWidth * frameHeight;

	m_frames = new ATKIColor::RGBA[numLayers * m_frameWidth * m_frameHeight];

	begin();
	setSpeed(2000000);
}


// blue t-> green -> red
// layer 0 is background drawn (0 or 255 alpha only)
// layer 1 is premultiplied alpha
void WS2801::render(int16_t frameX, int16_t frameY) {
	uint8_t trash;
	ATKIColor::RGBA *p0 = m_frames;
//	ATKColor::RGBA *p1 = p0 + m_frameWidth * m_frameHeight;

	for (int x = 0; x < m_displayWidth; x += 2) {
		for (int y = 0; y < m_displayHeight; y++) {
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.b;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.g;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.r;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
/*
			if (p0->a) {
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p1->b + ((255 - p1->a) * p0->b >> 8);
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p1->g + ((255 - p1->a) * p0->g >> 8);
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p1->r + ((255 - p1->a) * p0->r >> 8);
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
			} else {
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p0->b;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p0->g;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
				m_pspi->sxBuf.reg = p0->r;
				while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
				trash = m_pspi->sxBuf.reg;
			}
*/
			p0++;
		}
		p0 += m_displayHeight;
		for (int y = 0; y < m_displayHeight; y++) {
			p0--;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.b;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.g;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
			m_pspi->sxBuf.reg = p0->color.r;
			while ((m_pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
			trash = m_pspi->sxBuf.reg;
		}
		p0 += m_displayHeight;
	}

//	delay(1);
	
	memset(m_frames, 0 , m_frameWidth * m_frameHeight * 4 * m_numLayers);
};

ATKIColor::RGBA* WS2801::framebuffer(int16_t layer) {
	return m_frames + layer * m_frameWidth * m_frameHeight;
}

void WS2801::circle(int16_t x, int16_t y, int16_t r, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
	circle(x, y, r, ATKIColor::HSVAtoRGBA(c), thickness, style, layer);
}

void WS2801::circle(int16_t x1, int16_t y1, int16_t r, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
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

void WS2801::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::HSVA c, int16_t thickness, int16_t style, int16_t layer) {
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
void WS2801::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, ATKIColor::RGBA c, int16_t thickness, int16_t style, int16_t layer) {
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

void WS2801::point(int16_t x, int16_t y, ATKIColor::HSVA c, int16_t style, int16_t layer) {
	x %= m_frameWidth;
	if (x < 0) {
		x = x + m_frameWidth - 1;
	}
	if (y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + x * m_frameHeight + y] = ATKIColor::HSVAtoRGBA(c);
	}
}

void WS2801::point(int16_t x, int16_t y, ATKIColor::RGBA c, int16_t style, int16_t layer) {
	x %= m_frameWidth;
	if (x < 0) {
		x = x + m_frameWidth - 1;
	}
	if (y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + x * m_frameHeight + y] = c;
	}
}


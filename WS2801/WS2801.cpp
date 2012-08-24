/*
 * WS2801.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: chow
 *
 *  https://www.gnu.org/licenses/gpl-3.0.html
 */

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


void WS2801::initialize(uint16_t displayWidth, uint16_t displayHeight, uint16_t frameWidth, uint16_t frameHeight, uint8_t numLayers) {
	m_displayWidth = displayWidth;
	m_displayHeight = displayHeight;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_numLayers = numLayers;
	m_frameSize = frameWidth * frameHeight;

	m_frames = new ATKColor::RGBA[numLayers * m_frameWidth * m_frameHeight];

	begin();
	setSpeed(2000000);
}


// blue t-> green -> red
// layer 0 is background drawn (0 or 255 alpha only)
// layer 1 is premultiplied alpha
void WS2801::render(uint16_t frameX, uint16_t frameY) {
	uint8_t trash;
	ATKColor::RGBA *p0 = m_frames;
	ATKColor::RGBA *p1 = p0 + m_frameWidth * m_frameHeight;

	for (int x = 0; x < m_displayWidth; x += 2) {
		for (int y = 0; y < m_displayHeight; y++) {
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
			p0++;
		}
		p0 += m_displayHeight;
		for (int y = 0; y < m_displayHeight; y++) {
			p0--;
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
		p0 += m_displayHeight;
	}

	delay(1);
	
	memset(m_frames, 0 , m_frameWidth * m_frameHeight * 4 * m_numLayers);
};

ATKColor::RGBA* WS2801::framebuffer(uint8_t layer) {
	return m_frames + layer * m_frameWidth * m_frameHeight;
}

void WS2801::circle(int8_t x, int8_t y, int8_t r, ATKColor::HSVA c, int8_t thickness, uint8_t style, uint8_t layer) {
}

void WS2801::circle(int8_t x, int8_t y, int8_t r, ATKColor::RGBA c, int8_t thickness, uint8_t style, uint8_t layer) {
}

void WS2801::line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, ATKColor::HSVA c, int8_t thickness, uint8_t style, uint8_t mode, uint8_t layer) {
}

void WS2801::line(int8_t x1, int8_t y1, int8_t x2, int8_t y2, ATKColor::RGBA c, int8_t thickness, uint8_t style, uint8_t mode, uint8_t layer) {
}

void WS2801::point(int8_t x, int8_t y, ATKColor::HSVA c, uint8_t style, uint8_t layer) {
	if (x >= 0 && x < m_frameWidth && y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + y * m_frameHeight + x] = ATKColor::HSVAtoRGBA(c);
	}
}

void WS2801::point(int8_t x, int8_t y, ATKColor::RGBA c, uint8_t style, uint8_t layer) {
	if (x >= 0 && x < m_frameWidth && y >= 0 && y < m_frameHeight) {
		m_frames[layer * m_frameSize + y * m_frameHeight + x] = c;
	}
}

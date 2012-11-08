#include <WProgram.h>
#include <p32xxxx.h>
#include <p32_defs.h>

// change this to match the dimensions of your project
// render function assumes an even number of rows
// where odd strips are wired left to right
// and even strips wired right to left
#define DISPLAY_WIDTH   38
#define DISPLAY_HEIGHT  8

#define DSPI_MODE0  ((0 << _SPICON_CKP)|(1 << _SPICON_CKE))     // CKP = 0 CKE = 1
#define DSPI_MODE1  ((0 << _SPICON_CKP)|(0 << _SPICON_CKE))     // CKP = 0 CKE = 0
#define DSPI_MODE2  ((1 << _SPICON_CKP)|(1 << _SPICON_CKE))     // CKP = 1 CKE = 1
#define DSPI_MODE3  ((1 << _SPICON_CKP)|(0 << _SPICON_CKE))     // CKP = 1 CKE = 0

#define _DSPI_SPD_DEFAULT   1000000L

// aarrggbb
struct RGBA {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;

    RGBA() { b = 0; g = 0; r = 0; a = 0; }
    RGBA(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) { r = cr; g = cg; b = cb; a = ca; }
};

p32_spi *pspi = (p32_spi *) _SPI2_BASE_ADDRESS;
RGBA *frame = new RGBA[DISPLAY_WIDTH * DISPLAY_HEIGHT];

void setup()
{
    // dont connect anything to digital pin 10 (unless your LED strip has slave select
    DSPI_begin(PIN_DSPI0_SS);
    DSPI_setSpeed(2000000);
}

void loop()
{
    for (uint16_t r = 0; r < 256; r += 8) {
        for (uint16_t g = 0; g < 256; g += 8) {
            for (uint16_t b = 0; b < 256; b += 8) {
                RGBA c = RGBA(r, g, b, 255);    // alpha channel is not used in this example
                for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
                    frame[i] = c;
                }
                DSPI_render();
            }
        }
    }
}

void DSPI_begin(uint8_t pinSS) {
    uint8_t         bTmp;
    uint16_t        brg;

    /* Initialize the pins. The pin directions for SDO, SDI and SCK
    ** are set automatically when the SPI controller is enabled. The
    ** SS pin isn't explicitly used by the SPI controller when in
    ** master mode, so we need to initialize it ourselves.
    */
    DSPI_setPinSelect(pinSS);

    /* Disable and reset the SPI controller.
    */
    pspi->sxCon.reg = 0;

    /* Clear the receive buffer.
    */
    bTmp = pspi->sxBuf.reg;

    /* Set the default baud rate.
    */
    brg = (uint16_t)((F_CPU / (2 * _DSPI_SPD_DEFAULT)) - 1);
    pspi->sxBrg.reg = brg;

    /* Clear the receive overflow bit and receive overflow error flag
    */
    pspi->sxStat.clr = (1 << _SPISTAT_SPIROV);

    /* Enable the SPI controller.
    ** Warning: if the SS pin ever becomes a LOW INPUT then SPI 
    ** automatically switches to Slave, so the data direction of 
    ** the SS pin MUST be kept as OUTPUT.
    */
    pspi->sxCon.reg = 0;
    pspi->sxCon.set = (1 << _SPICON_ON) + (1 << _SPICON_MSTEN) + DSPI_MODE0;
}

void DSPI_setPinSelect(uint8_t pinSS) {
    pinMode(pinSS, OUTPUT);
    digitalWrite(pinSS, HIGH);
}

void DSPI_setSpeed(uint32_t speed) {
    uint16_t    brg;

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
    pspi->sxCon.clr = (1 << _SPICON_ON);    // disable SPI
    pspi->sxBrg.reg = brg;
    pspi->sxCon.set = (1 << _SPICON_ON);    // enable SPI
}

void DSPI_render() {
    uint8_t trash;
    RGBA *p = frame;

    for (int y = 0; y < DISPLAY_HEIGHT; y += 2) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->b;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->g;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->r;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
            p++;
        }
        p += DISPLAY_WIDTH;
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            p--;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->b;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->g;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) { }
            pspi->sxBuf.reg = p->r;
            while ((pspi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) { }
            trash = pspi->sxBuf.reg;
        }
        p += DISPLAY_WIDTH;
    }

    delay(1);
    
    memset(frame, 0 , DISPLAY_WIDTH * DISPLAY_HEIGHT * 4);
};

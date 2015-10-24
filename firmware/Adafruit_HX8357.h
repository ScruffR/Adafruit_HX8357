/***************************************************
  This is our library for the Adafruit HX8357D Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
  ****************************************************/

#ifndef _ADAFRUIT_HX8357_H
#define _ADAFRUIT_HX8357_H

#ifdef SPARK
#include "application.h"
#include "../Adafruit_mfGFX/Adafruit_mfGFX.h"

// ScruffR ToDo: Try DIV8, DIV4 and DIV2 for more speeeeeeed
#define SPI_CLOCK_SETTING SPI_CLOCK_DIV8 // seems a good compromise for Photon too

// direct pin manipulation macros - where speed is required
#if (SYSTEM_VERSION < 0x00040400) // no fast pin functions before 0.4.4
#if defined(STM32F2XX)  // for the Photon and friends
#define pinResetFast(_pin)	               (PIN_MAP[_pin].gpio_peripheral->BSRRH = PIN_MAP[_pin].gpio_pin)
#define pinSetFast(_pin)	                 (PIN_MAP[_pin].gpio_peripheral->BSRRL = PIN_MAP[_pin].gpio_pin)
#elif defined(STM32F10X_MD)  // for the Core
#define pinResetFast(_pin)	               (PIN_MAP[_pin].gpio_peripheral->BRR = PIN_MAP[_pin].gpio_pin)
#define pinSetFast(_pin)	                 (PIN_MAP[_pin].gpio_peripheral->BSRR = PIN_MAP[_pin].gpio_pin)
#endif
#define digitalWriteFast(_pin, _hilo)      (_hilo ? pinSetFast(_pin) : pinResetFast(_pin))
#define pinReadFast(_pin)                  (PIN_MAP[_pin].gpio_peripheral->IDR & PIN_MAP[_pin].gpio_pin ? 0xFF : LOW)
#endif

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define RwReg uint8_t

//#ifndef prog_uchar
//typedef unsigned char prog_uchar;
//#endif
#else
#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_GFX.h>
#include <avr/pgmspace.h>
#endif


#define HX8357D 0xD
#define HX8357B 0xB

#define HX8357_TFTWIDTH  320
#define HX8357_TFTHEIGHT 480

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP 0xB7
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME 0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC


#define HX8357B_SETPOWER 0xD0
#define HX8357B_SETVCOM 0xD1
#define HX8357B_SETPWRNORMAL 0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3   0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9

// Color definitions
#define	HX8357_BLACK   0x0000
#define	HX8357_BLUE    0x001F
#define	HX8357_RED     0xF800
#define	HX8357_GREEN   0x07E0
#define HX8357_CYAN    0x07FF
#define HX8357_MAGENTA 0xF81F
#define HX8357_YELLOW  0xFFE0
#define HX8357_WHITE   0xFFFF

class Adafruit_HX8357 : public Adafruit_GFX {

public:
  Adafruit_HX8357(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST, int8_t _MISO);
  Adafruit_HX8357(int8_t _CS, int8_t _DC, int8_t _RST = -1);

  void begin(uint8_t);
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void pushColor(uint16_t color);
  void fillScreen(uint16_t color);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void setRotation(uint8_t r);
  void invertDisplay(boolean i);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */
  uint8_t readdata(void);
  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);
  /*
  uint16_t readcommand16(uint8_t);
  uint32_t readcommand32(uint8_t);
  void     dummyclock(void);
  */

  void spiwrite(uint8_t);
  void writecommand(uint8_t c);
  void writedata(uint8_t d);
  void commandList(uint8_t *addr);
  uint8_t spiread(void);

private:
  uint8_t tabcolor;

  boolean hwSPI;
#if defined (__AVR__)
  uint8_t mySPCR;
  volatile uint8_t *mosiport, *clkport, *dcport, *rsport, *csport;
  int8_t _cs, _dc, _rst, _mosi, _miso, _sclk;
  uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;
#elif defined (__arm__)
  volatile RwReg *mosiport, *clkport, *dcport, *rsport, *csport;
  uint32_t _cs, _dc, _rst, _mosi, _miso, _sclk;
  uint32_t mosipinmask, clkpinmask, cspinmask, dcpinmask;
#endif

#if defined(SPARK)
  inline void writeFast(uint8_t value);
#endif
};
#endif

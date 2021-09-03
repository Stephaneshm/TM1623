/*
* Project Name: TM1623 
* File: TM1632.h
* Description: TM1632.h header file arduino library for TM1623 module(LED & KEY).
* Author: Stéphane HELAIEM
* Created September 2021
* URL: https://github.com/
*/


#ifndef TM1623_H
#define TM1623_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define TM_ACTIVATE 0x8F // Start up
#define TM_BUTTONS_MODE 0x42 // Buttons mode
#define TM_WRITE_LOC 0x44 // Write to a location
#define TM_WRITE_INC 0x40 // Incremental write
#define TM_SEG_ADR 0xC0  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
//#define TM_LEDS_ADR 0xC1  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
#define TM_BRIGHT_ADR 0x88 // Brightness address
#define TM_BRIGHT_MASK 0x07 // Brightness mask 
#define TM_DEFAULT_BRIGHTNESS 0x02 //can be 0x00 to 0x07 
#define TM_DISPLAY_SIZE 4 //size of display

#define TM_ASCII_OFFSET 47 // Ascii table offset to jump over first missing 32 chars



const  PROGMEM unsigned char SevenSeg[] = {
  0x00, /* (space) */
  0xD7, /* 0 */
  0x14, /* 1 */
  0xCD, /* 2 */
  0x5D, /* 3 */
  0x1E, /* 4 */
  0x5B, /* 5 */
  0xDB, /* 6 */
  0x15, /* 7 */
  0xDF, /* 8 */ 
  0x1F, /* 9 */
};




class TM1623  {

public:
	// Constructor 
	//Parameters 
	// 1. strobe = GPIO STB pin
	// 2. clock = GPIO CLK  pin
	// 3. data = GPIO DIO pin
	TM1623(uint8_t strobe, uint8_t clock, uint8_t data );
	
	// Methods
	
	void displayBegin();	 // Begin method , sets pinmodes , Call in setup
	
	void reset(void);  // Reset / Clear module 
	
	//Sets the brightness level on a scale of brightness = 0 to 7.
	//0 is not turned off, it's just the lowest brightness.
	//If user wishes to change the default brightness at start-up change.
	 //The DEFAULT_BRIGHTNESS define in header file.
	void brightness(uint8_t brightness);

	//Read buttons returns a byte with value of buttons 1-8 b7b6b5b4b3b2b1b0
	// 1 pressed, zero not pressed. 
	//User may have to debounce buttons depending on application.
	//See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
	// for de-bonce example.
	uint8_t readButtons(void);

	// Send Text to Seven segments, passed char array pointer
	// dots are removed from string and dot on preceding digit switched on
	// "abc.def" will be shown as "abcdef" with c decimal point turned on.
	void displayText(const char *text);

	
	//Display an integer and leading zeros optional
	void displayIntNum(unsigned long number, boolean leadingZeros = true);



	// Send seven segment value to seven segment
	//  pass position 0-7 byte of data corresponding to segments (dp)gfedcba
	// i.e 0b01000001 will set g and a on. 
	void display7Seg(uint8_t position, uint8_t value);
	
		// Send ASCII value to seven segment, pass position 0-7 and ASCII value byte
	void displayASCII(uint8_t position, uint8_t ascii);

	
	
private:
		uint8_t _STROBE_IO;
		uint8_t _DATA_IO;
		uint8_t _CLOCK_IO;
        void sendCommand(uint8_t value);
        void sendData(uint8_t  data);
};

#endif

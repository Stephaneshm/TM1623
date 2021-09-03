/*
* Project Name: TM1623 
* File: TM1623.cpp
* Description: source file arduino  library for TM1623 module(LED & KEY). 
* Author: Stéphane HELAIEM
* Created September 2021
* URL: https://github.com/
*/

#include "TM1623.h"


TM1623::TM1623(uint8_t strobe, uint8_t clock, uint8_t data) {
  _STROBE_IO = strobe;
  _DATA_IO = data;
  _CLOCK_IO = clock;
}

void TM1623::displayBegin() {
  pinMode(_STROBE_IO , OUTPUT);
  pinMode(_DATA_IO, OUTPUT);
  pinMode(_CLOCK_IO , OUTPUT);
  sendCommand(TM_ACTIVATE);
  brightness(TM_DEFAULT_BRIGHTNESS);
  reset();
}


void TM1623::sendCommand(uint8_t value)
{
  digitalWrite(_STROBE_IO, LOW);
  sendData(value);
  digitalWrite(_STROBE_IO, HIGH);
}

void TM1623::sendData(uint8_t data)
{
    shiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, data);
}

void TM1623::reset() {
  sendCommand(TM_WRITE_INC); // set auto increment mode
  digitalWrite(_STROBE_IO, LOW);
  sendData(TM_SEG_ADR);  // set starting address to
  for (uint8_t i = 0; i < 16; i++)
  {
    sendData(0x00);
  }
   digitalWrite(_STROBE_IO, HIGH);
}




void TM1623::displayASCII(uint8_t position, uint8_t ascii) {
	if (ascii==32) ascii=47;
  display7Seg(position, pgm_read_byte(&SevenSeg[ascii - TM_ASCII_OFFSET]));
}


void TM1623::displayText(const char *text) {
  char c, pos;
  pos = TM_DISPLAY_SIZE-1;
	  while ((c = (*text++)) && pos >= 0)  {
		  displayASCII(pos--, c);
	  }
}



void TM1623::displayIntNum(unsigned long number, boolean leadingZeros)
{
  char values[TM_DISPLAY_SIZE + 1];
  snprintf(values, TM_DISPLAY_SIZE + 1, leadingZeros ? "%04ld" : "%4ld", number); 
  displayText(values);
}


void TM1623::display7Seg(uint8_t position, uint8_t value) { // call 7-segment
  sendCommand(TM_WRITE_LOC);
  digitalWrite(_STROBE_IO, LOW);
  sendData(TM_SEG_ADR + (position << 1));
  sendData(value);
  digitalWrite(_STROBE_IO, HIGH); 
}

uint8_t TM1623::readButtons()
{
  uint8_t buttons = 0;
  uint8_t v =0;
  
  digitalWrite(_STROBE_IO, LOW);
  sendData(TM_BUTTONS_MODE);
  pinMode(_DATA_IO, INPUT);  

  for (uint8_t i = 0; i < 4; i++)
  {
    
    v = shiftIn(_DATA_IO, _CLOCK_IO, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(_DATA_IO, OUTPUT);
  digitalWrite(_STROBE_IO, HIGH); 
  return buttons;
}


void TM1623::brightness(uint8_t brightness)
{
    uint8_t  value = 0;
    value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
    sendCommand(value);
}


#include <TM1623.h>
#define IR_SMALLD_NEC        //1st: define which protocol to use and then,
#include <IRsmallDecoder.h>  //2nd: include the library;

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 7 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 8  // clock = GPIO connected to clock line of module
#define  DIO_TM 9 // data = GPIO connected to data line of module

#define LED_Green 5
#define LED_Red   4

bool Alarm_Red = LOW;
bool Alarm_Green = LOW;
bool IR_Mode = LOW;
 
IRsmallDecoder irDecoder(2); //3rd: create one decoder object with the correct digital pin;
irSmallD_t irData;           //4th: declare one decoder data structure;
 
unsigned long currentTime=0;
unsigned long previousTime=0;

int buttonState;             // the current reading from the input pin
int lastButtonState = 0 ;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1623 tm(STROBE_TM, CLOCK_TM , DIO_TM);

unsigned long nombre=0;







void setup()
{
  Serial.begin(115200);
  delay(100);
  tm.displayBegin();
  tm.reset();
  pinMode(LED_Green,OUTPUT);
  pinMode(LED_Red,OUTPUT);
  digitalWrite(LED_Green,HIGH);
  digitalWrite(LED_Red,HIGH ); 
// init LED
  digitalWrite(LED_Green,LOW); 
  delay(1000);
  digitalWrite(LED_Green,HIGH); 
  delay(500);
  digitalWrite(LED_Red,LOW); 
  delay(500);
  digitalWrite(LED_Red,HIGH); 
  delay(1000);
//  tm.displayText(" 124");
    tm.displayIntNum(nombre, false); 
  Serial.println("-- TM1623 Exemple 2 with DT2131-ZC04 --");


//tm.displayASCII(0, '0');
//delay(1000);
//tm.displayASCII(0, '1');
//delay(1000);
//tm.displayASCII(0, '2');
//delay(1000);
//tm.displayASCII(0, '3');
//delay(1000);
//tm.displayASCII(0, '4');
//delay(1000);
//tm.displayASCII(0, '5');
//delay(1000);
//tm.displayASCII(0, '6');
//delay(1000);
//tm.displayASCII(0, '7');
//delay(1000);
//tm.displayASCII(0, '8');
//delay(1000);
//tm.displayASCII(0, '9');
//delay(1000);
//tm.displayASCII(0, ' ');
//delay(1000);
//

//  
 // tm.display7Seg(1, 0b11001101); // Displays tous les caracteres meme :
//  delay(1000);
//  
//  tm.display7Seg(1, 0b11111111); // Displays tous les caracteres meme :
//  delay(1000);
//
//  tm.display7Seg(2, 0b11111111); // Displays tous les caracteres meme :
//  delay(1000);
//
//  tm.display7Seg(3, 0b11111111); // Displays tous les caracteres meme :
//  delay(1000);
//  


}

void loop()
{
//
//   tm.display7Seg(2, 0b11011111); // Displays tous les caracteres meme :
//   delay(500);
//   tm.display7Seg(2, 0b11111111); // Displays tous les caracteres meme :
//   delay(500);
// 
  uint8_t buttons = tm.readButtons();
  if (buttons != lastButtonState) {lastDebounceTime = millis();}
  if ((millis() - lastDebounceTime) > debounceDelay) {
    //Serial.println(buttons, HEX);
    if (buttons != buttonState) {
       buttonState = buttons;
       if(buttons==0x08) nombre++;
       if (buttons==0x10) nombre --;
       tm.displayIntNum(nombre, false);
      }
   }
lastButtonState = buttons;
  
    
    
    
    currentTime=millis();
    if((currentTime-previousTime)>500)
      {
        previousTime=currentTime;
//        ledState = !ledState;
        if (Alarm_Red==HIGH) digitalWrite(LED_Red,!digitalRead(LED_Red)); 
        if (Alarm_Green==HIGH) digitalWrite(LED_Green,!digitalRead(LED_Green)); 

        //delay(180);
      }
//
if(irDecoder.dataAvailable(irData)) {    //5th: if the decoder has some new data available,       
    Serial.print(irData.keyHeld,HEX);      //6th: do something with the data.
    Serial.print("\t ");
    Serial.print(irData.addr,HEX); 
    Serial.print("\t ");
    Serial.println(irData.cmd,HEX);  
    if (irData.cmd==0x1C) nombre =555;
    if (irData.cmd==0x15) nombre +=10;
    if (irData.cmd==0x07) nombre -=10;
    if (irData.cmd==0x44) IR_Mode=HIGH;
    if (IR_Mode=HIGH && irData.cmd==0x0C) { Alarm_Red=HIGH;Alarm_Green=LOW;IR_Mode=LOW;}
    if (IR_Mode=HIGH && irData.cmd==0x18) { Alarm_Green=HIGH;Alarm_Red=LOW;IR_Mode=LOW;}
    if (IR_Mode=HIGH && irData.cmd==0x0D) { Alarm_Green=LOW;Alarm_Red=LOW;IR_Mode=LOW;}


    tm.displayIntNum(nombre, false);

  }






 
//    tm.display7Seg(2, 0b11111111); // Displays tous les caracteres meme :

//tm.displayASCII(0, '8');



//  // Test 1  Brightness and reset
//  for (uint8_t brightness = 0; brightness < 8; brightness++)
//  {
//    tm.brightness(brightness);
//    tm.displayText("1");
//    delay(1000);
//  }
//  tm.reset();
//  // restore default brightness
//  tm.brightness(0x02);

}

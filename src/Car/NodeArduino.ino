#include <SoftwareSerial.h>
#include "MotorDriver.h"
SoftwareSerial ArduinoSerial(4, 2); // RX, TX
#include "TM1637.h"
#define CLK A5//connect CLK of the 4-Digit Display to A5 of Arduino and can be changed to other ports    
#define DIO A4//
TM1637 disp(CLK, DIO);
void setup() {
  Serial.begin(9600);
  ArduinoSerial.begin(9600);
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  motordriver.init();
  motordriver.setSpeed(255, MOTORB);
  motordriver.setSpeed(255, MOTORA);
}
void loop() {
  while (ArduinoSerial.available() > 0) {
    float val = ArduinoSerial.parseFloat();
//    String val = "";
    if (ArduinoSerial.read() == '\n') {
      Serial.println(val);
      if (val == 1) {
        motordriver.goForward();
      } else{
        motordriver.goLeft();
      }
      disp.display(val);
    }
  }
}

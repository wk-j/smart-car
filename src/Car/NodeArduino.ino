#include <SoftwareSerial.h>
#include "MotorDriver.h"
SoftwareSerial ArduinoSerial(4, 2); // RX, TX
#include "TM1637.h"
#define CLK A5 //connect CLK of the 4-Digit Display to A5 of Arduino and can be changed to other ports
#define DIO A4 //
TM1637 disp(CLK, DIO);

void setup()
{
    Serial.begin(9600);
    ArduinoSerial.begin(9600);
    disp.set(BRIGHT_TYPICAL); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    disp.init(D4056A);        //D4056A is the type of the module
    motordriver.init();
    motordriver.setSpeed(255, MOTORB);
    motordriver.setSpeed(255, MOTORA);
}
void loop()
{

    int forward = 1;
    int left = 2;
    int right = 3;
    int backward = 4;
    int stop = 5;

    while (ArduinoSerial.available() > 0)
    {
        float val = ArduinoSerial.parseFloat();
        if (ArduinoSerial.read() == '\n')
        {
            Serial.println(val);
            if (val == forward)
            {
                motordriver.goForward();
            }
            else if (val == left)
            {
                motordriver.goLeft();
            }
            else if (val == right)
            {
                motordriver.goRight();
            }
            else if (val == backward)
            {
                motordriver.goBackward();
            }
            else
            {
                motordriver.stop();
            }
            disp.display(val);
        }
    }
}

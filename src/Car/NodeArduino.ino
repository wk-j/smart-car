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

    quiet();
}

long microsecondsToCentimeters(long microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
}

int isHit()
{
    int pingPin = 5;
    int inPin = 3;

    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    pinMode(inPin, INPUT);

    long duration = pulseIn(inPin, HIGH);
    int cm = (int)microsecondsToCentimeters(duration);

    disp.display(cm);

    if (cm < 20)
    {
        return 1;
    }
    return 0;
}

void quiet()
{
    int pingPin = 6;
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, HIGH);
}

void beep()
{
    int pingPin = 6;
    pinMode(pingPin, OUTPUT);

    int count = 2;
    while (count > 0)
    {
        digitalWrite(pingPin, LOW);
        delay(100);

        digitalWrite(pingPin, HIGH);
        delay(100);
        count--;
    }
}

void loop()
{
    int forward = 1;
    int left = 2;
    int right = 3;
    int backward = 4;
    int stop = 5;

    int hit = isHit();

    if (hit)
    {
        motordriver.goBackward();
        beep();
        delay(200);
        motordriver.stop();
        delay(500);

        long r = random(0, 2);

        if (r == 0)
        {
            while (hit)
            {
                motordriver.goLeft();
                delay(500);
                hit = isHit();
            }
        }
        else
        {
            while (hit)
            {
                motordriver.goRight();
                delay(500);
                hit = isHit();
            }
        }
    }
    else
    {
        motordriver.goForward();
    }

    if (ArduinoSerial.available())
    {
        float val = ArduinoSerial.parseFloat();

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
    }
}

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D3, D4); // RX | TX
WiFiServer wifiServer(80);

#define WIFI_SSID "BcircleWIFI"
#define WIFI_PASSWORD "10sep2558"

// #define FIREBASE_HOST "smartcar-e0ddc.firebaseio.com"
// #define FIREBASE_AUTH "C2YLwnFsrIYTByUhRzJspy3fgExna0oSI53lHMc7"

const int pingPin = 5;
const int inPin = 4;

void setup()
{
    Serial.begin(9600);
    NodeSerial.begin(9600);
    Serial.println(WiFi.localIP());
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());

    //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    wifiServer.begin();
}

void loop()
{
    WiFiClient client = wifiServer.available();
    String command = "";
    if (client)
    {
        while (client.connected())
        {
            int val = 0;
            long duration, cm;
            pinMode(pingPin, OUTPUT);
            digitalWrite(pingPin, LOW);
            delayMicroseconds(2);
            digitalWrite(pingPin, HIGH);
            delayMicroseconds(5);
            digitalWrite(pingPin, LOW);
            pinMode(inPin, INPUT);
            duration = pulseIn(inPin, HIGH);
            cm = microsecondsToCentimeters(duration);

            //Firebase.setInt("DATA", cm);
            while (client.available() > 0)
            {
                char c = client.read();
                if (c == '\n')
                {
                    break;
                }
                command = c;
                //        Serial.write(c);
            }

            if (command == "F")
            {
                val = 1;
                //        Serial.println(val);
                NodeSerial.print(val);
                NodeSerial.print("\n");
            }
            else if (command == "L")
            {
                val = 0;
                //        Serial.println(val);
                NodeSerial.print(val);
                NodeSerial.print("\n");
            }

            Serial.println(val);
            // command = "L";
        }

        client.stop();
        Serial.println("Client disconnected");
    }
}
long microsecondsToCentimeters(long microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
}

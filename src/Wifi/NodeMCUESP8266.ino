#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D3, D4); // RX | TX
WiFiServer wifiServer(80);

#define WIFI_SSID "BcircleWIFI"
#define WIFI_PASSWORD "10sep2558"

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

    wifiServer.begin();
}

void loop()
{
    int forward = 1;
    int left = 2;
    int right = 3;
    int backward = 4;
    int stop = 5;

    WiFiClient client = wifiServer.available();
    String command = "";
    if (client)
    {
        while (client.connected())
        {
            while (client.available() > 0)
            {
                char c = client.read();
                if (c == '\n')
                {
                    break;
                }
                command = c;
            }

            if (command == "F")
            {
                NodeSerial.print(forward);
                NodeSerial.print("\n");
            }
            else if (command == "L")
            {
                NodeSerial.print(left);
                NodeSerial.print("\n");
            }
            else if (command == "R")
            {
                NodeSerial.print(right);
                NodeSerial.print("\n");
            }
            else if (command == "B")
            {
                NodeSerial.print(backward);
                NodeSerial.print("\n");
            }
            else
            {
                NodeSerial.print(stop);
                NodeSerial.print("\n");
            }
        }
        client.stop();
    }
}

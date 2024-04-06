#include <ArduinoJson.h>
#include <Http.h>

#define RST_PIN 19
#define RX_PIN 8
#define TX_PIN 7

const char BEARER[] PROGMEM = "internet.beeline.ru";

const int ledPin1 = 10;
const int ledPin2 = 12;
const int ledPin3 = 14;
const int ledPin4 = 16;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Starting!");

    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
}

void loop()
{
    HTTP http(9600, RX_PIN, TX_PIN, RST_PIN);

    char response[256];
    Result result;

    result = http.connect(BEARER);
    Serial.print(F("HTTP connect: "));
    Serial.println(result);

    while (true)
    {
        result = http.get("http://iot.necrosskull.ru/ard", response);
        Serial.print(F("HTTP GET: "));

        String strresp = "[";
        strresp += response;

        Serial.println(strresp);
        if (result == SUCCESS)
        {
            Serial.println(response);
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, strresp);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }
            Serial.println(F("Parsed JSON:"));

            for (size_t i = 0; i < doc.size(); i++)
            {
                JsonObject lamp = doc[i];
                const char *name = lamp["d"];
                bool status = lamp["s"];

                Serial.print(F("Name: "));
                Serial.println(name);
                Serial.print(F("Status: "));
                Serial.println(status);

                if (status)
                {
                    if (strcmp(name, "1") == 0)
                        digitalWrite(ledPin1, HIGH);
                    else if (strcmp(name, "2") == 0)
                        digitalWrite(ledPin2, HIGH);
                    else if (strcmp(name, "3") == 0)
                        digitalWrite(ledPin3, HIGH);
                    else if (strcmp(name, "4") == 0)
                        digitalWrite(ledPin4, HIGH);
                }
                else
                {
                    if (strcmp(name, "1") == 0)
                        digitalWrite(ledPin1, LOW);
                    else if (strcmp(name, "2") == 0)
                        digitalWrite(ledPin2, LOW);
                    else if (strcmp(name, "3") == 0)
                        digitalWrite(ledPin3, LOW);
                    else if (strcmp(name, "4") == 0)
                        digitalWrite(ledPin4, LOW);
                }
            }
        }
    }
}
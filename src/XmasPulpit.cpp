/******************************************************************************
*
* FM Speaker box, aka Xmas Pulpit
*
* This is the firmware that runs on a ESP8266 NodeMCU board to control these
* hardware features:
*
* - A big button with a built-in LED
* - A LED Strip to illuminate the box
* - A FM Receiver
*
* In the idle state, the box flashes the LED in the button and the LED strip is on.
* When the user presses the button, the flashing stops, the lightstrip turns off 
* and the FM receiver turns on.
* This state remains for 8 minutes. For 30s after that, the button LED flashes 
* with increasing frequency, the LED strip gets brighter. If the button is not
* pressed during that minute, the FM receiver turns off, the LED strip turns on, and
* the button LED starts flashing (so back in Idle state).
* If the button is pressed at any time during the 8.5 minutes, the timer starts anew
* at zero.
* The board also serves a webpage that shows the speakers current state and allows
* you to control the sound and the light.

*******************************************************************************/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "serverPage.hpp"
#include "LightSource.hpp"
#include "Pulpit.hpp"
#include "LightSource.hpp"

#define SWITCH_INPUT_PIN D3  // Don't use D8, it won't work.
#define SWITCH_LIGHT_PIN D5
#define RELAY_CONTROL_PIN D6
#define LEDSTRIP_DATA_PIN D7 // MUST define in LightSource.hpp. Only here as a reminder :-)

char *ssid = "<YOUR_SSID>";
const char *password = "<YOUR_PWD>";

AsyncWebServer server(80);
Pulpit *pulpit;

// Replace some parts of the webpage with the status of the speaker box.
String processor( const String& var)
{
    if (var =="STATUS"){
        return pulpit->getState();
    }
    if (var =="SUBSTATUS"){
        return pulpit->getSubState();
    }
    if (var =="STATUS_LOWER"){
        String state=pulpit->getState();
        state.toLowerCase();
        return state;
    }
    return String();
}

void handleRootPath(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", index_html, processor);
}

void handleStatus(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", pulpit->getState()+"|"+pulpit->getSubState());
}

void handleUpdates(AsyncWebServerRequest *request)
{
    String state;
    if (request->hasParam("sound"))
    {
        state = request->getParam("sound")->value();
        if (state == "on")
        {
            pulpit->turnSoundOn();
        }
        else if (state == "off")
        {
            pulpit->turnSoundOff();
        }
    } else     if (request->hasParam("light"))
    {
        state = request->getParam("light")->value();
        int level = state.toInt();
          
        pulpit->setLightBrightness(level);
    }

    request->send(200, "text/plain", "OK");
}

void setup()
{
    Serial.begin(115200);
    Serial.print("Connecting to Wifi. ");
    WiFi.begin(ssid, password);
    Serial.print("Waiting.");

    unsigned long startTime = millis();

	// Attempt to connect to the Wifi for 7.5s
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - startTime) < 7500))
    {
        ESP.wdtFeed();
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
		// We are connected to the Wifi, so configure the web server and start it
        Serial.print(" success at ");
        Serial.println(WiFi.localIP());

        server.on("/", HTTP_GET, handleRootPath);
        server.on("/status", HTTP_GET, handleStatus);
        server.on("/update", handleUpdates);
        server.begin();
    }
    else
    {
        Serial.print(" failed to connect to SSID '");
        Serial.print(ssid);
        Serial.println("'.");
    }
	
	// Create the pulpit class to handle the states
    pulpit = new Pulpit(SWITCH_LIGHT_PIN, SWITCH_INPUT_PIN, RELAY_CONTROL_PIN, LEDSTRIP_DATA_PIN);
    Serial.println("Setup complete."); // print a message out in the serial port
}

void loop()
{
	// Run the pulpit state machine.
    pulpit->process();
}

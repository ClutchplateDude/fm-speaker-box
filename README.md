# FM Speaker box, aka Xmas Pulpit

### Introduction
This is an outdoor speaker box that can be used to allow passersbyt ot turn on sound (e.g. for a Xmas show).
It only requires power (120V), everything else is wireless. It uses a FM receiver to pick up the sound to play.

This repository contains the firmware that runs on a ESP8266 NodeMCU board (probably will run on a ESP32 as well) to control these
hardware features:

- A big button with a built-in LED
- A LED Strip to illuminate the box
- A FM Receiver powered by a relay

### Behaviour
In the idle state, the box flashes the LED in the button and the LED strip is on.
When the user presses the button, the flashing stops, the lightstrip turns off 
and the FM receiver turns on.

This state remains for 8 minutes. For 30s after that, the button LED flashes 
with increasing frequency, the LED strip gets brighter. If the button is not
pressed during that time, the FM receiver turns off, the LED strip turns on, and
the button LED starts flashing (so back in Idle state).

If the button is pressed at any time during the 8.5 minutes, the timer starts anew
at zero.

### External control

The board also serves a webpage that shows the speakers current state and allows
you to control the sound and the light. In order for this to work, your pedestal 
must be within reach of your Wifi network.

### Configuration

The following items can be configured via `#define`s:

- XmasPulpit.hpp
```
#define SWITCH_INPUT_PIN    D3
#define SWITCH_LIGHT_PIN    D5
#define RELAY_CONTROL_PIN   D6

char *ssid = "<YOUR_SSID>";
const char *password = "<YOUR_PWD>";
```
The defines above determine what pins to use to read the button press, flash 
the LED in the button, and turn on the relay for the FM receiver. You also need
to enter the Wifi SSID and password to connect to.

- Pulpit.hpp
```
#define RUNTIME_OF_SOUND       (8L * 60L * 1000L)
#define WARNING_PHASE_OF_SOUND      (30L * 1000L)
```
This determines how long in milliseconds the two phases of operation are. The 
first one is the length of the normal listening phase, the second one starts
flashing things at you to make you extend the time.

- LightSource.hpp
```
#define LEDSTRIP_DATA_PIN   D7
#define NUM_LEDS            72
```
These defines determine what pin to use to send the brightness data via WS2811 and
to how many LEDs to send it to.

- Blinker.hpp
```
#define LIGHT_ON            HIGH
#define LIGHT_OFF           LOW
```
These determine to use HIGH or LOW on the output pin to turn the button LED on.

### Shopping List

I used an old 12V subwoofer computer speaker I had left over, so you may need to source that from somewhere.
Other than that, these are the parts I used for my box, but you can probably find alternatives and
other suppliers.
You may beed some wires and connectors not listed below. Also the switch in the wiring diagram is not needed, I usually just cut power.

| Item| Description| Link|
|--------|---------|------|
| Power plug | Placed this in the back panel to receive the end of an extension cord | [Amazon](https://www.amazon.com/gp/product/B09C7ZQWK3/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)|
| Big Button | This blinks and allows users to press it to start listening | [Experience Lights](https://experiencelights.com/big-led-button#/)
| FM Receiver | Receives the FM signal from your show (or other sound source) | [Amazon](https://www.amazon.com/gp/product/B01N4AHISZ/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)|
| Two Speakers | These are placed in a panel | [Amazon](https://www.amazon.com/gp/product/B07CRXDFF1/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)|
| Speaker Cloth | This covers the panel that contains the two speakers | [Amazon](https://www.amazon.com/gp/product/B09S2YBDGW/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1)|
| 2 x Buck Down Converter | Used to convert the 12V power source to 5V needed for the relay, the receiver, the ESP8266 and the LED strip lights |[Amazon](https://www.amazon.com/gp/product/B08G4GYNL6/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1)|
| Relay | Used to power on the FM receiver and Subwoofer | [Amazon](https://www.amazon.com/gp/product/B095YD3732/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)|
| 5V LED Strip | This is used to light up the front to indicate that the buttons should be pressed. Any WS2811 LED strip will do, but a density of at least 60/m is recommended | [Amazon](https://www.amazon.com/BTF-LIGHTING-300LEDs-Addressable-Flexible-Waterproof/dp/B01CNL6O2Q/ref=sr_1_4?crid=2350GGDJ84OX5&dib=eyJ2IjoiMSJ9.VeiIn1qBcfWMd2G9iX9oF853LS2UrHNHYB5XyAN7ROyyNHTPvmI9udPgpKQ0Ck9e-6A_gxJYVghNQS25Kl2L8GFczbeBvHEkiW_JdJdjZe2naXOEyQnbtWRY0cO6Q_XPxVYlBw3tUd0v6gRH-xqADDLMAvzCaU9HOdacXqaw1XvYBMnHqYUftxy40ZYodSAb_5jTa5gXa0eMvsJ8q2o4xHdET7CP-nMCSFl8EafkFy8DyfmnMPUvEL4SCSqy8y85N-S01_dynZN9hHJhzYmfIX5jQJfhYwRbWGIRDIe5Q4Q.B-IzY_cr2ufvnIpUotEpo3ii8RONOQNt-pAGVVqM1o4&dib_tag=se&keywords=led%2Bstrips%2Bws2811%2B5v%2Bwaterproof%2B60%2Fm&qid=1714363273&sprefix=led%2Bstrips%2Bws2811%2B5v%2Bweaterproof%2B60%2F%2Caps%2C149&sr=8-4&th=1)
| Micro-USB Power cable | This is needed to plug into the FM Receiver to power it | [Amazon](https://www.amazon.com/gp/product/B09TNDWW2Y/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1)
| 12V Power Supply | Connects to 120V and produces 12V | [Amazon](https://www.amazon.com/gp/product/B077PW5JC3/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)|

### Wiring diagram
![Wiring diagram for the pedestal](https://github.com/ClutchplateDude/fm-speaker-box/blob/main/docs/Pedestal%20Wiring.png)

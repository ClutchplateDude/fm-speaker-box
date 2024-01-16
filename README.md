# FM Speaker box, aka Xmas Pulpit

### Introduction
This is the firmware that runs on a ESP8266 NodeMCU board to control these
hardware features:

- A big button with a built-in LED
- A LED Strip to illuminate the box
- A FM Receiver

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
you to control the sound and the light.

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

There is a wiring diagram in the docs folder.


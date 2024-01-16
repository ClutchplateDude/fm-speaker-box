#pragma once

#include <Arduino.h>

#define LIGHT_ON  HIGH
#define LIGHT_OFF LOW

class Blinker
{
  private:
    bool _shouldFlash;
    bool _state;
    bool _pressed;
    bool _buttonPressed;
    int _switchPin;
    int _lightPin;
    long _cycleTime;
    long _lastToggle;
    long _buttonPressedAt;

  public:
    Blinker(int outputPin, int inputPin, float freqHz)
    {
        _switchPin       = inputPin;
        _lightPin        = outputPin;
        _buttonPressed   = false;
        _buttonPressedAt = 0;
        _cycleTime       = 1000.0 / freqHz;
        _state           = LIGHT_OFF;
        _lastToggle      = millis();
        _pressed         = false;
        _shouldFlash     = true;
        pinMode(_switchPin, INPUT_PULLUP);
        pinMode(_lightPin, OUTPUT);
        digitalWrite(_lightPin, _state);
    }

    void run()
    {
        if (_pressed && (digitalRead(_switchPin) == HIGH))
        {
            // if we were pressed and now we're high, unpress
            // // Serial.println("Button released!");
            _pressed = false;
        }
        else if (!_pressed && _buttonPressed && ((millis() - _buttonPressedAt) > 75) && (digitalRead(_switchPin) == LOW))
        {
            // If we were pressed 75ms ago and are still pressed, consider it a button press
            _pressed = true;
            // Serial.println("---- Button pressed! -----");
        }
        if (!_buttonPressed && (digitalRead(_switchPin) == LOW))
        {
            // Serial.println("Button depressed!");
            _buttonPressed   = true;
            _buttonPressedAt = millis();
        }

        if (_shouldFlash)
        {
            long elapsed = millis() - _lastToggle;
            if (elapsed > (_cycleTime / 2))
            {
                _state = _state == LOW ? HIGH : LOW;
                Serial.print("Blinker : Run -> Light -> ");
                Serial.println(_state == LIGHT_ON ? "On" : "Off");
                digitalWrite(_lightPin, _state);
                _lastToggle = millis();
            }
        }
    }

    void lightOff()
    {
        _shouldFlash = false;
        _state       = LIGHT_OFF;
        Serial.println("Blinker : LightOff ");
        digitalWrite(_lightPin, _state);
    }

    void lightOn()
    {
        Serial.println("Blinker : LightOn -> shouldFlash true");
        _shouldFlash = true;
    }

    bool isPressed()
    {
        return _pressed;
    }

    void setFrequency(float freqHz)
    {
        _cycleTime = 1000.0 / freqHz;
    }
};

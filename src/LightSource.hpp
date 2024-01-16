#pragma once

#include <Arduino.h>
#include "FastLED.h"

#define LEDSTRIP_DATA_PIN D7
#define NUM_LEDS 72

class LightSource
{
private:
    float _startLevel;
    float _currentLevel;
    float _targetLevel;
    long _rampTime;
    long _rampStartedAt;
    bool _turningOn;
    bool _active;
    uint8_t _dataPin;
    CRGB _ledData[NUM_LEDS];

public:
    LightSource(int dataPin)
    {
        _active = false;
        _startLevel = 0.0f;
        _targetLevel = 0.0f;
        _currentLevel = 0.0f;
        _rampStartedAt = 0;
        _rampTime = 500;
        _dataPin = dataPin;

        FastLED.addLeds<WS2811, LEDSTRIP_DATA_PIN, EOrder::GRB>(_ledData, NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(128);
    }

    void setBrightness(int targetLevel, long rampTime = 500)
    {
        _targetLevel = 1.0f * targetLevel / 255.0f;
        _active = true;
        _rampStartedAt = millis();
        _rampTime = rampTime;
        Serial.println("Light: brightness " + String(_targetLevel, 2) + " in " + String(_rampTime) + "ms");
    }

    void turnOn(long rampTime = 500)
    {
        _active = true;
        _targetLevel = 1.0f;
        _rampStartedAt = millis();
        _rampTime = rampTime;
        Serial.println("Light: on in " + String(_rampTime) + "ms");
    }

    void turnOff(long rampTime = 500)
    {
        _active = true;
        _targetLevel = 0.0f;
        _rampStartedAt = millis();
        _rampTime = rampTime;
        Serial.println("Light: off in " + String(_rampTime) + "ms");
    }

    void process()
    {
        if (_active)
        {
            long elapsed = millis() - _rampStartedAt;
            if (elapsed < _rampTime)
            {
                float t = 1.0f * elapsed / _rampTime;
                float _currentLevel = _startLevel * (1.0f - t) + _targetLevel * t;
                uint8_t level = static_cast<uint8_t>(floor(255 * _currentLevel));
                Serial.println("Light at " + String(level) + ". Start:" + String(_startLevel, 3) + ". Target: " + String(_targetLevel) + ". Current: " + String(_currentLevel, 3) + ". t:" + String(t, 3));
                for (int i = 0; i < NUM_LEDS; i++)
                {
                    _ledData[i].red = level;
                    _ledData[i].green = level;
                    _ledData[i].blue = level;
                }
            }
            else
            {
                _startLevel = _targetLevel;
                _currentLevel = _targetLevel;
                uint8_t level = static_cast<uint8_t>(floor(255 * _targetLevel));
                Serial.println("Light at " + String(level) + ". Done.");
                for (int i = 0; i < NUM_LEDS; i++)
                {
                    _ledData[i].red = level;
                    _ledData[i].green = level;
                    _ledData[i].blue = level;
                }
                _active = false;
            }
            FastLED.show();
        }
    }

    float getLevel()
    {
        return _currentLevel;
    }
};


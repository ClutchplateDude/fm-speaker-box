#pragma once
#include <Arduino.h>

#include "Blinker.hpp"

// 8 minutes of runtime
#define RUNTIME_OF_SOUND (8L * 60L * 1000L)
#define WARNING_PHASE_OF_SOUND (30L * 1000L)

typedef enum
{
    Starting,
    Idle,
    Listening,
    ListeningEnding,
} PulpitState;

String stateNames[] = {
    "Starting",
    "Idle",
    "Listening",
    "ListeningEnding",
};

class Pulpit
{
private:
    PulpitState _state;
    Blinker blinker;
    LightSource light;
    int _lightPhase;
    long _startedListeningAt;
    long _startedEndingAt;
    int _relayControlPin;

public:
    Pulpit(int switchLightPin, int switchInputPin, int relayControlPin, int ledStripDataPin) : blinker(switchLightPin, switchInputPin, .5f), light(ledStripDataPin)
    {
        _relayControlPin = relayControlPin;
        _state = PulpitState::Starting;
    }

    PulpitState getStateEnum()
    {
        return _state;
    }

    String getState()
    {
        String state = stateNames[_state];
        return state;
    }

    String getSubState()
    {
        String state = "";
        switch (_state)
        {
        case PulpitState::Listening:
        {
            long elapsed = millis() - _startedListeningAt;
            state += "Sound on since " + String(elapsed / 1000L) + "s<br />";
            state += String((RUNTIME_OF_SOUND - elapsed) / 1000L) + "s remain until warning";
        }
        break;
        case PulpitState::ListeningEnding:
        {
            long elapsed = millis() - _startedListeningAt;
            state += "Sound on since " + String(elapsed / 1000L) + "s<br />";
            state += String((RUNTIME_OF_SOUND + WARNING_PHASE_OF_SOUND - elapsed) / 1000L) + "s remaining until sound stops";
        }
        break;
        case PulpitState::Starting:
        case PulpitState::Idle:
            break;
        }
        return state;
    }

    void turnSoundOn()
    {
        Serial.println("Turning Sound On");
        digitalWrite(_relayControlPin, HIGH);
        _state = PulpitState::Listening;
        Serial.println(" Go to Listening State");
        _startedListeningAt = millis();
        blinker.lightOff();
        light.turnOff();
        _lightPhase = 0;
    }

    void setLightBrightness(int level)
    {
        light.setBrightness(level);
    }

    void turnSoundOff()
    {
        blinker.setFrequency(0.5f);
        blinker.lightOn();
        // Turn off FM tuner and speaker
        Serial.println("Turning Sound Off");
        digitalWrite(_relayControlPin, LOW);

        Serial.println(" Go to Idle State");
        _state = PulpitState::Idle;
        _lightPhase = 0;

        light.turnOn();
    }

    void processStartingState()
    {
        Serial.println(" Handle Starting State");
        pinMode(_relayControlPin, OUTPUT);
        _state = PulpitState::Idle;
        Serial.println(" Go to Idle State");
        light.turnOn(200);
    }

    void processIdleState()
    {
        // Low frequency attention grabber
        blinker.run();
        light.process();
        if (blinker.isPressed())
        {
            turnSoundOn();
        }
    }

    void processListeningState()
    {
        blinker.run();
        light.process();
        if ((millis() - _startedListeningAt) > RUNTIME_OF_SOUND)
        {
            _state = PulpitState::ListeningEnding;
            // Serial.println(" Go to ListeningEnding State");
            _startedEndingAt = millis();
            blinker.setFrequency(0.5f);
            blinker.lightOn();
        }
    }

    void processListeningEndingState()
    {
        blinker.run();
        light.process();
        long elapsedSinceEndingStarted = millis() - _startedEndingAt;
        if (blinker.isPressed())
        {
            turnSoundOn();
        }
        else if (elapsedSinceEndingStarted > 1.0000 * WARNING_PHASE_OF_SOUND)
        {
            turnSoundOff();
        }
        else if (elapsedSinceEndingStarted > 0.9000 * WARNING_PHASE_OF_SOUND)
        {
            // Serial.println(" Blink 15Hz");
            blinker.setFrequency(15.0f);
            if (_lightPhase == 3)
            {
                _lightPhase++;
                light.setBrightness(90);
            }
        }
        else if (elapsedSinceEndingStarted > 0.7500 * WARNING_PHASE_OF_SOUND)
        {
            // Serial.println(" Blink 8Hz");
            blinker.setFrequency(8.0f);
            if (_lightPhase == 2)
            {
                _lightPhase++;
                light.setBrightness(48);
            }
        }
        else if (elapsedSinceEndingStarted > 0.5500 * WARNING_PHASE_OF_SOUND)
        {
            // Serial.println(" Blink 4Hz");
            blinker.setFrequency(4.0f);
            if (_lightPhase == 1)
            {
                _lightPhase++;
                light.setBrightness(24);
            }
        }
        else if (elapsedSinceEndingStarted > 0.4000 * WARNING_PHASE_OF_SOUND)
        {
            // Serial.println(" Blink 2Hz");
            blinker.setFrequency(2.0f);
            if (_lightPhase == 0)
            {
                _lightPhase++;
                light.setBrightness(12);
            }
        }
    }

    void process()
    {
        switch (_state)
        {
        case PulpitState::Starting:
            processStartingState();
            break;
        case PulpitState::Idle:
            processIdleState();
            break;
        case PulpitState::Listening:
            processListeningState();
            break;
        case PulpitState::ListeningEnding:
            processListeningEndingState();
            break;
        }
    }
};

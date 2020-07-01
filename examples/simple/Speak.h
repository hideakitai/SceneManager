#pragma once
#ifndef SPEAK_H // change depending on your class
#define SPEAK_H // change depending on your class

#include <SceneManager.h>

class Speak : public Scene::Base
{
public:

    Speak(const String& name, double fps)
    : Base(name, fps)
    {
        Serial.begin(115200);
    }

    virtual ~Speak() {}

    // optional (you can remove this method)
    virtual void begin() override
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.println(" begin()");
    }

    // optional (you can remove this method)
    virtual void enter() override
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.println(" enter()");
    }

    virtual void update() override
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.print(" update() at frame = ");
        Serial.print(frame());
        Serial.print(", time = ");
        Serial.println(millis());
    }
};

#endif // SPEAK_H

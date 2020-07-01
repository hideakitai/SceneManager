#pragma once
#ifndef BLINK_H // change depending on your class
#define BLINK_H // change depending on your class

#include <SceneManager.h>

class Blink : public Scene::Base
{
    bool b;

public:

    Blink(const String& name, double fps)
    : Base(name, fps)
    , b(false)
    {
        pinMode(13, OUTPUT);
        digitalWrite(13, LOW);
    }

    virtual ~Blink() {}

    virtual void update() override
    {
        digitalWrite(13, b);
        b = !b;
    }
};

#endif // BLINK_H

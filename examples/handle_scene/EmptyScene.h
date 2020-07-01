#pragma once
#ifndef ARDUINO_SCENE_MANAGER_SCENE_EMPTY_H // change depending on your class
#define ARDUINO_SCENE_MANAGER_SCENE_EMPTY_H // change depending on your class

#include <SceneManager.h>

class EmptyScene : public Scene::Base
{
public:

    EmptyScene(const String& name, double fps)
    : Base(name, fps)
    {
        Serial.print("Scene ");
        Serial.print(name);
        Serial.print(" has created at fps = ");
        Serial.println(fps);
    }

    virtual ~EmptyScene()
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.println(" will be destructed");
    }

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
        Serial.println(frame());
    }

    // optional (you can remove this method)
    virtual void exit() override
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.println(" exit()");
    }

    // optional (you can remove this method)
    virtual void reset() override
    {
        Serial.print("Scene ");
        Serial.print(getName());
        Serial.println(" reset()");
    }
};

#endif // ARDUINO_SCENE_MANAGER_SCENE_EMPTY_H

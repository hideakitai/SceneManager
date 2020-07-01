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
    }

    virtual ~EmptyScene()
    {
    }

    // optional (you can remove this method)
    // virtual void begin() override
    // {
    // }

    // optional (you can remove this method)
    // virtual void enter() override
    // {
    // }

    virtual void update() override
    {
    }

    // optional (you can remove this method)
    // virtual void exit() override
    // {
    // }

    // optional (you can remove this method)
    // virtual void reset() override
    // {
    // }
};

#endif // ARDUINO_SCENE_MANAGER_SCENE_EMPTY_H

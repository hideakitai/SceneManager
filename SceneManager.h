#pragma once
#ifndef ARDUINO_SCENE_MANAGER_H
#define ARDUINO_SCENE_MANAGER_H

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAM)\
 || defined(ARDUINO_ARCH_SAMD)\
 || defined(ARDUINO_spresense_ast)
    #define ARDUINO_SCENE_MANAGER_DISABLE_STL
#endif

#include <Arduino.h>

#ifdef ARDUINO_SCENE_MANAGER_DISABLE_STL
    #include "SceneManager/util/ArxContainer/ArxContainer.h"
    #include "SceneManager/util/ArxSmartPtr/ArxSmartPtr.h"
#else
    #include "SceneManager/util/TeensyDirtySTLErrorSolution/TeensyDirtySTLErrorSolution.h"
    #include <algorithm>
    #include <iterator>
    #include <vector>
    #include <memory>
#endif // ARDUINO_SCENE_MANAGER_DISABLE_STL

#include "SceneManager/SceneBase.h"


namespace arduino {
namespace scene {

#ifdef ARDUINO_SCENE_MANAGER_DISABLE_STL
    template <typename T> using Vec = arx::vector<T>;
    template <typename T> using Ref = arx::shared_ptr<T>;
    using namespace arx;
#else
    template <typename T> using Vec = std::vector<T>;
    template <typename T> using Ref = std::shared_ptr<T>;
    using namespace std;
#endif // ARDUINO_SCENE_MANAGER_DISABLE_STL

using SceneRef = Ref<Base>;

class Manager
{
    Manager() {}
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    Vec<SceneRef> scenes;

public:

    static Manager& get()
    {
        static Manager m;
        return m;
    }

    template <typename SceneType>
    Ref<SceneType> add(const String& name, const double fps)
    {
        Ref<SceneType> s = make_shared<SceneType>(name, fps);
        scenes.emplace_back(s);
        s->begin();
        return s;
    }

    void start()
    {
        for (auto& s : scenes) s->start();
    }

    bool start(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->start();
            return true;
        }
        return false;
    }

    void update()
    {
        for (auto& s : scenes)
        {
            if (!s->isRunning()) continue;
            if (s->FrameRateCounter::update() || s->hasExit())
            {
                s->callUpdate();
            }
        }
    }

    void stop()
    {
        for (auto& s : scenes) s->stop();
    }

    bool stop(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->stop();
            s->callUpdate();
            return true;
        }
        return false;
    }

    void restart()
    {
        for (auto& s : scenes)
        {
            s->stop();
            s->callUpdate();
            s->start();
        }
    }

    bool restart(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->stop();
            s->callUpdate();
            s->start();
            return true;
        }
        return false;
    }

    void reset()
    {
        for (auto& s : scenes) s->reset();
    }

    bool reset(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->reset();
            return true;
        }
        return false;
    }

    bool empty() const { return scenes.size() == 0; }
    size_t size() const { return scenes.size(); }

    bool isRunning(const String& name) const
    {
        auto s = getSceneByName(name);
        if (s) return s->isRunning();
        return false;
    }

    bool isNext(const String& name) const
    {
        auto s = getSceneByName(name);
        if (s) return s->FrameRateCounter::update();
        return false;
    }

    void erase(const String& name)
    {
#ifdef ARDUINO_SCENE_MANAGER_DISABLE_STL
        auto it = scenes.begin();
        while (it != scenes.end())
        {
            if ((*it)->getName() == name) it = scenes.erase(it);
            else it++;
        }
#else
        auto results = std::remove_if(scenes.begin(), scenes.end(),
            [&](const SceneRef& s) { return (s->getName() == name); }
        );
        scenes.erase(results, scenes.end());
#endif
    }

    double frame(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->frame();
        else   return 0.;
    }

    void setFrameRate(const String& name, float fps)
    {
        auto s = getSceneByName(name);
        if (s) s->setFrameRate(fps);
    }

    template <typename SceneType = Base>
    Ref<SceneType> getSceneByName(const String& name) const
    {
        for (auto& s : scenes)
            if (s->getName() == name)
#ifdef ARDUINO_SCENE_MANAGER_DISABLE_STL
                return (Ref<SceneType>)s;
#else
                return std::static_pointer_cast<SceneType>(s);
#endif
        return nullptr;
    }

    template <typename SceneType = Base>
    Ref<SceneType> operator[] (const String& name) const
    {
        return getSceneByName(name);
    }

    template <typename SceneType = Base>
    Ref<SceneType> operator[] (const size_t i) const
    {
        if (i >= scenes.size()) return nullptr;

#ifdef ARDUINO_SCENE_MANAGER_DISABLE_STL
        return (Ref<SceneType>)scenes[i];
#else
        return std::static_pointer_cast<SceneType>(scenes[i]);
#endif
    }
};

} // scene
} // arduino


#ifndef ARDUINO_SCENE_MANAGER_NAMESPACE_BEGIN
#define ARDUINO_SCENE_MANAGER_NAMESPACE_BEGIN namespace arduino { namespace scene {
#endif // ARDUINO_SCENE_MANAGER_NAMESPACE_BEGIN
#ifndef ARDUINO_SCENE_MANAGER_NAMESPACE_END
#define ARDUINO_SCENE_MANAGER_NAMESPACE_END   }}
#endif // ARDUINO_SCENE_MANAGER_NAMESPACE_END

#define Scenes arduino::scene::Manager::get()
namespace Scene = arduino::scene;

#endif // ARDUINO_SCENE_MANAGER_H

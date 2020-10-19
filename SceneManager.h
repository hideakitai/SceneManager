#pragma once
#ifndef ARDUINO_SCENE_MANAGER_H
#define ARDUINO_SCENE_MANAGER_H

#include <Arduino.h>

#include "SceneManager/util/ArxContainer/ArxContainer.h"
#include "SceneManager/util/ArxSmartPtr/ArxSmartPtr.h"
#include "SceneManager/util/TeensyDirtySTLErrorSolution/TeensyDirtySTLErrorSolution.h"
#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
    #include <algorithm>
    #include <iterator>
#endif

#include "SceneManager/SceneBase.h"


namespace arduino {
namespace scene {

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
    template <typename T> using Vec = std::vector<T>;
    template <typename T> using Ref = std::shared_ptr<T>;
    using namespace std;
#else
    template <typename T> using Vec = arx::vector<T>;
    template <typename T> using Ref = std::shared_ptr<T>;
    using namespace arx;
#endif

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
        Ref<SceneType> s = std::make_shared<SceneType>(name, fps);
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

    bool startFrom(const String& name, const double from_sec)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->startFrom(from_sec);
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

    bool play(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->play();
            return true;
        }
        return false;
    }

    bool pause(const String& name)
    {
        auto s = getSceneByName(name);
        if (s)
        {
            s->pause();
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

    bool isPausing(const String& name) const
    {
        auto s = getSceneByName(name);
        if (s) return s->isPausing();
        return false;
    }

    bool isStopping(const String& name) const
    {
        auto s = getSceneByName(name);
        if (s) return s->isStopping();
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
#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
        auto results = std::remove_if(scenes.begin(), scenes.end(),
            [&](const SceneRef& s) {
                return (s->getName() == name);
            }
        );
        scenes.erase(results, scenes.end());
#else
        auto it = scenes.begin();
        while (it != scenes.end())
        {
            if ((*it)->getName() == name) it = scenes.erase(it);
            else it++;
        }
#endif
    }

    double frame(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->frame();
        else   return 0.;
    }

    int64_t usec64(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->usec64();
        return 0;
    }

    double usec(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->usec();
        return 0.;
    }

    double msec(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->msec();
        return 0.;
    }

    double sec(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->sec();
        return 0.;
    }


    double getDuration(const String& name) const
    {
        auto s = getSceneByName(name);
        if (s) return s->getDuration();
        return 0.;
    }

    double getRemainingTime(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->getRemainingTime();
        return 0.;
    }

    double getRemainingLife(const String& name)
    {
        auto s = getSceneByName(name);
        if (s) return s->getRemainingLife();
        return 0.;
    }


    void setOffset(const String& name, const double sec)
    {
        auto s = getSceneByName(name);
        if (s) s->setOffsetSec(sec);
    }

    void addOffset(const String& name, const double sec)
    {
        auto s = getSceneByName(name);
        if (s) s->addOffsetSec(sec);
    }

    void setTime(const String& name, const double sec)
    {
        auto s = getSceneByName(name);
        if (s) s->setTimeSec(sec);
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
#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
                return std::static_pointer_cast<SceneType>(s);
#else
                return (Ref<SceneType>)s;
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

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
        return std::static_pointer_cast<SceneType>(scenes[i]);
#else
        return (Ref<SceneType>)scenes[i];
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

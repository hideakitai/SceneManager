# SceneManager

cooperative multi-scene manager for Arduino


## Usage

SceneManager can handle more flexible operation than [TaskManager](https://github.com/hideakitai/TaskManager).
TaskManager can only handle functions, but making scene classes can achieve more complex and flexible event handling.
To use SceneManager, first define the scene class, and add it to SceneManager (`Scenes`) with framerate.
Overrided `update()` function will be automatically called if `Scenes.update()` is called and the target frame interval has elapsed.

```C++
#include <SceneManager.h>

// make your scene class
class Blink : public Scene::Base
{
    bool b;

public:

    virtual ~Blink() {}

    Blink(const String& name, double fps)
    : Base(name, fps)
    , b(false)
    {
        pinMode(13, OUTPUT);
        digitalWrite(13, LOW);
    }

    virtual void update() override
    {
        digitalWrite(13, b);
        b = !b;
    }
};


void setup()
{
    Scenes.add<Blink>("blink", 1); // add scene to run in 1 [fps]
    Scenes.start();                // start scenes
}

void loop()
{
    Scenes.update(); // automatically Blink::update() is called in 1 [fps]
}
```


### Scene Class Optional Methods

You can optionally define methods which is called only when specific timing.
See `handle_scene` example for detail.

```C++
class EmptyScene : public Scene::Base
{
public:
    EmptyScene(const String& name, double fps)
    : Base(name, fps) {}
    virtual ~EmptyScene() {}

    virtual void begin() override {}  // optional
    virtual void enter() override {}  // optional
    virtual void update() override {} // must be implemented
    virtual void exit() override {}   // optional
    virtual void reset() override {}  // optional
};
```

## APIs

### SceneManager

```C++
template <typename SceneType>
Ref<SceneType> add(const String& name, const double fps);
void start();
void update();
void stop();
void restart();
void reset();
bool empty() const;
size_t size() const;
bool start(const String& name);
bool stop(const String& name);
bool restart(const String& name);
bool reset(const String& name);
bool isRunning(const String& name) const;
bool isNext(const String& name) const;
void erase(const String& name);
void setFrameRate(const String& name, float fps);
template <typename SceneType>
Ref<SceneType> getSceneByName(const String& name) const;
template <typename SceneType>
Ref<SceneType> operator[] (const String& name) const;
template <typename SceneType>
Ref<SceneType> operator[] (const size_t i) const;
```

### Scene

```C++
virtual void begin();      // optional
virtual void enter();      // optional
virtual void update() = 0; // must be implemented
virtual void exit();       // optional
virtual void reset();      // optional

bool hasEnter() const;
bool hasExit() const;
const String& getName() const;

// defined on FrameRateCounter (derived)
void start();
void stop();
void restart();
bool isRunning() const;
double usec();
double msec();
double sec();
double frame();
void setFrameRate(const double rate);
double getFrameRate() const;
```

## Embedded Libraries

- [PollingTimer v0.2.2](https://github.com/hideakitai/PollingTimer)
- [ArxContainer v0.3.10](https://github.com/hideakitai/ArxContainer)
- [ArxSmartPtr v0.2.1](https://github.com/hideakitai/ArxSmartPtr)
- [TeensyDirtySTLErrorSolution v0.1.0](https://github.com/hideakitai/TeensyDirtySTLErrorSolution)


## License

MIT

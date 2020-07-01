#include <SceneManager.h>
#include "EmptyScene.h"

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("add scenes");
    Serial.println("start scene1");

    Scenes.add<EmptyScene>("scene1", 1)->start();
    Scenes.add<EmptyScene>("scene2", 2);

    // you can also start by:
    // Scenes.start();           // start all scene at once
    // Scenes.start("scene2");   // start "scene2"

    Serial.println("main loop start");
}

void loop()
{
    Scenes.update();

    if (Scenes.frame("scene1") >= 5 && !Scenes.isRunning("scene2"))
    {
        Serial.println("start scene2");
        Scenes.start("scene2");
    }

    if (Scenes.frame("scene1") >= 10)
    {
        Serial.println("stop scene1");
        Scenes.stop("scene1");
    }

    if (Scenes.frame("scene2") >= 15)
    {
        Serial.println("stop scene2");
        Scenes.stop("scene2");
    }

    if (millis() > 16000 && !Scenes.empty())
    {
        Serial.println("erase scene1");
        Scenes.erase("scene1");
        Serial.println("erase scene2");
        Scenes.erase("scene2");
    }
}

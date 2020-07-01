#include <SceneManager.h>
#include "EmptyScene.h"

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Scenes.add<EmptyScene>("empty", 1)->start();
}

void loop()
{
    Scenes.update();
}

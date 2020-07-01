#include <SceneManager.h>
#include "Blink.h"
#include "Speak.h"

void setup()
{
    delay(2000);

    Scenes.add<Blink>("blink", 1);
    Scenes.add<Speak>("speak", 2);

    Scenes.start();
}

void loop()
{
    Scenes.update();
}

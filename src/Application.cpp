#include "DisplayManager.h"
#include "Renderer.h"

int main()
{
    DisplayManager::createDisplay();

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!DisplayManager::shouldCloseWindow())
    {
        renderer.render();

        DisplayManager::updateDisplay();
    }

    renderer.cleanUp();
    DisplayManager::closeDisplay();
    return 0;
}
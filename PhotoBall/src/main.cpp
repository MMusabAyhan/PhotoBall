#include "Application.h"

int WinMain(void)
{
    Application* myApp = new Application();
    myApp->Run();
    delete myApp;

    return 0;
}

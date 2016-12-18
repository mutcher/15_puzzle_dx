#include <Windows.h>
#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    return Application::getSingleton().run(hInstance, nCmdShow);
}
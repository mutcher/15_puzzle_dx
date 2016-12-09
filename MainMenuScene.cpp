#include "MainMenuScene.h"
#include "Application.h"
#include "SceneManager.h"
#include "GameScene.h"

MainMenuScene::MainMenuScene(DxRenderer* renderer)
    :IScene(renderer), _sceneRenderer(renderer), _activeButton(MainMenuButtons::Start)
{
    _sceneRenderer.init();
}

void MainMenuScene::activate()
{
    _sceneRenderer.startup();
}

void MainMenuScene::update()
{
    auto activeButton = static_cast<uint8_t>(_activeButton);
    if (Application::isKeyDown(VK_UP) && activeButton != 0)
    {
        --activeButton;
    }
    else if (Application::isKeyDown(VK_DOWN) && activeButton != 1)
    {
        ++activeButton;
    }
    else if (Application::isKeyDown(VK_RETURN))
    {
        if (_activeButton == MainMenuButtons::Start)
        {
            SceneManager::getSingleton().setActiveScene(new GameScene(_renderer));
        }
        else if (_activeButton == MainMenuButtons::Exit)
        {
            //TODO: find optimal solution to exit from application
            exit(0);
        }
    }
    _activeButton = static_cast<MainMenuButtons>(activeButton);
}

void MainMenuScene::render()
{
    _sceneRenderer.render();
}
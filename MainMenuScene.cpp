#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(DxRenderer* renderer)
    :IScene(renderer), _sceneRenderer(renderer)
{
}

void MainMenuScene::update()
{
}

void MainMenuScene::render()
{
    _sceneRenderer.render();
}
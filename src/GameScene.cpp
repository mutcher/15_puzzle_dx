#include "GameScene.h"
#include "Application.h"
#include "SceneManager.h"
#include "MainMenuScene.h"

GameScene::GameScene(DxRenderer* renderer)
    :IScene(renderer), _stepCount(0), _sceneRender(renderer, &_gameField)
{
    _sceneRender.init();
}

void GameScene::activate()
{
   _sceneRender.startup();
}

void GameScene::update()
{
    _gameField.update();
    auto& app = Application::getSingleton();
    if (!_gameField.isGameCompleted())
    {
        if (app.isKeyDown(VK_UP))
        {
            _gameField.move(GameField::MoveDirection::up);
        }
        else if (app.isKeyDown(VK_DOWN))
        {
            _gameField.move(GameField::MoveDirection::down);
        }
        else if (app.isKeyDown(VK_RIGHT))
        {
            _gameField.move(GameField::MoveDirection::right);
        }
        else if (app.isKeyDown(VK_LEFT))
        {
            _gameField.move(GameField::MoveDirection::left);
        }
        if (_gameField.isFieldChanged())
        {
            ++_stepCount;
        }
    }
    if (app.isKeyDown(VK_ESCAPE))
    {
        SceneManager::getSingleton().setActiveScene(new MainMenuScene(_renderer));
    }
}

void GameScene::render()
{
    _sceneRender.render();
}
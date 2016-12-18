#include "GameScene.h"
#include "Application.h"

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
#ifdef _DEBUG
    if (app.isKeyDown(VK_ESCAPE))
    {
        Application::Shutdown();
    }
#endif
}

void GameScene::render()
{
    _sceneRender.render();
}
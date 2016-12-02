#pragma once
#include "GameField.h"
#include "IScene.h"
#include "GameSceneRenderer.h"

class GameScene final : public IScene
{
private:
    GameField _gameField;
    uint16_t _stepCount;
    GameSceneRenderer _sceneRender;

public:
    GameScene(DxRenderer* renderer);

    virtual void update() override;
    virtual void render() override;
};
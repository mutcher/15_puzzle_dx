#pragma once
#include "IScene.h"
#include "MainMenuSceneRenderer.h"

class MainMenuScene final : public IScene
{
private:
    MainMenuSceneRenderer _sceneRenderer;

public:
    MainMenuScene(DxRenderer* renderer)
        :IScene(renderer), _sceneRenderer(renderer)
    {
    }

    virtual void update() override
    {
    }

    virtual void render() override
    {
        _renderer->beginRender();
        _renderer->endRender();
    }
};

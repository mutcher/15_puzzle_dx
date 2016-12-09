#pragma once
#include "IScene.h"
#include "MainMenuSceneRenderer.h"

class MainMenuScene final : public IScene
{
private:
    MainMenuSceneRenderer _sceneRenderer;

public:
    MainMenuScene(DxRenderer* renderer);

    virtual void update() override;
    virtual void render() override;
};

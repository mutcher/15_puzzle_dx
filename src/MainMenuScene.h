#pragma once
#include "IScene.h"
#include "MainMenuSceneRenderer.h"

class MainMenuScene final : public IScene
{
private:
    MainMenuSceneRenderer _sceneRenderer;
    MainMenuSceneRenderer::MainMenuButtons _activeButton;

public:
    MainMenuScene(DxRenderer* renderer);

    virtual void activate() override;
    virtual void update() override;
    virtual void render() override;
};

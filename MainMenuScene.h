#pragma once
#include "IScene.h"
#include "MainMenuSceneRenderer.h"

class MainMenuScene final : public IScene
{
private:
    enum class MainMenuButtons : uint8_t
    {
        Start = 0,
        Exit = 1
    };

private:
    MainMenuSceneRenderer _sceneRenderer;
    MainMenuButtons _activeButton;

public:
    MainMenuScene(DxRenderer* renderer);

    virtual void activate() override;
    virtual void update() override;
    virtual void render() override;
};

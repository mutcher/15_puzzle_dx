#include "MainMenuSceneRenderer.h"

MainMenuSceneRenderer::MainMenuSceneRenderer(DxRenderer* renderer)
    :_renderer(renderer), _texture()
{
}

void MainMenuSceneRenderer::init()
{
    _texture.load("data/menu.tga", _renderer->getDevice());
}

void MainMenuSceneRenderer::render()
{
    _renderer->beginRender();

    _renderer->endRender();
}
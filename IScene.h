#pragma once
#include "DxRenderer.h"

class IScene
{
protected:
    DxRenderer* _renderer;

public:
    IScene(DxRenderer* renderer)
        :_renderer(renderer)
    {
    }

    virtual void update() = 0;
    virtual void render() = 0;
};
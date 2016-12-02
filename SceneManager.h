#pragma once
#include "IScene.h"

class SceneManager final
{
private:
    IScene* _activeScene;

    SceneManager()
        :_activeScene(nullptr)
    {
    }

public:
    static SceneManager& getSingleton()
    {
        static SceneManager mgr;
        return mgr;
    }

    IScene* getActiveScene()
    {
        return _activeScene;
    }

    void setActiveScene(IScene* scene)
    {
        _activeScene = scene;
    }
};
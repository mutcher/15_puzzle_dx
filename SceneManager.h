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

    ~SceneManager()
    {
        deleteActiveScene();
    }

    void deleteActiveScene()
    {
        if (_activeScene != nullptr)
        {
            delete _activeScene;
            _activeScene = nullptr;
        }
    }

public:
    static SceneManager& getSingleton()
    {
        static SceneManager mgr;
        return mgr;
    }

    IScene* getActiveScene()
    {
        assert(_activeScene != nullptr);
        return _activeScene;
    }

    void setActiveScene(IScene* scene)
    {
        deleteActiveScene();
        _activeScene = scene;
        _activeScene->activate();
    }
};
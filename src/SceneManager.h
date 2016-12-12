#pragma once
#include "IScene.h"

class SceneManager final
{
private:
    IScene* _activeScene;
    IScene* _tmpScene;

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

    void update()
    {
        if (_tmpScene != nullptr)
        {
            deleteActiveScene();
            _activeScene = _tmpScene;
            _tmpScene = nullptr;
            _activeScene->activate();
        }
    }

    IScene* getActiveScene()
    {
        assert(_activeScene != nullptr);
        return _activeScene;
    }

    void setActiveScene(IScene* scene)
    {
        if (_tmpScene != nullptr)
        {
            delete _tmpScene;
            _tmpScene = nullptr;
        }

        _tmpScene = scene;
    }
};
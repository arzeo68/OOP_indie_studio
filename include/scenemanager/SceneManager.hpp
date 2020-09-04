//
// Created by guillaumeg on 31/05/2020.
//

#ifndef INDIE_STUDIO_SCENEMANAGER_HPP
#define INDIE_STUDIO_SCENEMANAGER_HPP

#include <vector>
#include <memory>
#include <irrlicht/ISceneManager.h>
#include <engine/Irrlicht.hpp>
#include <engine/Tracks.hpp>
#include <clock/Clock.hpp>
#include <scenes/IScene.hpp>
#include <scenes/Background.hpp>
#include <utils/Enum.hpp>
#include <map>
#include <map/Map.hpp>
#include <utils/Save.hpp>

class SceneManager
{
    public:

    SceneManager();

    SceneManager(SceneManager &&) = default;

    ~SceneManager() = default;

    void loop() noexcept;

    private:
    std::shared_ptr<Irrlicht> _irr;
    std::shared_ptr<Clock> _clock;
    irr::scene::ICameraSceneNode *_camera;
    Background _background;
    std::shared_ptr<Map> _map;
    Tracks _tracks;
    Save _save;
    std::shared_ptr<IScene> _scene;
    std::map<Enum::Scene, std::shared_ptr<IScene>> _scenes;
    irr::scene::IMesh *plane;

    void update() noexcept;
    void setScene(Enum::Scene scene);
};

#endif //INDIE_STUDIO_SCENEMANAGER_HPP

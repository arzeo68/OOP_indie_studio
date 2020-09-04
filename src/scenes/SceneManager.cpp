//
// Created by guillaumeg on 31/05/2020.
//

#include <thread>
#include <algorithm>
#include <scenemanager/SceneManager.hpp>
#include <scenes/Game.hpp>
#include <scenes/End.hpp>
#include <scenes/Menu.hpp>
#include <scenes/Options.hpp>
#include <scenes/MapPreview.hpp>
#include <scenes/Login.hpp>
#include <scenes/Ladder.hpp>

SceneManager::SceneManager() : _irr(std::make_shared<Irrlicht>(1280, 720)), _clock(std::make_shared<Clock>()), _camera(nullptr),
    _background(_irr, _clock), _map(std::make_shared<Map>(_irr)), _save(_tracks)
{
    plane = _irr->getSceneManager()->getGeometryCreator()->createPlaneMesh({(float)19.2, 10}, {20, 10}, nullptr, {(float)19.2, 10});
    plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    auto table = _irr->createNodeFromMesh(plane);
    table->setMaterialTexture(0, _irr->createTexture("asset/map/parquet.jpg"));
    _scenes.emplace(std::make_pair(Enum::Scene::Menu, std::make_shared<Menu>(_irr, _clock, _camera, _tracks)));
    _scenes.emplace(std::make_pair(Enum::Scene::Ladder, std::make_shared<Ladder>(_irr, _clock, _camera, _tracks, _save.getScores())));
    _scenes.emplace(std::make_pair(Enum::Scene::PlayerSelect, std::make_shared<PlayerSelection>(_irr, _clock, _camera, _tracks)));
    _scenes.emplace(std::make_pair(Enum::Scene::Game, std::make_shared<Game>(_irr, _clock, _camera, _tracks, _map, _save)));
    _scenes.emplace(std::make_pair(Enum::Scene::MapPreview,
        std::make_shared<MapPreview>(_irr, _clock, _camera, _tracks, dynamic_cast<Game *>(_scenes[Enum::Scene::Game].get()))));
    _scenes.emplace(std::make_pair(Enum::Scene::End, std::make_shared<End>(_irr, _clock, _camera, _tracks)));
    _scenes.emplace(std::make_pair(Enum::Scene::Options, std::make_shared<Options>(_irr, _clock, _camera, _tracks, _save)));
    _scenes.emplace(std::make_pair(Enum::Scene::Login, std::make_shared<Login>(_irr, _clock, _camera, _tracks)));
    setScene(Enum::Scene::Login);
    auto menu = _scenes[Enum::Scene::Menu]->getCameraInfo();
    _camera = _irr->addCamera(menu.first, menu.second);
    table->setPosition(_camera->getTarget());
}

void SceneManager::loop() noexcept
{
    while (_irr->getDevice()->run() && _irr->getVDriver()) {
        bool active = _irr->getDevice()->isWindowActive();
        if (active && (1000.0 / 30) > (_clock->getElapsedTimeSinceLastReset() * 1000))
        {
            long x = (long)((1000.0 / 30) - (_clock->getElapsedTimeSinceLastReset() * 1000));
            std::this_thread::sleep_for(std::chrono::milliseconds(x));
        } else if (active)
            update();
        else
            _irr->getDevice()->yield();
    }
}

void SceneManager::update() noexcept
{
    Enum::Scene value;

    _irr->getVDriver()->beginScene(true, true, irr::video::SColor(255, 255, 255, 255));
    _background.update();
    value = _scene->adjustCamera() ? _scene->update() : _scene->getScene();
    _irr->getSceneManager()->drawAll();
    _irr->getGuiEnv()->drawAll();
    _irr->getVDriver()->endScene();
    if (_scenes[value] != _scene)
        setScene(value);
    _clock->reset();
}

void SceneManager::setScene(Enum::Scene scene)
{
    if (_scene)
        _scene->unsetup();
    _scene = _scenes[scene];
    _scene->setup();
}

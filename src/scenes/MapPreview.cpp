//
// Created by guillaumeg on 07/06/2020.
//

#include "scenes/MapPreview.hpp"

MapPreview::MapPreview(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
    Game *game) :
    AMenu(irr, clock, camera, tracks, game->getCameraInfo()), _map(game->getMap())
{
}

Enum::Scene MapPreview::update()
{
    auto id = _irr->getInput()->exportIDButtonPressed();

    if (id == AMenu::ButtonID::ReloadMap)
    {
        _irr->getSceneManager()->clear();
        _camera = _irr->addCamera(_cameraPositions.first, _cameraPositions.second);
        auto plane = _irr->getSceneManager()->getGeometryCreator()->createPlaneMesh({(float)19.2, 10}, {20, 10}, nullptr, {(float)19.2, 10});
        plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        auto table = _irr->createNodeFromMesh(plane);
        table->setPosition({-55, 0, 6});
        table->setMaterialTexture(0, _irr->createTexture("asset/map/parquet.jpg"));
        _map = std::make_shared<Map>(_irr);
    }
    return id == Play ? Enum::Scene::Game : id == AMenu::ButtonID::BackToMenu ? Enum::Scene::PlayerSelect : Enum::Scene::MapPreview;
}

void MapPreview::setup()
{
    _irr->getSceneManager()->clear();
    _camera = _irr->addCamera(_cameraPositions.first, _cameraPositions.second);
    auto plane = _irr->getSceneManager()->getGeometryCreator()->createPlaneMesh({(float)19.2, 10}, {20, 10}, nullptr, {(float)19.2, 10});
    plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    auto table = _irr->createNodeFromMesh(plane);
    table->setPosition({-55, 0, 6});
    table->setMaterialTexture(0, _irr->createTexture("asset/map/parquet.jpg"));
    _map->reset(_irr);
    _map = std::make_shared<Map>(_irr);
    _irr->getFader()->fadeIn(1000);
    _buttons.emplace_back(_irr->createButton({1280 / 2 + 77 / 2 + 5, 720 - 10 - 77, 77, 77}, AMenu::ButtonID::Play, "asset/menu/play.jpg"));
    _buttons.emplace_back(_irr->createButton({1280 / 2 - 77 / 2 - 5, 720 - 10 - 77, 77, 77}, AMenu::ButtonID::ReloadMap, "asset/menu/reload.png"));
}

void MapPreview::unsetup()
{
    AMenu::unsetup();
}

bool MapPreview::adjustCamera()
{
    _camera->setTarget(_cameraPositions.second);
    _camera->setPosition(_cameraPositions.first);
    return true;
}

Enum::Scene MapPreview::getScene()
{
    return Enum::Scene::MapPreview;
}

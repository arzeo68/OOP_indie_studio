//
// Created by alexis on 27/05/2020.
//

#include <thread>
#include <algorithm>
#include <scenes/Game.hpp>
#include <iostream>
#include <engine/Music.hpp>
#include <scenes/Login.hpp>

Game::Game(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
           std::shared_ptr<Map> &map, Save &save
) : AScene(irr, clock, camera, tracks, {}), victory(0), _map(map), _save(save)
{
    _cameraPositions = _map->getCameraPosition();
}

void Game::setup()
{
    _tracks.add<Sound>("asset/soundEffect/mort.wav");
    _tracks.add<Sound>("asset/soundEffect/powerUp.wav");
    _tracks.add<Sound>("asset/soundEffect/explo.wav");
    _tracks.add<Sound>("asset/soundEffect/victoryMusic.wav");

    /********************* SET DECORATIONS ***************************/
    std::string obj[3] = { "asset/Bear.obj", "asset/ABC_Spielwuerfel.obj", "asset/MilitaryTruck.obj"};
    irr::core::vector3d<irr::f32> scales[3] = {{7, 7, 7}, {0.3, 0.3, 0.3}, {1, 1, 1}};
    irr::core::vector3d<irr::f32> rotations[3] = {{0, 120, 0}, {0, 60, 0}, {0, -70, 0}};
    irr::core::vector3d<irr::f32> positions[3] = {
            {_cameraPositions.second.X - 14, _cameraPositions.second.Y + 2, _cameraPositions.second.Z + 6},
            {_cameraPositions.second.X + (float)9.1, _cameraPositions.second.Y + _map->getBlocksize() * 2, _cameraPositions.second.Z},
            {_cameraPositions.second.X + 13, _cameraPositions.second.Y + _map->getBlocksize() * 2, _cameraPositions.second.Z + 5}};

    for (int i = 0; i < 3; i++) {
        auto rnode = _irr->createNodeFromAnimatedMesh(_irr->createAnimatedMesh(obj[i]));
        rnode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        if (!i)
            rnode->setMaterialTexture(0, _irr->createTexture("asset/texbear.png"));
        rnode->addShadowVolumeSceneNode();
        rnode->setScale(scales[i]);
        rnode->setRotation(rotations[i]);
        rnode->setPosition(positions[i]);
    }

    /***************** SET PLAYERS **********************/
    irr::core::vector3d<irr::f32> playersPositions[4] = {
            {0, _map->getBlocksize() / 2, 0},
            {_map->getMapCorner(Map::TopRight).X, _map->getBlocksize() / 2, _map->getMapCorner(Map::TopRight).Z},
            {_map->getMapCorner(Map::BotRight).X, _map->getBlocksize() / 2, _map->getMapCorner(Map::BotRight).Z},
            {_map->getMapCorner(Map::TopLeft).X, _map->getBlocksize() / 2, _map->getMapCorner(Map::TopLeft).Z},
    };
    irr::scene::IAnimatedMesh *mesh = _irr->createAnimatedMesh("asset/Character/Bomberman.MD3");
    for (int i = 0; i < 4; i++)
        _players.push_back(std::make_shared<Player>(mesh, _irr->getPodiumPlacement()[i], _irr,
                                                    !i ? Player::isAzerty + 1 : i == 1 && _irr->isSecondPlayer() ? 3 : 0, playersPositions[i]));
    playerOneTexturePath = _irr->getPodiumPlacement()[0];
    _irr->getPodiumPlacement().clear();
}

Enum::Scene Game::getScene()
{
    return Enum::Scene::Game;
}

Enum::Scene Game::update()
{
    Enum::Scene returnValue = Enum::Scene::Game;
    if (_players.size() > 1)
    {
        auto val = std::remove_if(_players.begin(), _players.end(), [&](const std::shared_ptr<Player> &player)
        {
            if (player->handleEvent(_map, _tracks))
            {
                _tracks.play("asset/soundEffect/mort.wav");
                _irr->getPodiumPlacement().push_back(player->getTexturePath());
                return true;
            }
            if (player->getIsPlayer() && _irr->getInput()->IsKeyDown(player->getKeyMap()[4]) && !player->isPlacing()) {
                std::cout << "try to place bomb" << std::endl;
                player->placeBomb(_map);
                player->setIsPlacing(true);
            } else if (player->getIsPlayer() && !_irr->getInput()->IsKeyDown(player->getKeyMap()[4]) && player->isPlacing())
                player->setIsPlacing(false);
            if (!_map->getBombs().empty() && player->explode(_map, _tracks)) {
                _tracks.play("asset/soundEffect/mort.wav");
                _irr->getPodiumPlacement().push_back(player->getTexturePath());
                return true;
            }
            return false;
        });
        _players.erase(val, _players.end());
    } else {
        _tracks.pause(BACKGROUND_MUSIC);
        _tracks.play("asset/soundEffect/victoryMusic.wav");
        if (_players.size() == 1)
        {
            victory = _players.back()->getTexturePath() == playerOneTexturePath ? 1 : 0;
            _irr->getPodiumPlacement().push_back(_players.back()->getTexturePath());
        }
        returnValue = Enum::Scene::End;
    }
    return (returnValue);
}

void Game::unsetup()
{
    if (!_players.empty())
    {
        _irr->getSceneManager()->addToDeletionQueue(_players.back()->getNode());
        _players.clear();
    }
    _save.addScore(victory);
    _save.save();
}
//
// Created by alexis on 27/05/2020.
//

#ifndef INDIE_STUDIO_GAME_HPP
#define INDIE_STUDIO_GAME_HPP

#include <memory>
#include <scenes/AScene.hpp>
#include <irrlicht/ISceneManager.h>
#include <engine/Irrlicht.hpp>
#include <map/Map.hpp>
#include <entity/IEntity.hpp>
#include <clock/Clock.hpp>
#include <scenemanager/SceneManager.hpp>

class Game : public AScene
{
    public:
    Game(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks,
        std::shared_ptr<Map> &map, Save &_save);

    Game(Game &&) = default;

    Enum::Scene update() override;

    void setup() override;

    void unsetup() override;

    Enum::Scene getScene() override;

    [[nodiscard]] std::shared_ptr<Map> &getMap() const noexcept { return _map; }

private:
    std::shared_ptr<Map> &_map;
    std::vector<std::shared_ptr<Player>> _players;
    int victory = 0;
    std::string playerOneTexturePath;
    Save &_save;
};

#endif //INDIE_STUDIO_GAME_HPP

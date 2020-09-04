//
// Created by guillaumeg on 01/06/2020.
//

#ifndef INDIE_STUDIO_MENU_HPP
#define INDIE_STUDIO_MENU_HPP

#include <map>
#include <string>
#include <irrlicht/ISceneManager.h>
#include <engine/Irrlicht.hpp>
#include <engine/Tracks.hpp>
#include <clock/Clock.hpp>
#include <scenemanager/SceneManager.hpp>
#include "AMenu.hpp"

class Menu : public AMenu
{
public:
    Menu(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks);

    ~Menu() override;

    Menu(Menu &&) = default;

    Enum::Scene update() override;

    Enum::Scene getScene() override;

    void setup() override;

    void unsetup() override;

private:
    irr::s32 _id;
};

#endif //INDIE_STUDIO_MENU_HPP

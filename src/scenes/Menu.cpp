//
// Created by guillaumeg on 01/06/2020.
//

#include <scenes/Menu.hpp>
#include <scenes/Login.hpp>

Menu::Menu(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks) :
    AMenu(irr, clock,camera, tracks), _id(-1)
{
}

Menu::~Menu()
{
    _tracks.stop();
}

void Menu::setup()
{
    auto pos = Irrlicht::getCenter(20, 189);
    _buttons.emplace_back(_irr->createButton({pos.UpperLeftCorner.X, pos.UpperLeftCorner.Y - 120, 189, 80},
         Menu::ButtonID::Play, "asset/menu/play.png"));
    auto pos2 = Irrlicht::getCenter(20, 260);
    _buttons.emplace_back(_irr->createButton({pos2.UpperLeftCorner.X, pos2.UpperLeftCorner.Y + 0, 260, 80},
         Menu::ButtonID::Options, "asset/menu/options.png"));
    auto pos3 = Irrlicht::getCenter(20, 239);
    _buttons.emplace_back(_irr->createButton({pos3.UpperLeftCorner.X, pos3.UpperLeftCorner.Y + 120, 239, 80},
         Menu::ButtonID::Ladder, "asset/menu/ladder.png"));
    _tracks.play(BACKGROUND_MUSIC);
}

Enum::Scene Menu::update()
{
    _id = _irr->getInput()->exportIDButtonPressed();
    return (_id == Menu::ButtonID::Play ? Enum::Scene::PlayerSelect :
        _id == Options ? Enum::Scene::Options : _id == Menu::ButtonID::Ladder ? Enum::Scene::Ladder : Enum::Scene::Menu);
}

void Menu::unsetup()
{
    AMenu::unsetup();
}

Enum::Scene Menu::getScene()
{
    return Enum::Scene::Menu;
}

//
// Created by guillaumeg on 09/06/2020.
//

#include "engine/Music.hpp"
#include "scenes/Login.hpp"
#include "entity/player/Player.hpp"

Login::Login(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks) : AMenu(irr, clock,
    camera, tracks), _textBox(nullptr), _error(nullptr)
{
    _tracks.add<Music>(BACKGROUND_MUSIC);
}

Enum::Scene Login::update()
{
    auto id = _irr->getInput()->exportIDButtonPressed();

    Player::playerName = std::wstring(_textBox->getText());
    if (id == AMenu::ButtonID::Advance && Player::playerName.empty() && !_error)
    {
        auto pos = Irrlicht::getCenter(40, 300);
        _error = _irr->createText(L"Name is empty !", pos.UpperLeftCorner.X + 50, pos.UpperLeftCorner.Y + 75, pos.LowerRightCorner.X + 50,
            pos.LowerRightCorner.Y + 75);
        _error->setOverrideFont(_irr->getLoadedFont());
    }
    return id == AMenu::ButtonID::Advance && !Player::playerName.empty() ? Enum::Scene::Menu : Enum::Scene::Login;
}

void Login::setup()
{
    auto pos = Irrlicht::getCenter(77, 300);
    _textBox = _irr->createTextBox({pos.UpperLeftCorner.X, pos.UpperLeftCorner.Y, 300, 77}, UserName);
    _textBox->setOverrideFont(_irr->getLoadedFont());
    _buttons.emplace_back(
        _irr->createButton({pos.UpperLeftCorner.X + 325, pos.UpperLeftCorner.Y, 77, 77}, AMenu::ButtonID::Advance, "asset/menu/advance.png"));
    _tracks.play(BACKGROUND_MUSIC);
}

void Login::unsetup()
{
    AMenu::unsetup();
}

Enum::Scene Login::getScene()
{
    return Enum::Scene::Login;
}

//
// Created by guillaumeg on 04/06/2020.
//

#include "entity/player/Player.hpp"
#include "scenes/Options.hpp"

Options::Options(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks, Save &save) : AMenu(irr,
    clock, camera, tracks), _textures(), _keyboard(), _save(save)
{
    _textures[0] = _irr->createTexture("asset/options/sound.png");
    _textures[1] = _irr->createTexture("asset/options/music.png");
    _keyboard[0] = _irr->createTexture("asset/options/qwerty.png");
    _keyboard[1] = _irr->createTexture("asset/options/azerty.png");
}

void Options::setup()
{
    _buttons.emplace_back(_irr->createButton({1280 / 2 - (251 / 2), 400, 251, 80}, AMenu::ButtonID::SwitchKeyBoard,
        Player::isAzerty ? "asset/options/azerty.png" : "asset/options/qwerty.png"));
    _buttons.emplace_back(_irr->createButton({10, 720 - 77 - 10, 77, 77}, AMenu::ButtonID::BackToMenu, "asset/menu/return.png"));
    _buttons.emplace_back(_irr->createButton({(1280 / 2) - 231, 100, 77, 77}, AMenu::ButtonID::MinusMusic, "asset/options/minus.png"));
    _buttons.emplace_back(_irr->createButton({(1280 / 2) + 231 - 77, 100, 77, 77}, AMenu::ButtonID::PlusMusic, "asset/options/plus.png"));
    _buttons.emplace_back(_irr->createButton({(1280 / 2) - 231, 250, 77, 77}, AMenu::ButtonID::MinusSound, "asset/options/minus.png"));
    _buttons.emplace_back(_irr->createButton({(1280 / 2) + 231 - 77, 250, 77, 77}, AMenu::ButtonID::PlusSound, "asset/options/plus.png"));
    for (int i = 125; i <= 275; i += 150)
    {
        auto volume = i == 125 ? _tracks.getVolumeMusic() : _tracks.getVolumeSound();
        _texts.emplace_back(_irr->createText(std::to_wstring(volume).c_str(), 1280 / 2 - 25, i, 1280 / 2 + 25, i + 50));
        _texts.back()->setOverrideFont(_irr->getLoadedFont());
    }
}

Enum::Scene Options::update()
{
    auto id = _irr->getInput()->exportIDButtonPressed();
    bool isMinusMusic = id == AMenu::ButtonID::MinusMusic;
    bool isMinusSound = id == AMenu::ButtonID::MinusSound;

    _buttons[0]->setImage(_keyboard[Player::isAzerty]);
    switch (id)
    {
    case PlusMusic:
    case MinusMusic:
        _tracks.setVolumeMusic(_tracks.getVolumeMusic() + (isMinusMusic ? -5 : 5));
        _texts[0]->setText(std::to_wstring(_tracks.getVolumeMusic()).c_str());
        return Enum::Scene::Options;
    case PlusSound:
    case MinusSound:
        _tracks.setVolumeSound(_tracks.getVolumeSound() + (isMinusSound ? -5 : 5));
        _texts[1]->setText(std::to_wstring(_tracks.getVolumeSound()).c_str());
        return Enum::Scene::Options;
    case SwitchKeyBoard:
        Player::isAzerty = !Player::isAzerty;
        _buttons[0]->setImage(_keyboard[Player::isAzerty]);
        return Enum::Scene::Options;
    case BackToMenu:
        return Enum::Scene::Menu;
    default:
        return Enum::Scene::Options;
    }
}

void Options::unsetup()
{
    _save.save();
    AMenu::unsetup();
}

Enum::Scene Options::getScene()
{
    return Enum::Scene::Options;
}

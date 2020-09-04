//
// Created by arzeo on 6/6/2020.
//

#include <entity/player/Player.hpp>
#include <scenes/AMenu.hpp>
#include "scenes/PlayerSelection.hpp"

PlayerSelection::PlayerSelection(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks)
        : AMenu(irr, clock, camera, tracks)
{
    _cameraPositions.first.Y = 10.0;
    _cameraPositions.first.Z = -17.0;
}

Enum::Scene PlayerSelection::update()
{
    irr::s32 idButtonPressed = _irr->getInput()->exportIDButtonPressed();

    secondPlayerJoin(idButtonPressed);
    handleSwitchColorButton(idButtonPressed);
    return (idButtonPressed == AMenu::ButtonID::BackToMenu ? Enum::Scene::Menu :
        idButtonPressed == AMenu::ButtonID::Play ? Enum::Scene::MapPreview : Enum::Scene::PlayerSelect);
}

void PlayerSelection::setup()
{
    irr::scene::IAnimatedMesh *mesh = _irr->createAnimatedMesh("asset/Character/Bomberman.MD3");
    irr::core::vector3df pos = _camera->getTarget();

    pos.X -= 20;
    for (int i = 0; i < 4; ++i) {
        _nodeVec.emplace_back(createNode(mesh, colorPath[i]));
        _nodeVec.back()->setPosition(pos);
        if (i < 2)
            addColorSelectionButton(!i, i);
        pos.X += 13;
    }
    join_leave_button = _irr->createButton({1280 / 2 - 207 - 40, 720 / 2 + 50, 207, 80}, AMenu::ButtonID::Join, "asset/menu/join.png");
    _buttons.push_back(join_leave_button);
    _buttons.emplace_back(_irr->createButton({10, 720 - 77 - 10, 77, 77}, AMenu::ButtonID::BackToMenu, "asset/menu/return.png"));
    _buttons.emplace_back(_irr->createButton({1280 / 2 - 425 / 2, 720 - 10 - 77, 425, 77}, AMenu::ButtonID::Play, "asset/menu/play.png"));
}

void PlayerSelection::unsetup()
{
    _irr->setSecondPlayer(_player[1] == 1);
    for (int i = 0; i < 4; ++i) {
        _irr->getPodiumPlacement().push_back(colorPath[_colorUsed[i]]);
        _nodeVec[i]->remove();
    }
    _nodeVec.clear();
    joinIsPressed = false;
    _player = {1, 0, 0, 0};
    AMenu::unsetup();
    _irr->setFader(_irr->getGuiEnv()->addInOutFader());
}

irr::scene::IAnimatedMeshSceneNode *PlayerSelection::createNode(irr::scene::IAnimatedMesh *mesh, const std::string &texturePath)
{
    irr::scene::IAnimatedMeshSceneNode *node = _irr->createNodeFromAnimatedMesh(mesh);
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMaterialTexture(0, _irr->createTexture(texturePath));
    node->setFrameLoop(playerState::STAND.first, playerState::STAND.second);
    node->setScale({3, 3, 3});
    return node;
}

void PlayerSelection::addColorSelectionButton(bool visible, int idx)
{
    int id = !idx ? leftFirst : idx == 1 ? leftSecond : 0;
    int posX = 181 + ((275) * idx);
    std::array<int, 4> pos[2] = {
            {posX - 40, 720 / 2 - 15 - 40, 40, 40},
            {posX + 130 - 40, 720 / 2 - 15 - 40, 40, 40}
    };

    for (int i = 0; i < 2; i++) {
        _buttons.emplace_back(_irr->createButton(pos[i], id + i, "asset/menu/" + std::string(!i ? "return.png" : "advance.png")));
        _buttons.back()->setScaleImage(true);
        _buttons.back()->setVisible(visible);
    }
}

void PlayerSelection::secondPlayerJoin(irr::s32 buttonID)
{
    if (buttonID == AMenu::ButtonID::Join) {
        _player[1] = !joinIsPressed;
        join_leave_button->setImage(_irr->createTexture(!joinIsPressed ? "asset/menu/leave.png" : "asset/menu/join.png"));
        joinIsPressed = !joinIsPressed;
    }

    for (auto &_button : _buttons)
        if (_button->getID() == leftSecond || _button->getID() == rightSecond) {
            _button->setVisible(_player[1] == 1);
            _button->setEnabled(_player[1] == 1);
        }
}

void PlayerSelection::switchColor(int idx, int diff)
{
    _colorUsed[idx] += diff;
    _colorUsed[idx] = _colorUsed[idx] > 3 ? 0 : _colorUsed[idx] < 0 ? 3 : _colorUsed[idx];
    _nodeVec[idx]->setMaterialTexture(0, _irr->createTexture(colorPath[_colorUsed[idx]]));
    if (_colorUsed[1] == _colorUsed[0] && _player[1] == 1)
        return switchColor(idx, diff);
    adjustBotColors();
}

void PlayerSelection::handleSwitchColorButton(irr::s32 buttonID)
{
    if (buttonID == leftFirst || buttonID == leftSecond)
        switchColor(buttonID == leftSecond, -1);
    else if (buttonID == rightFirst || buttonID == rightSecond)
        switchColor(buttonID == rightSecond, 1);
}

void PlayerSelection::adjustBotColors()
{
    int limit = _player[1] == 1 ? 2 : 1;
    std::vector<int> used;

    for (int i = 0; i < limit; i++)
        used.emplace_back(_colorUsed[i]);
    for (int i = limit; i < 4; i++) {
        if (std::find(used.begin(), used.end(), _colorUsed[i]) != used.end()) {
            _colorUsed[i]++;
            _colorUsed[i] = _colorUsed[i] > 3 ? 0 : _colorUsed[i] < 0 ? 3 : _colorUsed[i];
            i--;
            continue;
        }
        used.push_back(_colorUsed[i]);
    }
    for (int i = limit; i < 4; i++)
        _nodeVec[i]->setMaterialTexture(0, _irr->createTexture(colorPath[_colorUsed[i]]));
}

Enum::Scene PlayerSelection::getScene()
{
    return Enum::Scene::PlayerSelect;
}

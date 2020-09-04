//
// Created by arzeo on 6/6/2020.
//em

#ifndef INDIE_STUDIO_PLAYERSELECTION_HPP
#define INDIE_STUDIO_PLAYERSELECTION_HPP

#include <scenes/AMenu.hpp>
#include <array>

const static std::vector<std::string> colorPath = {
        "asset/Character/RedBombermanTextures.png",
        "asset/Character/BlackBombermanTextures.png",
        "asset/Character/WhiteBombermanTextures.png",
        "asset/Character/PinkBombermanTextures.png"
};

class PlayerSelection : public AMenu {
public:
    PlayerSelection(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks);

    Enum::Scene update() override;

    void setup() override;

    Enum::Scene getScene() override;

    void unsetup() override;

private:
    enum colorSelection
    {
        leftFirst = 50,
        rightFirst = 51,
        leftSecond = 52,
        rightSecond = 53
    };
    bool joinIsPressed = false;
    irr::gui::IGUIButton *join_leave_button = nullptr;
    std::array<int, 4> _colorUsed = {0, 1, 2, 3};
    std::array<int, 4> _player = {1, 0, 0, 0};
    std::vector<irr::scene::IAnimatedMeshSceneNode *> _nodeVec;

    void adjustBotColors();
    void handleSwitchColorButton(irr::s32 buttonID);
    void switchColor(int idx, int diff);
    void secondPlayerJoin(irr::s32 buttonID);
    void addColorSelectionButton(bool visible, int idx);
    irr::scene::IAnimatedMeshSceneNode *createNode(irr::scene::IAnimatedMesh *mesh, const std::string &texturePath);
};

#endif //INDIE_STUDIO_PLAYERSELECTION_HPP

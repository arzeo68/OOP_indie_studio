//
// Created by Laurent on 6/1/2020.
//

#ifndef INDIE_STUDIO_END_HPP
#define INDIE_STUDIO_END_HPP

#include <array>
#include <entity/block/Block.hpp>
#include <memory>
#include <engine/Irrlicht.hpp>
#include <scenemanager/SceneManager.hpp>
#include "AScene.hpp"
#include <scenes/PlayerSelection.hpp>

class End : public AMenu
{
    public:
    End(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks);

    End(End &&) = default;

    Enum::Scene getScene() override;

    ~End() override = default;

    Enum::Scene update() override;

    void setup() override;

    void unsetup() override;

    private:
    irr::scene::IAnimatedMeshSceneNode *createNode(std::shared_ptr<Irrlicht> &_irr, irr::scene::IAnimatedMesh *mesh, const std::string& texturePath
    );

    void placePlayerOnPodium();

    std::array<irr::scene::IAnimatedMeshSceneNode *, 4> _nodeVec;
    std::vector<std::shared_ptr<Block>> _podiumBlock;
    bool victorySoundFinished = false;

    void drawPodium(const irr::core::vector3df& position, std::shared_ptr<Irrlicht> &irr
    );
};

#endif //INDIE_STUDIO_END_HPP

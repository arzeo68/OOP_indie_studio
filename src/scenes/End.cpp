//
// Created by Laurent on 6/1/2020.
//

#include <scenes/End.hpp>

End::End(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks) :
    AMenu(irr, clock,camera, tracks,
            std::make_pair<irr::core::vector3df, irr::core::vector3df>({(float)8.4, (float)19.2 + 14, (float)-7.8 - 6},
            {(float)8.4, (float)0 + 20, (float)6})),
    _nodeVec({0, 0, 0, 0})
{
}

void End::setup()
{
    _buttons.emplace_back(_irr->createButton({10, 720 - 77 - 10, 77, 77}, AMenu::ButtonID::BackToMenu, "asset/menu/return.png"));
    drawPodium(_cameraPositions.second, _irr);
    placePlayerOnPodium();
}

Enum::Scene End::update()
{
    if (!victorySoundFinished && _tracks.getStatus("asset/soundEffect/victoryMusic.wav") == sf::SoundSource::Status::Stopped) {
            _tracks.play("asset/menu/bomberman.ogg");
            victorySoundFinished = true;
    }

    irr::s32 idButtonPressed = _irr->getInput()->exportIDButtonPressed();
    return idButtonPressed == AMenu::ButtonID::BackToMenu ? Enum::Scene::Menu : Enum::Scene::End;
}

void End::drawPodium(const irr::core::vector3df& position, std::shared_ptr<Irrlicht> &irr)
{

    irr::scene::IMeshSceneNode *fistPlace = irr->getSceneManager()->addCubeSceneNode(3, nullptr, -1,
        {position.X, position.Y + (float)0.75, position.Z});
    fistPlace->setScale({1, 1.5, 1});
    _podiumBlock.emplace_back(std::make_shared<Block>(nullptr, fistPlace, irr->createTexture("asset/map/block_unbreakable.png"), false));

    irr::scene::IMeshSceneNode *secondPlace = irr->getSceneManager()->addCubeSceneNode(3, nullptr, -1, {position.X - 3, position.Y, position.Z});
    _podiumBlock.emplace_back(std::make_shared<Block>(nullptr, secondPlace, irr->createTexture("asset/map/block_unbreakable.png"), false));

    irr::scene::IMeshSceneNode *thirdPlace = irr->getSceneManager()->addCubeSceneNode(3, nullptr, -1,
        {position.X + 3, position.Y - (float)1.5 + (float)0.75, position.Z});
    thirdPlace->setScale({1, 0.5, 1});
    _podiumBlock.emplace_back(std::make_shared<Block>(nullptr, thirdPlace, irr->createTexture("asset/map/block_unbreakable.png"), false));
}

void End::placePlayerOnPodium()
{
    irr::scene::IAnimatedMesh *mesh = _irr->createAnimatedMesh("asset/Character/Bomberman.MD3");
    for (int i = 0; i < 4; ++i) {
        _nodeVec[i] = (createNode(_irr, mesh, _irr->getPodiumPlacement()[i]));
    }
    _nodeVec[0]->setPosition({_cameraPositions.second.X + 6, _cameraPositions.second.Y - (float)1.5, _cameraPositions.second.Z});
    _nodeVec[1]->setPosition({_cameraPositions.second.X + 3, _cameraPositions.second.Y, _cameraPositions.second.Z});
    _nodeVec[2]->setPosition({_cameraPositions.second.X - 3, _cameraPositions.second.Y + (float)1.5, _cameraPositions.second.Z});
    _nodeVec[3]->setPosition({_cameraPositions.second.X, _cameraPositions.second.Y + (float)3, _cameraPositions.second.Z});
}

irr::scene::IAnimatedMeshSceneNode *End::createNode(std::shared_ptr<Irrlicht> &_irr, irr::scene::IAnimatedMesh *mesh,
        const std::string& texturePath)
{
    irr::scene::IAnimatedMeshSceneNode *node = _irr->createNodeFromAnimatedMesh(mesh);
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMaterialTexture(0, _irr->createTexture(texturePath));
    node->setFrameLoop(playerState::STAND.first, playerState::STAND.second);
    node->setScale({2, 2, 2});
    return node;
}

void End::unsetup()
{
    _irr->getPodiumPlacement().clear();
    _irr->getSceneManager()->clear();
    _camera = _irr->addCamera(_cameraPositions.first, _cameraPositions.second);
    auto plane = _irr->getSceneManager()->getGeometryCreator()->createPlaneMesh({(float)19.2, 10}, {20, 10}, nullptr, {(float)19.2, 10});
    plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    auto table = _irr->createNodeFromMesh(plane);
    table->setPosition({-55, 0, 6});
    table->setMaterialTexture(0, _irr->createTexture("asset/map/parquet.jpg"));
    AMenu::unsetup();
}

Enum::Scene End::getScene()
{
    return Enum::Scene::End;
}

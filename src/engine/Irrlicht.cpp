//
// Created by seres on 04/05/2020.
//

#include <engine/Irrlicht.hpp>
#include <utils/IrrlichtException.hpp>
#include <iostream>
#include <array>

Irrlicht::Irrlicht(int width, int heigth) : _input(new UserInput), _driverType(irr::video::EDT_OPENGL),
    _device(irr::createDevice(_driverType, irr::core::dimension2d<irr::u32>(width, heigth), 32, false, true, false, _input.get())),
    _vDriver(_device->getVideoDriver()), _sManager(_device->getSceneManager()), _guiEnv(_device->getGUIEnvironment()), _fader(nullptr),
    _font(_guiEnv->getFont("asset/font/test.png"))
{
    setWindowTitle(L"BomberMan");
}

void Irrlicht::setWindowTitle(const wchar_t *title)
{
    _device->setWindowCaption(title);
}

irr::gui::IGUIStaticText *Irrlicht::createText(const wchar_t *text, irr::s32 x1, irr::s32 y1, irr::s32 x2, irr::s32 y2)
{
    return _guiEnv->addStaticText(text, irr::core::rect<irr::s32>(x1, y1, x2, y2));
}

irr::scene::IMeshSceneNode *Irrlicht::createNodeFromMesh(irr::scene::IMesh *mesh)
{
    return _sManager->addMeshSceneNode(mesh);
}

irr::scene::IAnimatedMeshSceneNode *Irrlicht::createNodeFromAnimatedMesh(irr::scene::IAnimatedMesh *mesh)
{
    return (_sManager->addAnimatedMeshSceneNode(mesh));
}

irr::scene::IMesh *Irrlicht::createMesh(const std::string &filePath)
{
    auto mesh = _sManager->getMesh(filePath.c_str());
    if (!mesh)
    {
        _device->drop();
        throw IrrlichtException("Cannot find mesh at " + filePath);
    }
    return mesh;
}

irr::scene::IAnimatedMesh *Irrlicht::createAnimatedMesh(const std::string &filePath)
{
    auto mesh = _sManager->getMesh(filePath.c_str());
    if (!mesh)
    {
        _device->drop();
        throw IrrlichtException("Cannot find animated mesh at " + filePath);
    }
    return mesh;
}

irr::video::ITexture *Irrlicht::createTexture(const std::string &texturePath)
{
    return _vDriver->getTexture(texturePath.c_str());
}

irr::scene::ICameraSceneNode *Irrlicht::addCamera(const irr::core::vector3df &placement, const irr::core::vector3df &lookat)
{
    return _sManager->addCameraSceneNode(nullptr, placement, lookat);
}

irr::scene::ISceneManager *Irrlicht::getSceneManager()
{
    return _sManager;
}

const std::unique_ptr<irr::IrrlichtDevice, IrrlichtDeleter<irr::IrrlichtDevice>> &Irrlicht::getDevice() const
{
    return _device;
}

const std::unique_ptr<irr::gui::IGUIEnvironment, nop> &Irrlicht::getGuiEnv() const
{
    return _guiEnv;
}

const std::unique_ptr<irr::video::IVideoDriver, nop> &Irrlicht::getVDriver() const
{
    return _vDriver;
}

const std::unique_ptr<UserInput, nop> &Irrlicht::getInput() const
{
    return _input;
}

Irrlicht::~Irrlicht()
{
    delete _input.get();
}

irr::gui::IGUIButton *Irrlicht::createButton(std::array<int, 4> arr, irr::s32 id, const std::string &imagePath)
{
    irr::core::rect<irr::s32> rec(arr[0], arr[1], arr[0] + arr[2], arr[1] + arr[3]);
    auto button = _guiEnv->addButton(rec, nullptr, id);
    button->setDrawBorder(false);
    button->setUseAlphaChannel(true);
    button->setImage(createTexture(imagePath));
    return button;
}

irr::gui::IGUIEditBox *Irrlicht::createTextBox(std::array<int, 4> arr, irr::s32 id)
{
    irr::core::rect<irr::s32> rec(arr[0], arr[1], arr[0] + arr[2], arr[1] + arr[3]);

    return _guiEnv->addEditBox(L"", rec, true, nullptr, id);
}

std::vector<std::string> &Irrlicht::getPodiumPlacement()
{
    return podiumPlacement;
}

bool Irrlicht::isSecondPlayer() const
{
    return _secondPlayer;
}

void Irrlicht::setSecondPlayer(bool secondPlayer)
{
    _secondPlayer = secondPlayer;
}

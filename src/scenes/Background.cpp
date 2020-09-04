//
// Created by guillaumeg on 02/06/2020.
//

#include <scenes/Background.hpp>

Background::Background(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock) : _irr(irr), _clock(clock),
    _texture(_irr->createTexture("asset/back-ciel.jpg"))
{
}

void Background::update()
{
    _irr->getVDriver()->draw2DImage(_texture, {0, 0, 1280, 720}, {0, 0, 500, 500});
}

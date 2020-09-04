//
// Created by Laurent on 6/4/2020.
//

#include <utils/IrrlichtException.hpp>
#include "engine/Music.hpp"

Music::Music(const std::string &path) : ASound(path, SoundType::Music)
{
    if (!_music.openFromFile(_path))
        throw IrrlichtException("Couldn't load music: " + _path);
    _music.setLoop(true);
    _music.getStatus();
}

void Music::setVolume(int volume) noexcept
{
    _music.setVolume((float)volume);
}

void Music::play()
{
    if (_music.getStatus() != sf::SoundSource::Playing)
        _music.play();
}

void Music::pause()
{
    _music.pause();
}

void Music::stop()
{
    _music.stop();
}

sf::SoundSource::Status Music::getStatus()
{
    return _music.getStatus();
}

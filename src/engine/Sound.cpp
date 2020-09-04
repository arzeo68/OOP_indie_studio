//
// Created by guillaumeg on 04/06/2020.
//

#include <utils/IrrlichtException.hpp>
#include <engine/Sound.hpp>

Sound::Sound(const std::string &path) : ASound(path, SoundType::Sound)
{
    if (!_buffer.loadFromFile(_path))
        throw IrrlichtException("Couldn't load music: " + _path);
    _sound.setBuffer(_buffer);
    _sound.setLoop(false);
    _sound.getStatus();
}

void Sound::play() noexcept
{
    if (_sound.getStatus() != sf::SoundSource::Playing)
        _sound.play();
}

void Sound::pause() noexcept
{
    _sound.pause();
}

void Sound::stop() noexcept
{
    _sound.stop();
}

void Sound::setVolume(int volume) noexcept
{
    _sound.setVolume((float)volume);
}

sf::SoundSource::Status Sound::getStatus()
{
    return _sound.getStatus();
}

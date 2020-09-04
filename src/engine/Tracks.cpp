//
// Created by guillaumeg on 04/06/2020.
//

#include <engine/ASound.hpp>
#include <engine/Tracks.hpp>

Tracks::Tracks()
{
}

void Tracks::remove(const std::string &path)
{
    _noises.erase(path);
}

void Tracks::play()
{
    for (auto &pair : _noises)
    {
        pair.second->setVolume(pair.second->getType() == ASound::Music ? _volumeMusic : _volumeSound);
        pair.second->play();
    }
}

void Tracks::play(const std::string &path)
{
    _noises[path]->setVolume(_noises[path]->getType() == ASound::Music ? _volumeMusic : _volumeSound);
    _noises[path]->play();
}

void Tracks::pause()
{
    for (auto &pair : _noises)
        pair.second->pause();
}

void Tracks::pause(const std::string &path)
{
    _noises[path]->pause();
}

void Tracks::stop()
{
    for (auto &pair : _noises)
        pair.second->stop();
}

void Tracks::stop(const std::string &path)
{
    _noises[path]->stop();
}

int Tracks::getVolumeMusic() const noexcept
{
    return _volumeMusic;
}

int Tracks::getVolumeSound() const noexcept
{
    return _volumeSound;
}

void Tracks::setVolumeMusic(int volume) noexcept
{
    if (volume < 0 || volume > 100)
        return;
    _volumeMusic = volume;
    for (auto &pair : _noises)
        if (pair.second->getType() == ASound::Music)
            pair.second->setVolume(volume);
}

void Tracks::setVolumeSound(int volume) noexcept
{
    if (volume < 0 || volume > 100)
        return;
    _volumeSound = volume;
    for (auto &pair : _noises)
        if (pair.second->getType() == ASound::Sound)
            pair.second->setVolume(volume);
}

sf::SoundSource::Status Tracks::getStatus(const std::string& path)
{
    return _noises[path]->getStatus();
}

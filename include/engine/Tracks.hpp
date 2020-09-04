//
// Created by guillaumeg on 04/06/2020.
//

#ifndef INDIE_STUDIO_TRACKS_HPP
#define INDIE_STUDIO_TRACKS_HPP

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "Sound.hpp"

class Tracks
{
    public:
    Tracks();

    ~Tracks() = default;

    template<typename T>
    void add(const std::string &path)
    {
        _noises.insert(std::make_pair(path, std::make_shared<T>(path)));
    }

    void remove(const std::string &path);

    sf::SoundSource::Status getStatus(const std::string& path);

    void play();

    void play(const std::string &path);

    void pause();

    void pause(const std::string &path);

    void stop();

    void stop(const std::string &path);

    void setVolumeMusic(int volume) noexcept;

    void setVolumeSound(int volume) noexcept;

    [[nodiscard]] int getVolumeMusic() const noexcept;

    [[nodiscard]] int getVolumeSound() const noexcept;

    private:
    std::map<std::string, std::shared_ptr<ASound>> _noises;
    int _volumeMusic = 10;
    int _volumeSound = 10;
};

#endif //INDIE_STUDIO_TRACKS_HPP

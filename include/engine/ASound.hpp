//
// Created by Laurent on 6/4/2020.
//

#ifndef INDIE_STUDIO_SOUND_HPP
#define INDIE_STUDIO_SOUND_HPP

#include <string>
#include <utility>
#include <SFML/Audio/Music.hpp>

class ASound
{
    public:
    enum SoundType
    {
        Music,
        Sound
    };

    ASound(std::string path, SoundType type) : _path(std::move(path)), _type(type)
    {
    };

    ~ASound() = default;

    virtual sf::SoundSource::Status getStatus() = 0;

    virtual void play() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;

    virtual void setVolume(int volume) = 0;

    [[nodiscard]] SoundType getType() const noexcept
    {
        return _type;
    }

    protected:
    std::string _path;
    SoundType _type;
};

#endif //INDIE_STUDIO_SOUND_HPP

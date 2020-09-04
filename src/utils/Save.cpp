//
// Created by guillaumeg on 12/06/2020.
//

#include <regex>
#include <codecvt>
#include <fstream>
#include "utils/Save.hpp"
#include "entity/player/Player.hpp"

static std::string wstringToString(const std::wstring &utf16Str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(utf16Str);
}

Save::Save(Tracks &options) : _tracks(options), _config(".config"), _scores(".scores")
{
    std::smatch match;
    auto configLines = _config.getLines();

    if (configLines.empty())
        return;
    if (std::regex_search(configLines[0], match, std::regex("Sound:(\\d+)")))
        _tracks.setVolumeSound(std::stoi(match.str(1)));
    if (std::regex_search(configLines[1], match, std::regex("Music:(\\d+)")))
        _tracks.setVolumeMusic(std::stoi(match.str(1)));
    if (std::regex_search(configLines[2], match, std::regex("Keyboard:(\\w+)")))
        Player::isAzerty = match.str(1) == "AZERTY";
}

Save::~Save()
{
    save();
}

void Save::addScore(int score) noexcept
{
    if (_scores.getScores().find(Player::playerName) == _scores.getScores().end())
        _scores.add(Player::playerName);
    _scores.getScores()[Player::playerName] += score;
}

void Save::save() noexcept
{
    std::ofstream stream(".scores");

    for (auto &pair : _scores.getScores())
        stream << wstringToString(pair.first) << ":" << pair.second << std::endl;
    stream.close();
    stream.open(".config");

    stream << "Sound:" << _tracks.getVolumeSound() << std::endl;
    stream << "Music:" << _tracks.getVolumeMusic() << std::endl;
    stream << "Keyboard:" << Player::keyboardType() << std::endl;
}

//
// Created by arzeo on 6/12/2020.
//

#include <fstream>
#include "scenes/Ladder.hpp"

static std::string wstringToString(const std::wstring &utf16Str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(utf16Str);
}

template<typename K, typename V>
std::multimap<V, K> invertMap(std::map<K, V> const &map)
{
    std::multimap<V, K> multimap;

    for (auto const &pair: map) {
        multimap.insert(std::make_pair(pair.second, pair.first));
    }
    return multimap;
}

Ladder::Ladder(std::shared_ptr<Irrlicht> &irr, std::shared_ptr<Clock> &clock, irr::scene::ICameraSceneNode *&camera, Tracks &tracks, Scores &scores)
    : AMenu(irr, clock, camera, tracks), _scores(scores)
{
}

Enum::Scene Ladder::update()
{
    auto id = _irr->getInput()->exportIDButtonPressed();
    return id == AMenu::ButtonID::BackToMenu ? Enum::Scene::Menu : Enum::Scene::Ladder;
}

Enum::Scene Ladder::getScene()
{
    return Enum::Scene::Ladder;
}

void Ladder::setup()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    _texts.emplace_back(_irr->createText(conv.from_bytes("LADDER").c_str(), 1280 / 2 - 50, 0, 1280, 50));
    _texts.back()->setOverrideFont(_irr->getLoadedFont());
    _buttons.emplace_back(_irr->createButton({10, 720 - 77 - 10, 77, 77}, AMenu::ButtonID::BackToMenu, "asset/menu/return.png"));
    std::multimap<int, std::wstring> multimap = invertMap(_scores.getScores());
    int i = 0;
    int max = findLargestWord(multimap);
    for (auto it = multimap.rbegin(); it != multimap.rend() && i < 10; ++it) {
        ++i;
        std::string tmp = "#" + std::to_string(i) + "  " + wstringToString(it->second);
        tmp.append(max - it->second.size() + 1, ' ');
        tmp += std::to_string(it->first);
        _texts.emplace_back(_irr->createText(conv.from_bytes(tmp).c_str(), 1280 / 2 - 200, 50 + (i * 50), 1280, (i * 50) + 100));
        _texts.back()->setOverrideFont(_irr->getLoadedFont());
    }
}

void Ladder::unsetup()
{
    AMenu::unsetup();
}

int Ladder::findLargestWord(std::multimap<int, std::wstring> multimap)
{
    int max = 0;

    for (auto it = multimap.rbegin(); it != multimap.rend() && std::distance(multimap.rbegin(), it) < 10; ++it) {
        if (it->second.size() > max)
            max = it->second.size();
    }
    return max;
}


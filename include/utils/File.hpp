//
// Created by guillaumeg on 12/06/2020.
//

#ifndef INDIE_STUDIO_FILE_HPP
#define INDIE_STUDIO_FILE_HPP

#include <fstream>
#include <regex>

class File {
public:
    explicit File(const std::string& filePath)
    {
        std::string line;
        std::ifstream myfile(filePath);
        if (myfile.is_open()) {
            while (getline(myfile, line))
                _lines.push_back(line);
            myfile.close();
        } else
            std::cerr << "Unable to open file" << std::endl;
    }
    virtual ~File() = default;

    [[nodiscard]] const std::vector<std::string> &getLines() const noexcept { return _lines; }

protected:
    std::vector<std::string> _lines;
};

#include <codecvt>

class Scores : public File {
public:
    explicit Scores(const std::string &filePath) : File(filePath)
    {
        std::smatch match;
        for (auto & line : _lines) {
            if (std::regex_search(line, match, std::regex(R"(([\w\s]+):(\d+))"))) {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
                _playersScores.insert(std::make_pair(conv.from_bytes(match.str(1)), std::stoi(match.str(2))));
            }
        }
    }

    [[nodiscard]] std::map<std::wstring, int> &getScores() noexcept { return _playersScores; }

    void add(const std::wstring &str) { _playersScores.insert(std::make_pair(str, 0)); }

private:
    std::map<std::wstring, int> _playersScores;
};

#endif //INDIE_STUDIO_FILE_HPP

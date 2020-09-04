//
// Created by alexis on 26/05/2020.
//

#ifndef INDIE_STUDIO_PLAYER_HPP
#define INDIE_STUDIO_PLAYER_HPP

#include <random>
#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <entity/AEntity.hpp>
#include <engine/UserInput.hpp>
#include <entity/bomb/Bomb.hpp>
#include <array>
#include <memory>
#include <vector>
#include <engine/Irrlicht.hpp>
#include <stack>
#include <engine/Tracks.hpp>

class Map;

static std::array<irr::EKEY_CODE, 5> keyEvent[4] = {
    {irr::KEY_KEY_T, irr::KEY_KEY_F, irr::KEY_KEY_G, irr::KEY_KEY_H, irr::KEY_KEY_V},
    {irr::KEY_KEY_W, irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_D, irr::KEY_SPACE},
    {irr::KEY_KEY_Z, irr::KEY_KEY_Q, irr::KEY_KEY_S, irr::KEY_KEY_D, irr::KEY_SPACE},
    {irr::KEY_UP, irr::KEY_LEFT, irr::KEY_DOWN, irr::KEY_RIGHT, irr::KEY_RETURN}};

typedef struct playerState_s
{
    constexpr const static std::pair<irr::s32, irr::s32> RUN = {0, 26};
    constexpr const static std::pair<irr::s32, irr::s32> STAND = {27, 76};
} playerState;

class Player : public AEntity
{
    public:
    Player(irr::scene::IAnimatedMesh *mesh, std::string texturePath, const std::shared_ptr<Irrlicht> &irr, int keyEventIndex,
           const irr::core::vector3df &position) :
            AEntity(mesh, irr->createNodeFromAnimatedMesh(mesh), irr->createTexture(texturePath)),
            _irr(irr),
            _isPlayer(keyEventIndex > 0),
            _clock(new Clock),
            _texturePath(std::move(texturePath)),
            _keyMap(keyEvent[keyEventIndex]),
            _movementManager(_isPlayer ? &Player::updatePlayer : &Player::updateIA)
    {
        setFrameLoop(playerState::STAND);
        _node->setScale({1.0, 1.0, 1.0});
        _node->setAnimationSpeed(40);
        _node->setPosition(position);
        std::random_device rd;
        std::mt19937 gen(rd());
        _gen = gen;
        _bombPosition = std::make_pair(-100, -100);
    };

    Player(Player &&) = default;

    [[nodiscard]] bool isPlacing() const;

    void setIsPlacing(bool isPlacing);

    void placeBomb(const std::shared_ptr<Map> &map);

    int handleEvent(const std::shared_ptr<Map> &map, Tracks &track);

    bool explode(const std::shared_ptr<Map> &map, Tracks &track);

    enum Movement { Up = 0, Left = 1, Down = 2, Right = 3 };

    [[nodiscard]] bool getIsPlayer() const { return _isPlayer; }
    [[nodiscard]] const std::string &getTexturePath() const { return  _texturePath; }
    [[nodiscard]] const std::array<irr::EKEY_CODE, 5> &getKeyMap() const { return _keyMap; }

    static bool isAzerty;
    static std::wstring playerName;
    static std::string keyboardType() { return isAzerty ? "AZERTY" : "QWERTY"; }

private:
    std::shared_ptr<Irrlicht> _irr;
    bool _isPlayer = true;
    std::string _texturePath;
    std::array<irr::EKEY_CODE, 5> _keyMap = keyEvent[0];
    std::unique_ptr<Clock> _clock;
    std::stack<irr::scene::IMesh *> _explosionMeshes;

    bool setFrameLoop(std::pair<irr::s32, irr::s32> val);

    /************************** POWER-UP ************************************/
    bool _wallPass = false;
    double _movement = 0.2;
    int _power = 1;
    int _maxBomb = 1;

    bool tryGetPowerUP(const std::shared_ptr<Map> &map);

    /************************** MOVEMENT MANAGER ************************************/
    using movementManager = bool (Player::*)(const std::shared_ptr<Map> &);
    movementManager _movementManager;
    bool updatePlayer(const std::shared_ptr<Map> &map);

    /************************** MOVEMENT ************************************/
        // PROPERTIES
    bool _isRunning = false;
    bool _up = false;
    int _angle = 0;
    irr::core::vector3df _speed = {0, 0, 0};
        // FUNCTIONS
    [[nodiscard]] std::pair<bool, bool> checkAxesCollisions(const irr::core::vector3df& dist, const std::shared_ptr<Map> &map) const;
    [[nodiscard]] bool checkCollisions(const irr::core::vector3df& pos, irr::core::aabbox3d<irr::f32> box, const std::shared_ptr<Map> &map) const;
    bool tryMove(const irr::core::vector3df &dist, const std::shared_ptr<Map> &map);
    void phantomMode(const std::shared_ptr<Map> &map);
    void goTo(Movement movement);

    /************************** BOMB ************************************/
    bool _placing = false;
    int _bombPlaced = 0;
    bool createBombExplosion(const std::shared_ptr<Bomb> &bomb, const std::pair<std::pair<irr::core::vector3df, irr::core::vector3df>,
            std::pair<irr::core::vector3df, irr::core::vector3df>> &a, const std::shared_ptr<Map> &map, bool reverse);
    bool detectPlayerDie(irr::scene::ISceneNode *node);
    [[nodiscard]] std::pair<std::pair<irr::core::vector3df, irr::core::vector3df>, std::pair<irr::core::vector3df, irr::core::vector3df>> detectBombCollisionWithMap(
            const std::shared_ptr<Bomb> &bomb, const std::shared_ptr<Map> &map) const;

    [[nodiscard]] irr::core::vector3df
    detectBombCollapseCoordonate(const std::shared_ptr<Bomb> &bomb, const std::shared_ptr<Map> &map, const std::pair<int, int> &direction) const;

    /************************** IA ************************************/
        //ENUM
    enum IAState { WALKING, RUN_OUT };
        // PROPERTIES
    double _incr = 0;
    std::mt19937 _gen;
    std::uniform_int_distribution<int> _distribution{0, 3};
    Movement _direction = Right;
    Movement _lastDirection = Left;
    std::pair<int, int> _bombPosition;
    IAState _state = WALKING;

        //FUNCTION
    bool updateIA(const std::shared_ptr<Map> &map);
    bool checkPosBombs(std::vector<std::shared_ptr<Bomb>> &bombs, float);
    bool checkWayAvailable(std::shared_ptr<Map> const &map, float blocksize);

    bool tryToEscape(std::shared_ptr<Map> const &map, float blocksize);

    bool checkInBombs(std::shared_ptr<Map> const &map, float blocksize, int X, int Y);

    bool checkMovement(std::shared_ptr<Map> const &map, float blocksize);

    static Movement getOpposedDirection(Movement);
    std::pair<int, int> getNextPos(irr::core::vector3df const &actualPos, int modifier, float blocksize, Movement direction);
    std::pair<int, int> getRight(irr::core::vector3df const &actualPos, int modifier, float blocksize);
    std::pair<int, int> getLeft(irr::core::vector3df const &actualPos, int modifier, float blocksize);
    std::pair<int, int> getDown(irr::core::vector3df const &actualPos, int modifier, float blocksize);
    std::pair<int, int> getUp(irr::core::vector3df const &actualPos, int modifier, float blocksize);

    Movement chooseDirection(irr::core::vector3df &);

    bool checkCollision(const std::shared_ptr<Irrlicht> &irr, const std::shared_ptr<Map> &map, float blockSize);
};

#endif //INDIE_STUDIO_PLAYER_HPP

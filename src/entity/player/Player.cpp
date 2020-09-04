//
// Created by alexis on 26/05/2020.
//

#include <algorithm>
#include <map/Map.hpp>
#include <entity/player/Player.hpp>


bool Player::isAzerty = false;
std::wstring Player::playerName;

bool Player::setFrameLoop(std::pair<irr::s32, irr::s32> val)
{
    return _node->setFrameLoop(val.first, val.second);
}

void Player::goTo(Player::Movement movement)
{
    double x = movement == Movement::Left ? -_movement : movement == Movement::Right ? _movement : 0;
    double z = movement == Movement::Up ? _movement : movement == Movement::Down ? -_movement : 0;

    _speed.X += (float)x;
    _speed.Z += (float)z;
    _angle += movement == Movement::Left ? 90 : movement == Movement::Up ? 180 : movement == Movement::Down ? 0 : (!_up ? -90 : 270);
    _up = movement == Movement::Up ? true : movement == Movement::Down ? false : _up;
}

void Player::placeBomb(const std::shared_ptr<Map> &map)
{
    if (_bombPlaced >= _maxBomb)
        return;
    std::vector<irr::scene::IMeshSceneNode *> floor = map->getFloorNodeVector();
    std::vector<float> distances;
    distances.reserve(floor.size());
    for (auto &f : floor)
        distances.push_back(f->getAbsolutePosition().getDistanceFrom(getNode()->getAbsolutePosition()));
    auto minimum = std::min_element(distances.begin(), distances.end());
    int index = (int)std::distance(distances.begin(), minimum);
    auto pos = floor.at(index)->getPosition();
    if (map->getEntities().find({pos.X / map->getBlocksize(), pos.Z / map->getBlocksize()}) != map->getEntities().end())
        return;
    irr::scene::IMesh *mesh = _irr->createMesh("asset/bomb_item/Bomb.obj");
    irr::scene::IMeshSceneNode *node = _irr->createNodeFromMesh(mesh);
    node->setScale({0.2, 0.2, 0.2});
    _bombPlaced++;
    pos.Y = map->getBlocksize() / 2;
    node->setPosition(pos);
    map->add(std::make_shared<Bomb>(mesh, node, _irr->createTexture("asset/bomb_item/Bomb.png"), _bombPlaced, _power));
}

void Player::phantomMode(const std::shared_ptr<Map> &map)
{
    if (_wallPass) {
        if (_clock->getElapsedTimeSinceLastReset() > 3.0) {
            float posX = _node->getPosition().X / map->getBlocksize();
            float posY = _node->getPosition().Z / map->getBlocksize();
            std::vector<std::pair<std::pair<int, int>, float>> distanceWithEmptyBlock;
            for (int i = 0; i < (int)(map->getColumn() * map->getLine()); i++) {
                if (map->getProceduralMap().at(i) == 0) {
                    distanceWithEmptyBlock.emplace_back(
                        std::pair<int, int>((int)std::fmod((float)i, map->getColumn()), (int)std::floor((float)i / map->getColumn())),
                        (float)std::sqrt(std::pow((posX - std::fmod((float)i, map->getColumn())), 2.0) +
                            std::pow((posY - std::floor((float)i / (float)map->getColumn())), 2.0)));
                }
            }
            std::sort(distanceWithEmptyBlock.begin(), distanceWithEmptyBlock.end(), [](const auto &val, const auto &val2)
            { return val.second < val2.second; });
            _node->setPosition({distanceWithEmptyBlock[0].first.first * map->getBlocksize(), map->getBlocksize() / 2,
                                distanceWithEmptyBlock[0].first.second * map->getBlocksize()});
            _wallPass = false;
        }
    }
}

bool Player::updatePlayer(const std::shared_ptr<Map> &map)
{
    int call = 0;
    for (int i = 0; i < 4; i++) {
        if (_irr->getInput()->IsKeyDown(_keyMap[i])) {
            goTo((Player::Movement)i);
            call++;
        }
    }
    if (_isRunning != call > 0)
        setFrameLoop(call > 0 ? playerState::RUN : playerState::STAND);
    _isRunning = call > 0;
    if (_isRunning) {
        setRotation(irr::core::vector3df{0, (float)_angle / (float)call, 0});
        _speed /= (float)call;
        tryMove(_speed, map);
    }
    return false;
}

int Player::handleEvent(const std::shared_ptr<Map> &map, Tracks &track)
{
    phantomMode(map);
    (this->*_movementManager)(map);
    for (auto it = map->getBombs().begin(); it != map->getBombs().end(); it++) {
        for (const auto &it2: (*it)->getExplosionCylender()) {
            if (it2.second->hasFinished()) {
                it = map->getBombs().erase(it);
                return false;
            } else if (detectPlayerDie(it2.first->getNode())) {
                die(_irr, 0);
                return true;
            }
        }
    }
    if (!_wallPass && tryGetPowerUP(map))
        track.play("asset/soundEffect/powerUp.wav");
    _angle = 0;
    _speed = {0, 0, 0};
    return false;
}

bool Player::checkCollisions(const irr::core::vector3df& pos, irr::core::aabbox3d<irr::f32> box, const std::shared_ptr<Map> &map) const
{
    box.MinEdge += pos;
    box.MaxEdge += pos;
    box.repair();
    for (auto &it : map->getEntities()) {
        if (_wallPass && it.second->isBreakable())
            continue;
        irr::core::aabbox3d<irr::f32> b2 = it.second->getNode()->getBoundingBox();
        it.second->getNode()->getRelativeTransformation().transformBoxEx(b2);
        b2.repair();
        if (b2.intersectsWithBox(box))
            return false;
    }
    return true;
}

std::pair<bool, bool> Player::checkAxesCollisions(const irr::core::vector3df& dist, const std::shared_ptr<Map> &map) const
{
    irr::core::aabbox3d<irr::f32> b1 = {{(float)-0.3, (float)-0.3, (float)-0.3}, {(float)0.3, (float)0.5, (float)0.3}};
    irr::core::vector3df postion = _node->getPosition();
    irr::core::vector3df axeX = {dist.X, 0, 0};
    irr::core::vector3df axeY = {0, 0, dist.Z};
    auto pair = std::make_pair(false, false);

    pair.first = checkCollisions(postion + axeX, b1, map);
    b1 = {{(float)-0.3, (float)-0.5, (float)-0.3}, {(float)0.3, (float)0.5, (float)0.3}};
    pair.second = checkCollisions(postion + axeY, b1, map);
    return pair;
}

bool Player::tryMove(const irr::core::vector3df &dist, const std::shared_ptr<Map> &map)
{
    auto canMove = checkAxesCollisions(dist, map);

    if (canMove.first)
        move({dist.X, 0, 0});
    if (canMove.second)
        move({0, 0, dist.Z});
    return (canMove.first && canMove.second);
}

bool Player::createBombExplosion(const std::shared_ptr<Bomb> &bomb, const std::pair<std::pair<irr::core::vector3df, irr::core::vector3df>,
    std::pair<irr::core::vector3df, irr::core::vector3df>> &a, const std::shared_ptr<Map> &map, bool reverse)
{
    bool ret = false;
    irr::core::vector3df pos = bomb->getNode()->getPosition();
    _explosionMeshes.push(_irr->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5,
        reverse ? a.first.first.Z - a.first.second.Z - map->getBlocksize() : a.second.second.X - a.second.first.X - map->getBlocksize(), 2000,
        irr::video::SColor{0xffffffff}, false));

    irr::scene::IMeshSceneNode *node = _irr->createNodeFromMesh(_explosionMeshes.top());
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setRotation({(float)(reverse ? 90 : 0), 0, 90});
    node->setPosition(reverse ? irr::core::vector3df(pos.X, pos.Y, a.first.second.Z + map->getBlocksize() / (float)2.0) :
        irr::core::vector3df(a.second.second.X - map->getBlocksize() / 2, pos.Y, pos.Z));

    irr::scene::ISceneNodeAnimator *explosion_animator = _irr->getSceneManager()->createDeleteAnimator(500);
    node->addAnimator(explosion_animator);
    node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    if (detectPlayerDie(node)) {
        die(_irr, 0);
        ret = true;
    }
    bomb->addExplosionCylender(std::pair<std::shared_ptr<AEntity>, irr::scene::ISceneNodeAnimator *>(
        std::make_shared<AEntity>(_explosionMeshes.top(), node, _irr->createTexture("asset/map/explo2.png")), explosion_animator));
    return ret;
}

bool Player::explode(const std::shared_ptr<Map> &map, Tracks &track)
{
    bool ret = false;
    std::vector<std::shared_ptr<Bomb>> bombs = map->getBombs();
    for (const auto &bomb : bombs)
        if (bomb->destroy() && bomb->isAlive()) {
            track.play("asset/soundEffect/explo.wav");
            bomb->getReload()--;
            auto a = detectBombCollisionWithMap(bomb, map);
            ret = createBombExplosion(bomb, a, map, true) || createBombExplosion(bomb, a, map, false);
            auto animator = _irr->getSceneManager()->createDeleteAnimator(0);
            bomb->getNode()->addAnimator(animator);
            animator->drop();
            bomb->setAlive(false);
        }
    return ret;
}

std::pair<std::pair<irr::core::vector3df, irr::core::vector3df>, std::pair<irr::core::vector3df, irr::core::vector3df>> Player::detectBombCollisionWithMap(
    const std::shared_ptr<Bomb> &bomb, const std::shared_ptr<Map> &map) const
{
    std::pair<std::pair<irr::core::vector3df, irr::core::vector3df>, std::pair<irr::core::vector3df, irr::core::vector3df>> coor;
    coor.first.second = detectBombCollapseCoordonate(bomb, map, {0, -1});
    coor.first.first = detectBombCollapseCoordonate(bomb, map, {0, 1});
    coor.second.second = detectBombCollapseCoordonate(bomb, map, {1, 0});
    coor.second.first = detectBombCollapseCoordonate(bomb, map, {-1, 0});
    return (coor);
}

irr::core::vector3df
Player::detectBombCollapseCoordonate(const std::shared_ptr<Bomb> &bomb, const std::shared_ptr<Map> &map, const std::pair<int, int> &direction) const
{
    irr::core::vector3df check = bomb->getNode()->getPosition();
    std::pair<int, int> sfdgd{(int)(check.X / map->getBlocksize()), (int)(check.Z / map->getBlocksize())};
    for (int i = 0; i < bomb->getPower() + 1; i++)
    {
        auto entitie = map->getEntities().find(std::pair<int, int>(sfdgd.first, sfdgd.second));
        if (entitie != map->getEntities().end())
        {
            if (entitie->second->isBreakable())
            {
                entitie->second->die(_irr, 500);
                map->getProceduralMap().getMap()[(int)(sfdgd.first + (sfdgd.second * map->getColumn()))] = 0;
                map->getEntities().erase(std::pair<int, int>(sfdgd.first, sfdgd.second));
            }
            break;
        }
        sfdgd.first += direction.first;
        sfdgd.second += direction.second;
    }
    return {sfdgd.first * map->getBlocksize(), map->getBlocksize(), sfdgd.second * map->getBlocksize()};
}

bool Player::isPlacing() const
{
    return _placing;
}

void Player::setIsPlacing(bool isPlacing)
{
    Player::_placing = isPlacing;
}

bool Player::detectPlayerDie(irr::scene::ISceneNode *node)
{
    irr::core::aabbox3d<irr::f32> b1 = {{(float)-0.4, (float)-0.4, (float)-0.4}, {(float)0.4, (float)0.5, (float)0.4}};
    irr::core::vector3df lol = _node->getPosition();
    irr::core::aabbox3d<irr::f32> b2 = node->getBoundingBox();

    b1.MinEdge += lol;
    b1.MaxEdge += lol;
    b1.repair();
    node->getRelativeTransformation().transformBoxEx(b2);
    b2.repair();
    return b2.intersectsWithBox(b1);
}

bool Player::tryGetPowerUP(const std::shared_ptr<Map> &map)
{
    irr::core::aabbox3d<irr::f32> b1 = {{(float)-0.3, (float)-0.3, (float)-0.3}, {(float)0.3, (float)0.5, (float)0.3}};
    irr::core::vector3df lol = _node->getPosition();
    b1.MinEdge += lol;
    b1.MaxEdge += lol;
    b1.repair();

    for (auto it = map->getPowerUpMap().begin(); it != map->getPowerUpMap().end(); ++it)
    {
        irr::core::aabbox3d<irr::f32> b2 = it->second->getNode()->getBoundingBox();
        it->second->getNode()->getRelativeTransformation().transformBoxEx(b2);
        b2.repair();
        if (b2.intersectsWithBox(b1))
        {
            if (it->second->getType() == 3)
                ++_power;
            else if (it->second->getType() == 4)
                ++_maxBomb;
            else if (it->second->getType() == 5)
                _movement += 0.02;
            else if (it->second->getType() == 6)
            {
                _wallPass = true;
                _clock->reset();
            }
            it->second->die(_irr, 0);
            map->getPowerUpMap().erase(it);
            return true;
        }
    }
    return false;
}

std::pair<int, int> Player::getRight(irr::core::vector3df const &actualPos, int modifier, float blocksize)
{
    int X = (int)(std::round(_node->getPosition().X / blocksize)) + modifier;
    int Y = (int)(std::round(_node->getPosition().Z / blocksize));
    return std::make_pair(X, Y);
}

std::pair<int, int> Player::getLeft(irr::core::vector3df const &actualPos, int modifier, float blocksize)
{
    int X = (int)(std::round(_node->getPosition().X / blocksize)) - modifier;
    int Y = (int)(std::round(_node->getPosition().Z / blocksize));
    return std::make_pair(X, Y);
}

std::pair<int, int> Player::getDown(irr::core::vector3df const &actualPos, int modifier, float blocksize)
{
    int X = (int)(std::round(_node->getPosition().X / blocksize));
    int Y = (int)(std::round(_node->getPosition().Z / blocksize)) + modifier;
    return std::make_pair(X, Y);
}

std::pair<int, int> Player::getUp(irr::core::vector3df const &actualPos, int modifier, float blocksize)
{
    int X = (int)(std::round(_node->getPosition().X / blocksize));
    int Y = (int)(std::round(_node->getPosition().Z / blocksize)) - modifier;
    return std::make_pair(X, Y);
}

std::pair<int, int> Player::getNextPos(irr::core::vector3df const &actualPos, int modifier, float blocksize, Movement direction)
{
    int X = (int)(std::round(_node->getPosition().X / blocksize)) + (direction == Right ? 1 : direction == Left ? -1 : 0);
    int Y = (int)(std::round(_node->getPosition().Z / blocksize)) + (direction == Up ? 1 : direction == Down ? -1 : 0);
    return std::make_pair(X, Y);
}

bool Player::updateIA(const std::shared_ptr<Map> &map)
{
    float blockSize = map->getBlocksize();
    checkCollision(_irr, map, blockSize);
    return false;
}

Player::Movement Player::chooseDirection(irr::core::vector3df &pos)
{
    return Right;
}

bool Player::checkCollision(const std::shared_ptr<Irrlicht> &irr, const std::shared_ptr<Map> &map, float blocksize)
{
    if (_incr <= blocksize) {
        goTo(_direction);
        if (tryMove(_speed, map)) {
            _incr += _movement;
            setRotation(irr::core::vector3df{0, (float)_angle, 0});
            return true;
        }
        setRotation(irr::core::vector3df{0, (float)_angle, 0});
    } else {
        setFrameLoop(playerState::STAND);
    }
    int X = (int)(std::round(_node->getPosition().X / blocksize)) + (_direction == Right ? 1 : _direction == Left ? -1 : 0);
    int Y = (int)(std::round(_node->getPosition().Z / blocksize)) + (_direction == Up ? 1 : _direction == Down ? -1 : 0);
    auto bombs = map->getBombs();

    static Movement lastDirection = _direction;
    if (bombs.empty()) {
        _state = WALKING;
    }
    if (_state == RUN_OUT) {
        if (checkMovement(map, blocksize)) {
            goTo(_direction);
            tryMove(_speed, map);
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
            return false;
        }
    }
    bool needToBreak = false;
    for (auto &b : bombs) {
        auto bPos = b->getPosition();
        int bX = (int)(std::round(bPos.X / blocksize));
        int bY = (int)(std::round(bPos.Z / blocksize));
        for (int i = b->getPower(); i > 0; --i) {
            if ((X == bX + i && Y == bY) || (X == bX && Y == bY + i)
            || (X == bX - i && Y == bY) || (X == bX && Y == bY - i)) {
                if (_state == WALKING) {
                    _state = RUN_OUT;
                    _direction = getOpposedDirection(_direction);
                    _lastDirection = getOpposedDirection(_direction);
                    X = (int)(std::round(_node->getPosition().X / blocksize)) + (_direction == Right ? 1 : _direction == Left ? -1 : 0);
                    Y = (int)(std::round(_node->getPosition().Z / blocksize)) + (_direction == Up ? 1 : _direction == Down ? -1 : 0);
                }
                needToBreak = true;
                break;
            } else if (map->getEntities().find({X, Y}) == map->getEntities().end()) {
                _state = WALKING;
            }
            if (needToBreak)
                break;
        }
    }

    if (map->getEntities().find({X, Y}) == map->getEntities().end()) {
        goTo(_direction);
        if (!tryMove(_speed, map)) {
            auto way = _distribution(_gen);
            while (way == getOpposedDirection(_direction))
                way = _distribution(_gen);
            _lastDirection = getOpposedDirection((Player::Movement)way);
            _direction = (Player::Movement)way;
            goTo(_direction);
            tryMove(_speed, map);
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
            setRotation(irr::core::vector3df{0, (float)_angle, 0});
        } else {
            setRotation(irr::core::vector3df{0, (float) _angle, 0});
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
        }
    } else if (map->getEntities().at({X, Y})->isBreakable() && _state == WALKING) {
        if (checkWayAvailable(map, blocksize)) {
            goTo(_direction);
            tryMove(_speed, map);
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
            setRotation(irr::core::vector3df{0, (float)_angle, 0});
        } else if (!checkPosBombs(bombs, blocksize)) {
            placeBomb(map);
            _bombPosition = std::make_pair((int)(std::round(_node->getPosition().X / blocksize)),
                    (int)(std::round(_node->getPosition().Z / blocksize)));
            _state = RUN_OUT;
            _direction = _lastDirection;
            _lastDirection = getOpposedDirection(_direction);
            goTo(_direction);
            tryMove(_speed, map);
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
            setRotation(irr::core::vector3df{0, (float)_angle, 0});
        } else if (checkInBombs(map, blocksize, (int)(std::round(_node->getPosition().X / blocksize)), (int)(std::round(_node->getPosition().Z / blocksize)))) {
            auto way = _distribution(_gen);
            while (way == getOpposedDirection(_direction))
                way = _distribution(_gen);
            _lastDirection = getOpposedDirection((Player::Movement)way);
            _direction = (Player::Movement)way;
            goTo(_direction);
            tryMove(_speed, map);
            _incr = 0;
            _incr += _movement;
            setFrameLoop(playerState::RUN);
            setRotation(irr::core::vector3df{0, (float)_angle, 0});
        }
        return false;
    } else {
        if (_state == RUN_OUT) {
            if (checkMovement(map, blocksize)) {
                goTo(_direction);
                tryMove(_speed, map);
                _incr = 0;
                _incr += _movement;
                setFrameLoop(playerState::RUN);
                return false;
            }
        }
        auto way = _distribution(_gen);
        while (way == getOpposedDirection(_direction))
            way = _distribution(_gen);
        _lastDirection = getOpposedDirection((Player::Movement)way);
        _direction = (Player::Movement)way;
        goTo(_direction);
        tryMove(_speed, map);
        _incr = 0;
        _incr += _movement;
        setFrameLoop(playerState::RUN);
        setRotation(irr::core::vector3df{0, (float)_angle, 0});
    }
    return false;
}

Player::Movement Player::getOpposedDirection(Movement actualDirection)
{
    return actualDirection == Left ? Right : actualDirection == Right ? Left
    : actualDirection == Up ? Down : actualDirection == Down ? Up : Left;
}

bool Player::checkPosBombs(std::vector<std::shared_ptr<Bomb>> &bombs, float blocksize) {
    for (auto &b : bombs) {
        auto bPos = b->getPosition();
        int bX = (int)(std::round(bPos.X / blocksize));
        int bY = (int)(std::round(bPos.Z / blocksize));
        if (bX == _bombPosition.first && bY == _bombPosition.second) {
            return true;
        }
    }
    return false;
}

bool Player::checkWayAvailable(std::shared_ptr<Map> const &map, float blocksize) {
    auto way = _distribution(_gen);
    while (way == getOpposedDirection(_direction))
        way = _distribution(_gen);
    int X = (int)(std::round(_node->getPosition().X / blocksize)) + ((Player::Movement)way == Right ? 1 : (Player::Movement)way == Left ? -1 : 0);
    int Y = (int)(std::round(_node->getPosition().Z / blocksize)) + ((Player::Movement)way == Up ? 1 : (Player::Movement)way == Down ? -1 : 0);
    if (map->getEntities().find({X, Y}) == map->getEntities().end() && !checkInBombs(map, blocksize, X, Y)) {
        _lastDirection = getOpposedDirection((Player::Movement)way);
        _direction = (Player::Movement) way;
        return true;
    }
    return false;
}

bool Player::tryToEscape(std::shared_ptr<Map> const &map, float blocksize) {
    auto bombs = map->getBombs();
    int X, Y;
    bool isInBomb = false;

    for (int i = 0; i < 4; ++i) {
        isInBomb = false;
        X = getNextPos(_node->getPosition(), 1, blocksize, (Player::Movement)i).first;
        Y = getNextPos(_node->getPosition(), 1, blocksize, (Player::Movement)i).second;
        for (auto &b : bombs) {
            auto bPos = b->getPosition();
            int bX = (int)(std::round(bPos.X / blocksize));
            int bY = (int)(std::round(bPos.Z / blocksize));
            for (int a = b->getPower(); a > 0; --a) {
                if ((X == bX + a && Y == bY) || (X == bX && Y == bY + a)
                || (X == bX - a && Y == bY) || (X == bX && Y == bY - a)) {
                    isInBomb = true;
                    break;
                }
            }
            if (!isInBomb) {
                if (map->getEntities().find({X, Y}) == map->getEntities().end()) {
                    _state = WALKING;
                    _direction = (Player::Movement) i;
                    _lastDirection = getOpposedDirection(_direction);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Player::checkInBombs(const std::shared_ptr<Map> &map, float blocksize, int X, int Y) {
    auto bombs = map->getBombs();

    for (auto &b : bombs) {
        auto bPos = b->getPosition();
        int bX = (int)(std::round(bPos.X / blocksize));
        int bY = (int)(std::round(bPos.Z / blocksize));
        for (int i = b->getPower(); i > 0; --i) {
            if ((X == bX + i && Y == bY) || (X == bX && Y == bY + i)
            || (X == bX - i && Y == bY) || (X == bX && Y == bY - i)) {
                return true;
            }
        }
    }
    return false;
}

bool Player::checkMovement(const std::shared_ptr<Map> &map, float blocksize) {
    auto bombs = map->getBombs();
    int X, Y;
    bool isInBomb = false;

    for (int i = 0; i < 4; ++i) {
        if (i != _lastDirection) {
            X = getNextPos(_node->getPosition(), 1, blocksize, (Player::Movement)i).first;
            Y = getNextPos(_node->getPosition(), 1, blocksize, (Player::Movement)i).second;
            if (map->getEntities().find({X, Y}) == map->getEntities().end()) {
                if (!checkInBombs(map, blocksize, X, Y))
                    _state = WALKING;
                _direction = (Player::Movement)i;
                _lastDirection = getOpposedDirection(_direction);
                return true;
            }
        }
    }
    return false;
}

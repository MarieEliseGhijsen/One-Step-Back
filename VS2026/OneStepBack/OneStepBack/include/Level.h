#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include <vector>
#include <queue>
#include <string>

#include <algorithm>
#include <cmath>

#include "Enemy.h"

const static int MAX_NEIGHBOORS = 4;

struct tileData
{
    int m_type = 0;
    std::vector<int> m_neighboors;
};

struct LevelData
{
    int m_width = 0;
    int m_height = 0;

    std::vector<tileData> m_tiles;
};

class Player;

class Level
{
public:
    Level() = default;
    void loadLevel(int t_levelNr);
    void loadTileNeighboors(int t_index, int t_row, int t_col);

    bool update(sf::Vector2f t_playerPosition);

    std::vector<int> breadthFirstSearch(sf::Vector2f t_startPos, sf::Vector2f t_endPos);
    int asCell(sf::Vector2f t_pos) const;

    void checkCollision();
    bool isWalkable(sf::Vector2f t_pos) const;
    bool isWalkable(int t_type) const;
    int getTileType(int t_x, int t_y) const;

    int getWidth() const;
    int getHeight() const;

    sf::Vector2f getStartPosition() const;

    const std::vector<Enemy>& getEnemies() const;

private:
    LevelData m_level;

    sf::Vector2f m_startPosition = sf::Vector2f(-1, -1);
    sf::Vector2f m_endPosition = sf::Vector2f(-1, -1);

    std::vector<Enemy> m_enemies;
};
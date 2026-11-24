#include "../include/Enemy.h"

Enemy::Enemy(sf::Vector2f t_startPosition)
{
    m_position = t_startPosition;
}

void Enemy::update(const std::vector<int>& t_path, int t_mapWidth)
{
    if (t_path.size() <= 1)
    {
        return;
    }

    int nextCell = t_path[1];

    int x = nextCell % t_mapWidth;
    int y = nextCell / t_mapWidth;

    m_position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

    std::cout << "enemy position : " << m_position.x << ", " << m_position.y << "\n";
}

sf::Vector2f Enemy::getPosition() const
{
    return m_position;
}
#include "Enemy.h"

Enemy::Enemy(sf::Vector2f t_startPosition)
{
    m_position = t_startPosition;
}

void Enemy::update(const std::vector<int>& t_path, int t_mapWidth)
{
    if (t_path.empty())
    {
        std::cout << "path empty" << "\n";
        return;
    }

    if (m_path != t_path)
    {
        std::cout << "no path" << "\n";
        m_path = t_path;
        m_currentPathIndex = 0;
    }

    if (m_currentPathIndex < static_cast<int>(m_path.size()))
    {
        std::cout << "position : " << m_position.x << ", " << m_position.y << "\n";
        int currentCell = m_path[m_currentPathIndex];

        int x = currentCell % t_mapWidth;
        int y = currentCell / t_mapWidth;

        m_position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

        m_currentPathIndex++;
    }
}

sf::Vector2f Enemy::getPosition() const
{
    return m_position;
}
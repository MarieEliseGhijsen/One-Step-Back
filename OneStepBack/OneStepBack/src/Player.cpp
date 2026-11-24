#include "../include/Player.h"

void Player::setPosition(sf::Vector2f t_startPosition)
{
	m_position = t_startPosition;
	m_previousPositions = std::stack<sf::Vector2f>();
}

void Player::update(int t_direction)
{
	m_previousPositions.push(m_position);

	switch (t_direction)
	{
	case Globals::UP:
		m_position.y--;
		break;
	case Globals::DOWN:
		m_position.y++;
		break;
	case Globals::LEFT:
		m_position.x--;
		break;
	case Globals::RIGHT:
		m_position.x++;
		break;
	}

	std::cout << "player position : " << m_position.x << ", " << m_position.y << "\n";
}

bool Player::rewind(const Level& t_level)
{
    if (m_previousPositions.size() < REWIND_AMOUNT)
    {
        return false;
    }

    sf::Vector2f rewindPosition = m_position;
    std::stack<sf::Vector2f> tempPositions = m_previousPositions;

    for (int i = 0; i < REWIND_AMOUNT; i++)
    {
        if (tempPositions.empty())
        {
            return false;
        }

        rewindPosition = tempPositions.top();
        tempPositions.pop();
    }

    if (!t_level.isWalkable(rewindPosition))
    {
        return false;
    }

    for (int i = 0; i < REWIND_AMOUNT; i++)
    {
        m_position = m_previousPositions.top();
        m_previousPositions.pop();
    }

    //std::cout << "Player rewound to: " << m_position.x << ", " << m_position.y << "\n";

    return true;
}

sf::Vector2f Player::getPosition() const
{
	return m_position;
}
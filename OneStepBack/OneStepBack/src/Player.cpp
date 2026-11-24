#include "../include/Player.h"

void Player::setPosition(sf::Vector2f t_startPosition)
{
	m_position = t_startPosition;
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

bool Player::rewind()
{
	if (m_previousPositions.size() < REWIND_AMOUNT) return false;

    if (!m_previousPositions.empty())
    {
		for(int i = 0; i < 3; i++)
		{
			m_position = m_previousPositions.top();
        	m_previousPositions.pop();
		}
    }

	return true;
}

sf::Vector2f Player::getPosition() const
{
	return m_position;
}
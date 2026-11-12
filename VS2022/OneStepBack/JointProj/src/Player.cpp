#include "../include/Player.h"

void Player::setPosition(sf::Vector2f t_startPosition)
{
	m_position = t_startPosition;
}

void Player::update(int t_direction)
{
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

sf::Vector2f Player::getPosition() const
{
	return m_position;
}
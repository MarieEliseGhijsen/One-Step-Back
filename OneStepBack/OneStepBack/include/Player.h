#pragma once

#include <SFML/Graphics.hpp>

#include <stack>

#include "Globals.h"
#include "Level.h"

class Player
{
public:
	void setPosition(sf::Vector2f t_startPosition);

	void update(int t_direction);
	bool rewind(const Level& t_level);

	sf::Vector2f getPosition() const;

private:
	sf::Vector2f m_position;
    std::stack<sf::Vector2f> m_previousPositions;
	int const REWIND_AMOUNT = 4;
};
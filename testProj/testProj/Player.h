#pragma once

#include <SFML/Graphics.hpp>

#include "Level.h"

class Player
{
public:
	void update();					//updates the player
	void handleInput(float t_dt, const Level& t_level);

	sf::Vector2f getPosition() const;
	float getAngle() const;

private:
	sf::Vector2f m_position{ 10.0f, 10.0f };
	sf::Vector2f m_forward{ 0.0f, 0.0f };
	sf::Vector2f m_strafe{ 0.0f, 0.0f };

	sf::Vector2f m_velocity{ 0.0f, 0.0f };
	float m_acceleration{ 30.0f };
	float m_friction{ 0.85f };
	const float m_MAX_SPEED{ 10.0f };

	float m_angle{ 0.0f };
	const static int m_ROTATION_SPEED{ 2 };
};
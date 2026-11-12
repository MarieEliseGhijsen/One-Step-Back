#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>

class Enemy
{
public:
	Enemy(sf::Vector2f t_startPosition);

	void update(const std::vector<int>& t_path, int t_mapWidth);

	sf::Vector2f getPosition() const;

private:
	sf::Vector2f m_position;
	std::vector<int> m_path;
	int m_currentPathIndex = 0;
};
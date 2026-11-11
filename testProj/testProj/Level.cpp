#include "Level.h"
#include "Player.h"

#include <fstream>
#include <stdexcept>

#include <filesystem>
#include <iostream>

void Level::loadLevel(int t_levelNr)
{
	std::string filename = "../resources/levelData/level" + std::to_string(t_levelNr) + ".txt";
	std::ifstream file(filename, std::ios::binary);

	if (file.is_open())
	{
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Could not open : " + filename);
	}

	char buffer[10] = {};
	file.read(buffer, 10);

	std::cout << "First 10 bytes :";
	for (int i = 0; i < file.gcount(); i++)
	{
		std::cout << std::hex << (static_cast<unsigned int>(static_cast<unsigned char>(buffer[i]))) << " ";
	}
	std::cout << std::dec << "\n";

	LevelData newLevel;

	int firstValu;
	if (file >> firstValu)
	{
		std::cout << "First value read : " << firstValu << "\n";
	}
	else
	{
		std::cout << "Failed to read first value" << "\n";
	}

	if (!(file >> newLevel.m_width >> newLevel.m_height))
	{
		throw std::runtime_error("Could not read level dimensions of : " + filename);
	}

	if (newLevel.m_width <= 0 || newLevel.m_height <= 0)
	{
		throw std::runtime_error("Invalid level dimensions in : " + filename);
	}

	int tileType;

	for (int i = 0; i < newLevel.m_width * newLevel.m_height; i++)
	{
		if (!(file >> tileType))
		{
			throw std::runtime_error("Incorrect amount of tile data in : " + filename);
		}

		tileData tile;
		tile.m_type = tileType;
		newLevel.m_tiles.push_back(tile);
	}

	file.close();

	m_level = std::move(newLevel);

	for (int y = 0; y < m_level.m_height; y++)
	{
		for (int x = 0; x < m_level.m_width; x++)
		{
			int index = y * m_level.m_width + x;
			int tileType = m_level.m_tiles[index].m_type;

			if (tileType == 8)
			{
				m_startPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
			}
			else if (tileType == 9)
			{
				m_endPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
			}
		}
	}

	if (m_startPosition.x < 0 || m_endPosition.x < 0)
	{
		throw std::runtime_error("Level is missing a start point or end point in : " + filename);
	}

	//load tile neighboors
	for (int i = 0; i < m_level.m_tiles.size(); i++)
	{
		m_level.m_tiles[i].m_neighboors.clear();
		loadTileNeighboors(i, i / m_level.m_width, i % m_level.m_width);
	}

	//////////////////////////////////////////////////////
	//testing
	m_enemies.clear();
	m_enemies.emplace_back(m_startPosition);
	//////////////////////////////////////////////////////
}

void Level::loadTileNeighboors(int t_index, int t_row, int t_col)
{
	int rowOffset[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int colOffset[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	int newRow = 0;
	int newCol = 0;
	int newTile = 0;

	for (int i = 0; i < MAX_NEIGHBOORS; i++)
	{
		newRow = t_row + rowOffset[i];
		newCol = t_col + colOffset[i];

		if (newRow < 0 || newRow >= m_level.m_height || newCol < 0 || newCol >= m_level.m_width)
		{
			continue;
		}

		newTile = (newRow * m_level.m_width) + newCol;
		m_level.m_tiles.at(t_index).m_neighboors.push_back(newTile);
	}
}

void Level::update()
{
	std::vector<int> enemyPath = breadthFirstSearch(m_startPosition, m_endPosition);

	for (Enemy& enemy : m_enemies)
	{
		enemy.update(enemyPath, m_level.m_width);
		std::cout << "updating enemy" << "\n";
	}
}

std::vector<int> Level::breadthFirstSearch(sf::Vector2f t_startPos, sf::Vector2f t_endPos)
{
	int startCell = asCell(t_startPos);
	int endCell = asCell(t_endPos);

	if (!isWalkable(m_level.m_tiles[startCell].m_type) || !isWalkable(m_level.m_tiles[endCell].m_type))
	{
		return {};
	}

	std::vector<bool> visited(m_level.m_tiles.size(), false);
	std::vector<int> parent(m_level.m_tiles.size(), -1);

	std::queue<int> queue;

	visited.at(startCell) = true;
	queue.push(startCell);

	int current, currentRow, currentCol;
	int neighboorRow, neighboorCol;
	int dRow, dCol;

	int tile1, tile2;

	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();

		if (current == endCell)
		{
			break;
		}

		for (int neighboor : m_level.m_tiles.at(current).m_neighboors)
		{
			if (visited.at(neighboor) || !isWalkable(m_level.m_tiles.at(neighboor).m_type)) continue;

			currentRow = current / m_level.m_width;
			currentCol = current % m_level.m_width;

			neighboorRow = neighboor / m_level.m_width;
			neighboorCol = neighboor % m_level.m_width;

			dRow = neighboorRow - currentRow;
			dCol = neighboorCol - currentCol;

			if (abs(dRow) == 1 && abs(dCol) == 1)
			{
				tile1 = currentRow * m_level.m_width + (currentCol + dCol); //horizontal
				tile2 = (currentRow + dRow) * m_level.m_width + currentCol; //vertical

				if (!isWalkable(m_level.m_tiles.at(tile1).m_type) || !isWalkable(m_level.m_tiles.at(tile2).m_type))
				{
					continue;
				}
			}

			visited.at(neighboor) = true;
			parent.at(neighboor) = current;

			queue.push(neighboor);
		}
	}

	std::vector<int> path;
	current = endCell;

	if (!visited.at(current))
	{
		return path; //no path found
	}

	while (current != -1)
	{
		path.push_back(current);
		current = parent.at(current);
	}

	std::reverse(path.begin(), path.end());

	return path;
}

int Level::asCell(sf::Vector2f t_pos)
{
	int x = std::clamp(static_cast<int>(t_pos.x), 0, m_level.m_width - 1);
	int y = std::clamp(static_cast<int>(t_pos.y), 0, m_level.m_height - 1);

	return y * m_level.m_width + x;
}

void Level::checkCollision()
{
}

bool Level::isWalkable(int t_type) const
{
	return t_type != 1 && t_type != 4;
}

int Level::getTileType(int t_x, int t_y) const
{
	if (t_x < 0 || t_x >= m_level.m_width || t_y < 0 || t_y >= m_level.m_height) return 1;
	return m_level.m_tiles[t_y * m_level.m_width + t_x].m_type;
}

int Level::getWidth() const
{
	return m_level.m_width;
}

int Level::getHeight() const
{
	return m_level.m_height;
}

const std::vector<Enemy>& Level::getEnemies() const
{
	return m_enemies;
}
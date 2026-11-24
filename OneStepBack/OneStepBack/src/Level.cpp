#include "../include/Level.h"
#include "../include/Player.h"

#include <fstream>
#include <stdexcept>

#include <filesystem>
#include <iostream>

void Level::loadLevel(int t_levelNr)
{
	m_currentLevelNumber = t_levelNr;
	m_levelCompleted = false;

	std::string filename = "Resources/LevelData/level" + std::to_string(t_levelNr) + ".txt";
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Could not open : " + filename);
	}

	LevelData newLevel;

	if (!(file >> newLevel.m_width >> newLevel.m_height))
	{
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Could not read level dimensions of : " + filename);
	}

	if (newLevel.m_width <= 0 || newLevel.m_height <= 0)
	{
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Invalid level dimensions in : " + filename);
	}

	if (newLevel.m_width > 1000 || newLevel.m_height > 1000)
	{
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Level dimensions are too large in : " + filename);
	}

	int tileType;
	std::vector<sf::Vector2f> enemyStartPositions;
	int coins = 0;

	for (int i = 0; i < newLevel.m_width * newLevel.m_height; i++)
	{
		if (!(file >> tileType))
		{
			std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
			std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
			throw std::runtime_error("Incorrect amount of tile data in : " + filename);
		}

		if (tileType == 7)
		{
			int x = i % newLevel.m_width;
			int y = i / newLevel.m_width;

			enemyStartPositions.push_back(sf::Vector2f( static_cast<float>(x), static_cast<float>(y)));
			tileType = 0;
		}

		if (tileType == 6)
		{
			coins++;
		}

		tileData tile;
		tile.m_type = tileType;
		newLevel.m_tiles.push_back(tile);
	}

	if (coins != 3)
	{
		throw std::runtime_error( "Incorrect amount of coin data in : " + filename);
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
		std::cout << "Current working directory : " << std::filesystem::current_path() << "\n";
		std::cout << "Attempting to load : " << std::filesystem::absolute(filename) << "\n";
		throw std::runtime_error("Level is missing a start point or end point in : " + filename);
	}

	//load tile neighboors
	for (int i = 0; i < m_level.m_tiles.size(); i++)
	{
		m_level.m_tiles[i].m_neighboors.clear();
		loadTileNeighboors(i, i / m_level.m_width, i % m_level.m_width);
	}

	m_enemies.clear();
	for (sf::Vector2f position : enemyStartPositions)
	{
		m_enemies.emplace_back(position);
	}

	m_levelScore = 0;
	m_collectedCoins = 0;

	m_levelStartTime = std::chrono::steady_clock::now();
	m_timeBonus = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//testing
	//m_enemies.clear();
	//m_enemies.emplace_back(m_endPosition);
	// //std::cout << "Enemy tile: " << getTileType(m_startPosition.x, m_startPosition.y) << "\n";
	//std::cout << "Player tile: " << getTileType(m_endPosition.x, m_endPosition.y) << "\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Level::loadTileNeighboors(int t_index, int t_row, int t_col)
{
	//int rowOffset[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	//int colOffset[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	int rowOffset[] = { -1, 0, 1, 0 };
	int colOffset[] = { 0, 1, 0, -1 };

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

bool Level::update(sf::Vector2f t_playerPosition)
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		std::vector<int> path = breadthFirstSearch(m_enemies[i].getPosition(), t_playerPosition);

		if (path.size() <= 1)
		{
			continue;
		}

		int nextCell = path[1];

		int nextX = nextCell % m_level.m_width;
		int nextY = nextCell / m_level.m_width;

		sf::Vector2f nextPosition(static_cast<float>(nextX), static_cast<float>(nextY));

		bool tileOccupied = false;

		for (int j = 0; j < m_enemies.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			if (m_enemies[j].getPosition() == nextPosition)
			{
				tileOccupied = true;
				break;
			}
		}

		if (!tileOccupied)
		{
			m_enemies[i].update(path, m_level.m_width);
		}
	}

	for (const Enemy& enemy : m_enemies)
	{
		if (enemy.getPosition() == t_playerPosition)
		{
			//std::cout << "Enemy has caught player - level update returning true" << "\n";
			return true;
		}
	}

	if (t_playerPosition == m_endPosition)
	{
		//std::cout << "Player reached end point - level update returning true" << "\n";

		if (!m_levelCompleted)
		{
			auto endTime = std::chrono::steady_clock::now();
			int elapsedSeconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(endTime - m_levelStartTime).count());

			m_levelScore += 100;
			m_timeBonus = std::max(0, 300 - (elapsedSeconds * 5)); //Time Bonus = max(0, 300 - 5t)
			m_levelScore += m_timeBonus;

			if (m_collectedCoins == 3)
			{
				m_levelScore += 200;
			}

			m_levelScores[m_currentLevelNumber] = m_levelScore;
			m_levelCompleted = true;
		}

		return true;
	}

	return false;
}

std::vector<int> Level::breadthFirstSearch(sf::Vector2f t_startPos, sf::Vector2f t_endPos)
{
	int startCell = asCell(t_startPos);
	int endCell = asCell(t_endPos);

	if (startCell == -1 || endCell == -1)
    {
        return {};
    }

	if (!isWalkable(m_level.m_tiles[startCell].m_type) || !isWalkable(m_level.m_tiles[endCell].m_type))
	{
		return {};
	}

	std::vector<bool> visited(m_level.m_tiles.size(), false);
	std::vector<int> parent(m_level.m_tiles.size(), -1);

	std::queue<int> queue;

	visited.at(startCell) = true;
	queue.push(startCell);

	int current;

	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();

		if (current == endCell)
		{
			break;
		}

		for (int neighboor : m_level.m_tiles[current].m_neighboors)
		{
			if (visited[neighboor] || !isWalkable(m_level.m_tiles[neighboor].m_type)) continue;

            visited[neighboor] = true;
            parent[neighboor] = current;
			
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

int Level::asCell(sf::Vector2f t_pos) const
{
	if (std::floor(t_pos.x) != t_pos.x || std::floor(t_pos.y) != t_pos.y || t_pos.x < 0 || t_pos.x >= m_level.m_width || t_pos.y < 0 || t_pos.y >= m_level.m_height)
    {
        return -1;
    }

	int x = static_cast<int>(t_pos.x);
    int y = static_cast<int>(t_pos.y);

	return y * m_level.m_width + x;
}

bool Level::isWalkable(sf::Vector2f t_pos) const
{
	int tileType = getTileType(t_pos.x, t_pos.y);
	return tileType != 1 && tileType != 4;
}

void Level::handlePlayerEntry(sf::Vector2f t_previousPosition, sf::Vector2f t_newPosition)
{
	if (t_previousPosition == t_newPosition)
	{
		return;
	}

	int newCell = asCell(t_newPosition);

	if (newCell == -1)
	{
		return;
	}

	int tileType = m_level.m_tiles[newCell].m_type;

	if (tileType == 6)
	{
		m_collectedCoins++;
		m_levelScore += 50;

		m_level.m_tiles[newCell].m_type = 0;

		std::cout << "Special coin collected! +50 points" << "\n";
		std::cout << "Coins: " << m_collectedCoins << " / " << m_TOTAL_COINS << "\n";
		return;
	}

	if (tileType != 5)
	{
		return;
	}

	for (tileData& tile : m_level.m_tiles)
	{
		if (tile.m_type == 3)
		{
			tile.m_type = 4;
		}
		else if (tile.m_type == 4)
		{
			tile.m_type = 3;
		}
	}
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

sf::Vector2f Level::getStartPosition() const
{
	return m_startPosition;
}

const std::vector<Enemy>& Level::getEnemies() const
{
	return m_enemies;
}

int Level::getLevelScore() const
{
	return m_levelScore;
}

int Level::getCollectedCoins() const
{
	return m_collectedCoins;
}

int Level::getTimeBonus() const
{
	return m_timeBonus;
}

const std::map<int, int>& Level::getLevelScores() const
{
	return m_levelScores;
}

int Level::getTotalScore() const
{
	int total = 0;

	for (const auto& score : m_levelScores)
	{
		total += score.second;
	}

	return total;
}
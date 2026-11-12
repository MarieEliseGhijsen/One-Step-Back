#include "../include/Window.h"

Window::Window() : m_window(sf::VideoMode({ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT }), Globals::WINDOW_TITLE)
{
	m_window.setVerticalSyncEnabled(true);
}

std::optional<sf::Event> Window::pollEvent()
{
	return m_window.pollEvent();
}

void Window::renderGameRunning(const Player& t_player, const Level& t_level)
{
	m_window.clear();

	renderLevel(t_player, t_level);

	m_window.display();
}

void Window::renderLevel(const Player& t_player, const Level& t_level)
{
	float tileWidth = static_cast<float>(m_window.getSize().x) / t_level.getWidth();
	float tileHeight = static_cast<float>(m_window.getSize().y) / t_level.getHeight();

	sf::RectangleShape tile;
	tile.setSize({ tileWidth, tileHeight });

	for (int col = 0; col < t_level.getHeight(); col++)
	{
		for (int row = 0; row < t_level.getWidth(); row++)
		{
			int tileType = t_level.getTileType(row, col);

			//draw tiles first
			switch (tileType)
			{
			case 0: //empty
				tile.setFillColor(sf::Color::White);
				break;

			case 1: //wall
				tile.setFillColor(sf::Color::Black);
				break;

			case 3: //unlocked door
				tile.setFillColor(sf::Color::Yellow);
				break;

			case 4: //locked door
				tile.setFillColor(sf::Color(128, 128, 128));
				break;

			case 8: //start
				tile.setFillColor(sf::Color(255, 165, 0));
				break;

			case 9: //end
				tile.setFillColor(sf::Color::Blue);
				break;

			default:
				tile.setFillColor(sf::Color::Magenta);
				break;
			}

			tile.setPosition({ row * tileWidth, col * tileHeight });

			m_window.draw(tile);
		}
	}

	//draw player
	sf::Vector2f playerPos = t_player.getPosition();

	sf::RectangleShape player;
	player.setSize({ tileWidth, tileHeight });
	player.setFillColor(sf::Color::Cyan);
	player.setPosition({ playerPos.x * tileWidth, playerPos.y * tileHeight });

	m_window.draw(player);

	//draw enemies
	sf::RectangleShape enemyShape;
	enemyShape.setSize({ tileWidth, tileHeight });
	enemyShape.setFillColor(sf::Color::Red);

	for (const Enemy& enemy : t_level.getEnemies())
	{
		sf::Vector2f enemyPos = enemy.getPosition();
		enemyShape.setPosition({ enemyPos.x * tileWidth, enemyPos.y * tileHeight });

		m_window.draw(enemyShape);
	}
}

bool Window::isOpen() const
{
	return m_window.isOpen();
}

void Window::close()
{
	m_window.close();
}

sf::RenderWindow& Window::getWindow()
{
	return m_window;
}
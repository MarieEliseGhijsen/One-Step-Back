
#include "Game.h"

Game::Game() : m_window{}, m_DELETEexitGame{ false } //when true game will exit
{
	m_level.loadLevel(1);
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);

	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
	}
}

void Game::processEvents()
{
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if (newEvent->is<sf::Event::Closed>())
		{
			m_DELETEexitGame = true;
		}
		else if (newEvent->is<sf::Event::KeyPressed>())
		{
			processKeys(newEvent);
		}
		else if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			processMouseClick(newEvent);
		}
	}
}

void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed* newKeypress = t_event->getIf<sf::Event::KeyPressed>();

	//deal with key presses
}

void Game::processMouseClick(const std::optional<sf::Event> t_event)
{
	auto mouseClick = t_event->getIf<sf::Event::MouseButtonPressed>();

	//deal with button clicks
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_DELETEexitGame)
	{
		m_window.close();
	}

	m_player.handleInput(t_deltaTime.asMilliseconds(), m_level);
	m_level.update();

	m_window.renderGameRunning(m_player, m_level);
}
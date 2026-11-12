
#include "../include/Game.h"

Game::Game() : m_window{}, m_DELETEexitGame{ false } //when true game will exit
{
	m_level.loadLevel(1);
	m_player.setPosition(m_level.getStartPosition());
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

bool Game::processEvents()
{
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if (newEvent->is<sf::Event::Closed>())
		{
			m_DELETEexitGame = true;
		}
		else if (newEvent->is<sf::Event::KeyPressed>())
		{
			if(processKeys(newEvent)) return true;
		}
	}

	return false;
}

bool Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed* keypress = t_event->getIf<sf::Event::KeyPressed>();

	if (keypress->code == sf::Keyboard::Key::Up && m_level.isWalkable(m_player.getPosition() + sf::Vector2f{0.0f, -1.0f}))
	{
		m_player.update(Globals::UP);
		return true;
	}
	else if (keypress->code == sf::Keyboard::Key::Down && m_level.isWalkable(m_player.getPosition() + sf::Vector2f{ 0.0f, 1.0f }))
	{
		m_player.update(Globals::DOWN);
		return true;
	}
	else if (keypress->code == sf::Keyboard::Key::Left && m_level.isWalkable(m_player.getPosition() + sf::Vector2f{ -1.0f, 0.0f }))
	{
		m_player.update(Globals::LEFT);
		return true;
	}
	else if (keypress->code == sf::Keyboard::Key::Right && m_level.isWalkable(m_player.getPosition() + sf::Vector2f{ 1.0f, 0.0f }))
	{
		m_player.update(Globals::RIGHT);
		return true;
	}

	return false;
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

	while (true)
	{
		if (processEvents()) break;
	}

	if (m_level.update(m_player.getPosition()))
	{
		//std::cout << "Player reached end point - closing game" << "\n";
		m_DELETEexitGame = true;
	}

	m_window.renderGameRunning(m_player, m_level);
}
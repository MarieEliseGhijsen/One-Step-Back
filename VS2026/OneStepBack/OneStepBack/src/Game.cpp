
#include "../include/Game.h"

Game::Game() : m_window{}, m_DELETEexitGame{ false } //when true game will exit
{
	m_gameState = States::GAME_RUNNING;
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

    if (keypress == nullptr)
    {
        return false;
    }

    sf::Vector2f playerPosition = m_player.getPosition();

	if (keypress->code == sf::Keyboard::Key::Up && m_level.isWalkable(playerPosition + sf::Vector2f{ 0.0f, -1.0f }))
    {
        playTurn(Globals::UP);
        return true;
    }
    else if (keypress->code == sf::Keyboard::Key::Down && m_level.isWalkable(playerPosition + sf::Vector2f{ 0.0f, 1.0f }))
    {
        playTurn(Globals::DOWN);
        return true;
    }
    else if (keypress->code == sf::Keyboard::Key::Left && m_level.isWalkable(playerPosition + sf::Vector2f{ -1.0f, 0.0f }))
    {
        playTurn(Globals::LEFT);
        return true;
    }
    else if (keypress->code == sf::Keyboard::Key::Right && m_level.isWalkable(playerPosition + sf::Vector2f{ 1.0f, 0.0f }))
    {
        playTurn(Globals::RIGHT);
        return true;
    }
	else if (keypress->code == sf::Keyboard::Key::R)
	{
		if (m_player.rewind())
		{
			m_level.update(m_player.getPosition());
			return true;
		}
		else
		{
			processEvents();
		}
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

	switch (m_gameState)
	{
	case States::GAME_MENU:
		m_window.renderMenu();
		break;

	case States::GAME_INSTRUCTIONS:
		m_window.renderInstructions();
		break;

	case States::GAME_SETTINGS:
		m_window.renderSettings();
		break;

	case States::GAME_RUNNING:
		m_window.renderGameRunning(m_player, m_level);
		break;

	case States::GAME_PAUSE:
		break;

	case States::GAME_WIN:
		m_window.renderWin();
		break;

	case States::GAME_LOSE:
		m_window.renderLose();
		break;
	}
}

void Game::playTurn(int t_direction)
{
    m_player.update(t_direction);

    if (m_level.update(m_player.getPosition()))
    {
        m_DELETEexitGame = true;
    }
}
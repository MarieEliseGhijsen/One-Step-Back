
#include "../include/Game.h"

Game::Game() : m_window{}, m_gameState{ States::GAME_MENU }, m_currentLevel{ 1 }, m_totalLevels{ 5 }, m_DELETEexitGame{ false } //when true game will exit
{
	loadLeaderboard();
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
			processKeys(newEvent);
		}
		else if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			processMouseClick(newEvent);
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

	if (m_gameState != States::GAME_RUNNING)
	{
		return false;
	}

	if (keypress->code == sf::Keyboard::Key::P)
	{
		m_gameState = States::GAME_PAUSE;
		return true;
	}

    sf::Vector2f playerPosition = m_player.getPosition();

	if (keypress->code == sf::Keyboard::Key::Up && m_level.isWalkable(playerPosition + sf::Vector2f{ 0.0f, -1.0f }))
    {
        playTurn(Globals::UP);
    }
    else if (keypress->code == sf::Keyboard::Key::Down && m_level.isWalkable(playerPosition + sf::Vector2f{ 0.0f, 1.0f }))
    {
        playTurn(Globals::DOWN);
    }
    else if (keypress->code == sf::Keyboard::Key::Left && m_level.isWalkable(playerPosition + sf::Vector2f{ -1.0f, 0.0f }))
    {
        playTurn(Globals::LEFT);
    }
    else if (keypress->code == sf::Keyboard::Key::Right && m_level.isWalkable(playerPosition + sf::Vector2f{ 1.0f, 0.0f }))
    {
        playTurn(Globals::RIGHT);
    }
	else if (keypress->code == sf::Keyboard::Key::R)
	{
		sf::Vector2f previousPosition = m_player.getPosition();

		if (m_player.rewind(m_level))
		{
			m_level.handlePlayerEntry(previousPosition, m_player.getPosition());
			bool gameEnded = m_level.update(m_player.getPosition());

			if (gameEnded)
			{
				bool caught = false;

				for (const Enemy& enemy : m_level.getEnemies())
				{
					if (enemy.getPosition() == m_player.getPosition())
					{
						caught = true;
						break;
					}
				}

				if (caught)
				{
					m_gameState = States::GAME_LOSE;
				}
				else
				{
					m_gameState = States::GAME_WIN;

					if (m_currentLevel == m_totalLevels)
					{
						addLeaderboardScore(m_level.getTotalScore());
					}
				}
			}
		}
	}

	return false;
}

void Game::processMouseClick(const std::optional<sf::Event> t_event)
{
	auto mouseClick = t_event->getIf<sf::Event::MouseButtonPressed>();

	if (mouseClick == nullptr || mouseClick->button != sf::Mouse::Button::Left)
	{
		return;
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window.getWindow());
	MenuButton clickedButton = m_window.getMenuButtonAt(mousePosition);
	handleButtonClick(clickedButton);
}

void Game::handleButtonClick(MenuButton t_button)
{
	switch (m_gameState)
	{
	case States::GAME_MENU:
		switch (t_button)
		{
		case MenuButton::PLAY:
			startLevel(1);
			break;
		case MenuButton::LEVEL_SELECT:
			m_gameState = States::GAME_LEVEL_SELECT;
			break;
		case MenuButton::INSTRUCTIONS:
			m_gameState = States::GAME_INSTRUCTIONS;
			break;
		case MenuButton::LEADER_BOARD:
			m_gameState = States::GAME_LEADER_BOARD;
			break;
		case MenuButton::QUIT:
			m_DELETEexitGame = true;
			break;
		default:
			break;
		}

		break;

	case States::GAME_INSTRUCTIONS:
		if (t_button == MenuButton::BACK)
		{
			m_gameState = States::GAME_MENU;
		}

		break;

	case States::GAME_LEVEL_SELECT:
		switch (t_button)
		{
		case MenuButton::LEVEL_1:
			startLevel(1);
			break;
		case MenuButton::LEVEL_2:
			startLevel(2);
			break;
		case MenuButton::LEVEL_3:
			startLevel(3);
			break;
		case MenuButton::LEVEL_4:
			startLevel(4);
			break;
		case MenuButton::LEVEL_5:
			startLevel(5);
			break;
		case MenuButton::BACK:
			m_gameState = States::GAME_MENU;
			break;
		default:
			break;
		}

		break;

	case States::GAME_LEADER_BOARD:
		if (t_button == MenuButton::BACK)
		{
			m_gameState = States::GAME_MENU;
		}

		break;

	case States::GAME_PAUSE:
		switch (t_button)
		{
		case MenuButton::CONTINUE:
			m_gameState = States::GAME_RUNNING;
			break;
		case MenuButton::MAIN_MENU:
			m_gameState = States::GAME_MENU;
			break;
		case MenuButton::QUIT:
			m_DELETEexitGame = true;
			break;
		default:
			break;
		}

		break;

	case States::GAME_WIN:
		switch (t_button)
		{
		case MenuButton::NEXT_LEVEL:
			if (m_currentLevel < m_totalLevels)
			{
				startLevel(m_currentLevel + 1);
			}
			break;
		case MenuButton::MAIN_MENU:
			m_gameState = States::GAME_MENU;
			break;
		case MenuButton::QUIT:
			m_DELETEexitGame = true;
			break;
		default:
			break;
		}

		break;

	case States::GAME_LOSE:

		switch (t_button)
		{
		case MenuButton::TRY_AGAIN:
			startLevel(m_currentLevel);
			break;
		case MenuButton::MAIN_MENU:
			m_gameState = States::GAME_MENU;
			break;
		case MenuButton::QUIT:
			m_DELETEexitGame = true;
			break;
		default:
			break;
		}

		break;

	default:
		break;
	}
}

void Game::startLevel(int t_levelNumber)
{
	if (t_levelNumber < 1 || t_levelNumber > m_totalLevels)
	{
		return;
	}

	m_currentLevel = t_levelNumber;
	m_level.loadLevel(m_currentLevel);
	m_player.setPosition(m_level.getStartPosition());
	m_gameState = States::GAME_RUNNING;
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_DELETEexitGame)
	{
		m_window.close();
		return;
	}
	
	renderCurrentState();
}

void Game::playTurn(int t_direction)
{
	sf::Vector2f previousPosition = m_player.getPosition();
	m_player.update(t_direction);
	m_level.handlePlayerEntry(previousPosition, m_player.getPosition());

	bool gameEnded = m_level.update(m_player.getPosition());

	if (gameEnded)
	{
		bool caught = false;

		for (const Enemy& enemy : m_level.getEnemies())
		{
			if (enemy.getPosition() == m_player.getPosition())
			{
				caught = true;
				break;
			}
		}

		if (caught)
		{
			m_gameState = States::GAME_LOSE;
		}
		else
		{
			m_gameState = States::GAME_WIN;

			if (m_currentLevel == m_totalLevels)
			{
				addLeaderboardScore(m_level.getTotalScore());
			}
		}
	}
}

void Game::loadLeaderboard()
{
	m_leaderboardScores.clear();

	std::ifstream file("Resources/leaderboard.txt");
	int score;

	while (file >> score)
	{
		m_leaderboardScores.push_back(score);
	}

	std::sort(m_leaderboardScores.begin(), m_leaderboardScores.end(), std::greater<int>());

	if (m_leaderboardScores.size() > 10)
	{
		m_leaderboardScores.resize(10);
	}
}

void Game::addLeaderboardScore(int t_score)
{
	m_leaderboardScores.push_back(t_score);
	std::sort(m_leaderboardScores.begin(), m_leaderboardScores.end(), std::greater<int>());

	if (m_leaderboardScores.size() > 10)
	{
		m_leaderboardScores.resize(10);
	}

	saveLeaderboard();
}

void Game::saveLeaderboard()
{
	std::ofstream file("Resources/leaderboard.txt");

	if (!file.is_open())
	{
		std::cout << "Could not save leaderboard" << "\n";
		return;
	}

	for (int score : m_leaderboardScores)
	{
		file << score << "\n";
	}
}

void Game::renderCurrentState()
{
	switch (m_gameState)
	{
	case States::GAME_MENU:
		m_window.renderMenu();
		break;

	case States::GAME_INSTRUCTIONS:
		m_window.renderInstructions();
		break;

	case States::GAME_LEVEL_SELECT:
		m_window.renderLevelSelect();
		break;

	case States::GAME_LEADER_BOARD:
		m_window.renderLeaderboard(m_leaderboardScores);
		break;

	case States::GAME_RUNNING:
		m_window.renderGameRunning(m_player, m_level);
		break;

	case States::GAME_PAUSE:
		m_window.renderPause();
		break;

	case States::GAME_WIN:
		m_window.renderWin(m_currentLevel);
		break;

	case States::GAME_LOSE:
		m_window.renderLose(m_currentLevel);
		break;
	}
}
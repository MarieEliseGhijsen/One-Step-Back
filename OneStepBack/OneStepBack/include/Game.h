#ifndef GAME_HPP
#define GAME_HPP

#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Window.h"
#include "Level.h"
#include "Player.h"

enum class States
{
	GAME_MENU, 
	GAME_INSTRUCTIONS, 
	GAME_LEVEL_SELECT,
	GAME_LEADER_BOARD,
	GAME_RUNNING, 
	GAME_PAUSE, 
	GAME_WIN, 
	GAME_LOSE
};

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void update(sf::Time t_deltaTime);

	bool processEvents();
	bool processKeys(const std::optional<sf::Event> t_event);
	void processMouseClick(const std::optional<sf::Event> t_event);

	void startLevel(int t_levelNumber);
	void handleButtonClick(MenuButton t_button);

	void playTurn(int t_direction);

	void loadLeaderboard();
	void saveLeaderboard();
	void addLeaderboardScore(int t_score);

	void renderCurrentState();

	Window m_window;
	Level m_level;
	Player m_player;

	States m_gameState{ States::GAME_RUNNING };

	int m_currentLevel{ 1 };
    int m_totalLevels{ 5 };

	std::vector<int> m_leaderboardScores;

	bool m_DELETEexitGame; //control exiting game
};

#pragma warning( pop )
#endif // !GAME_HPP
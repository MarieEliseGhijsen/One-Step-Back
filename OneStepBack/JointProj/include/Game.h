#ifndef GAME_HPP
#define GAME_HPP

#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include "Window.h"
#include "Level.h"

#include "Player.h"

enum class States
{
	GAME_MENU, GAME_INSTRUCTIONS, GAME_SETTINGS, GAME_RUNNING, GAME_PAUSE, GAME_WIN, GAME_LOSE
};

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void update(sf::Time t_deltaTime);

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void processMouseClick(const std::optional<sf::Event> t_event);

	sf::Font m_jerseyFont;

	Window m_window;
	Level m_level;
	States m_gameState{ States::GAME_RUNNING };

	Player m_player;
	
	bool m_DELETEexitGame; //control exiting game
};

#pragma warning( pop )
#endif // !GAME_HPP
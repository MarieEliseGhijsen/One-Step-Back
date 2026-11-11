#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include "Globals.h"

#include "Level.h"

#include "Player.h"

class Window
{
public:
	Window();

	std::optional<sf::Event> pollEvent();

	void renderMenu();
	void renderInstructions();
	void renderSettings();

	void renderGameRunning(const Player& t_player, const Level& t_level);
	void renderLevel(const Player& t_player, const Level& t_level);

	void renderWin();
	void renderLose();

	bool isOpen() const;
	void close();

	sf::RenderWindow &getWindow();
										
private:
	sf::RenderWindow m_window;

	sf::Font m_font;
};
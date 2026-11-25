#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

#include "Globals.h"

#include "Level.h"
#include "Player.h"

enum class MenuButton
{
    NONE,

    PLAY,
    LEVEL_SELECT,
    INSTRUCTIONS,
	LEADER_BOARD,
    QUIT,

    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5,
    BACK,

    CONTINUE,
    MAIN_MENU,

    NEXT_LEVEL,
    TRY_AGAIN
};

class Window
{
public:
	Window();

	std::optional<sf::Event> pollEvent();

	//menu screens
	void renderMenu();
	void renderInstructions();
	void renderLevelSelect();
	void renderLeaderboard(const std::vector<int>& t_scores);

	//gameplay
	void renderGameRunning(const Player& t_player, const Level& t_level);
	void renderLevel(const Player& t_player, const Level& t_level);
	void renderPause();
    void renderWin(int t_level);
    void renderLose(int t_level);

    void updateAnimations(sf::Time t_deltaTime);

	MenuButton getMenuButtonAt(sf::Vector2i t_mousePosition) const;

	bool isOpen() const;
	void close();

	sf::RenderWindow& getWindow();

private:
    void loadTextures();

    void drawTile(int t_tileType, sf::Vector2f t_position, sf::Vector2f t_size);
	void drawAnimatedSprite(sf::Texture& t_texture, sf::Vector2f t_position, sf::Vector2f t_size, int t_frame);

	void drawText( const std::string& t_text, unsigned int t_characterSize, sf::Vector2f t_position, sf::Color t_colour = sf::Color::White);

	void clearButtons();
	void drawButton(MenuButton t_button, const std::string& t_text, float t_yPosition);

	sf::RenderWindow m_window;

	sf::Font m_font;
    bool m_fontLoaded{ false };

	std::map<MenuButton, sf::FloatRect> m_buttonBounds;

	//textures
    sf::Texture m_floorTexture;
    sf::Texture m_wallTexture;
    sf::Texture m_unlockedDoorTexture;
    sf::Texture m_lockedDoorTexture;
    sf::Texture m_keyTexture;
    sf::Texture m_coinTexture;

	//spritesheet textures
    sf::Texture m_flagTexture;
    sf::Texture m_playerTexture;
    sf::Texture m_enemyTexture;

	//animation varibles
    int m_currentFrame = 0;
    float m_animationTimer = 0.0f;
    float m_frameDuration = 0.15f; //frame speed adjuster
};
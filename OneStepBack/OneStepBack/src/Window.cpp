#include "../include/Window.h"

Window::Window() : m_window(sf::VideoMode({ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT }), Globals::WINDOW_TITLE)
{
	m_window.setVerticalSyncEnabled(true);

	loadTextures();

	m_fontLoaded = m_font.openFromFile("Resources/ASSETS/FONTS/Jersey20-Regular.ttf");
	
	if (!m_fontLoaded)
    {
        m_fontLoaded = m_font.openFromFile("Resources/ASSETS/FONTS/Jersey20-Regular.ttf");
    }

	if (!m_fontLoaded)
    {
        std::cout << "WARNING: Could not load game font.\n";
        std::cout << "Place Jersey15-Regular.ttf in Resources/Fonts.\n";
    }
}

std::optional<sf::Event> Window::pollEvent()
{
	return m_window.pollEvent();
}

void Window::drawText(const std::string& t_text, unsigned int t_characterSize, sf::Vector2f t_position, sf::Color t_colour)
{
    if (!m_fontLoaded) return;

    sf::Text text(m_font, t_text, t_characterSize);
    text.setFillColor(t_colour);
    text.setPosition(t_position);

    m_window.draw(text);
}

void Window::loadTextures()
{
    auto load = [](sf::Texture& texture, const std::string& path)
    {
        if (!texture.loadFromFile(path))
        {
            std::cout << "Could not open : " << path << "\n";
            return false;
        }

        texture.setSmooth(false); //stops sprites from blurring
        return true;
    };

    load(m_floorTexture, "Resources/ASSETS/IMAGES/FLOOR.png");
    load(m_wallTexture, "Resources/ASSETS/IMAGES/WALL.png");
    load(m_unlockedDoorTexture, "Resources/ASSETS/IMAGES/DOOR_UNLOCKED.png");
    load(m_lockedDoorTexture, "Resources/ASSETS/IMAGES/DOOR_LOCKED.png");
    load(m_keyTexture, "Resources/ASSETS/IMAGES/KEY.png");
    load(m_coinTexture, "Resources/ASSETS/IMAGES/COIN.png");
    load(m_flagTexture, "Resources/ASSETS/IMAGES/FLAG_SPRITESHEET.png");
    load(m_playerTexture, "Resources/ASSETS/IMAGES/PURP_SPRITESHEET.png");
    load(m_enemyTexture, "Resources/ASSETS/IMAGES/GOOBER_SPRITESHEET.png");
}

void Window::drawButton(MenuButton t_button, const std::string& t_text, float t_yPosition)
{
    const float buttonWidth = 300.0f;
    const float buttonHeight = 55.0f;

    const float xPosition = 250.0f;

    sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
    button.setPosition({ xPosition, t_yPosition });
    button.setFillColor(sf::Color(48, 57, 75));
    button.setOutlineThickness(2.0f);
    button.setOutlineColor(sf::Color(255, 190, 70));

    m_window.draw(button);

    m_buttonBounds[t_button] = sf::FloatRect( { xPosition, t_yPosition }, { buttonWidth, buttonHeight } );

    if (m_fontLoaded)
    {
        sf::Text buttonText(m_font, t_text, 25);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition({ xPosition + 25.0f, t_yPosition + 12.0f });

        m_window.draw(buttonText);
    }
}

MenuButton Window::getMenuButtonAt(sf::Vector2i t_mousePosition) const
{
    sf::Vector2f mousePosition(static_cast<float>(t_mousePosition.x), static_cast<float>(t_mousePosition.y));

    for (const auto& button : m_buttonBounds)
    {
        if (button.second.contains(mousePosition))
        {
            return button.first;
        }
    }

    return MenuButton::NONE;
}

void Window::clearButtons()
{
    m_buttonBounds.clear();
}

//Menu Functions
void Window::renderMenu()
{
	clearButtons();

	m_window.clear();

	drawButton(MenuButton::PLAY, "PLAY", 200.0f);
	drawButton(MenuButton::LEVEL_SELECT, "LEVEL SELECTION", 275.0f);
	drawButton(MenuButton::INSTRUCTIONS, "INSTRUCTIONS", 350.0f);
	drawButton(MenuButton::LEADER_BOARD, "LEADERBOARD", 425.0f);
	drawButton(MenuButton::QUIT, "QUIT", 500.0f);

	m_window.display();
}

void Window::renderLevelSelect()
{
    clearButtons();

	m_window.clear();

	drawButton(MenuButton::LEVEL_1, "LEVEL 1", 220.0f);
    drawButton(MenuButton::LEVEL_2, "LEVEL 2", 290.0f);
    drawButton(MenuButton::LEVEL_3, "LEVEL 3", 360.0f);
    drawButton(MenuButton::LEVEL_4, "LEVEL 4", 430.0f);
    drawButton(MenuButton::LEVEL_5, "LEVEL 5", 500.0f);

    drawButton(MenuButton::BACK, "BACK", 590.0f);
	m_window.display();
}

void Window::renderInstructions()
{
    clearButtons();

	m_window.clear();

	drawText("OBJECTIVE", 25, { 100.0f, 190.0f }, sf::Color(255, 190, 70));
    drawText("Explore the abandoned environment and reach the exit.", 17, { 100.0f, 230.0f });
    drawText("Avoid enemies and navigate around walls and locked doors.",17, { 100.0f, 260.0f });
    drawText("Collect keys to unlock locked doors.", 17, { 100.0f, 290.0f });
    
	drawText("CONTROLS", 25, { 100.0f, 365.0f }, sf::Color(255, 190, 70));
    drawText("UP ARROW       - Move up", 18, { 100.0f, 410.0f });
    drawText("DOWN ARROW     - Move down", 18, { 100.0f, 445.0f });
    drawText("LEFT ARROW     - Move left", 18,{ 100.0f, 480.0f });
    drawText("RIGHT ARROW    - Move right",18,{ 100.0f, 515.0f });
    drawText("R              - Rewind previous moves",18, { 100.0f, 550.0f });
    drawText("P         - Pause the game",18, { 100.0f, 585.0f });

    drawButton(MenuButton::BACK, "BACK TO MENU", 670.0f);

	m_window.display();
}

void Window::renderLeaderboard(const std::vector<int>& t_scores)
{
	clearButtons();

	m_window.clear();

	drawText("LEADERBOARD", 40, { 260.0f, 100.0f }, sf::Color(255, 190, 70));

	for (int i = 0; i < 10; i++)
	{
		std::string scoreText;

		if (i < t_scores.size())
		{
			scoreText = std::to_string(i + 1) + ".    " + std::to_string(t_scores[i]);
		}
		else
		{
			scoreText = std::to_string(i + 1) + ".    ---";
		}

		drawText(scoreText, 25, { 300.0f, 180.0f + (i * 40.0f) });
	}

	drawButton(MenuButton::BACK, "BACK TO MENU", 620.0f);

	m_window.display();
}

//game pause scren
void Window::renderPause()
{
	clearButtons();

	m_window.clear();

	drawText("GAME PAUSED", 35, { 285.0f, 170.0f }, sf::Color(255, 190, 70));
	
	drawButton(MenuButton::CONTINUE, "CONTINUE", 300.0f);
	drawButton(MenuButton::MAIN_MENU, "MAIN MENU", 390.0f);

	m_window.display();
}


//level ened screens
void Window::renderWin(int t_level)
{
	clearButtons();

	m_window.clear();

	drawText("LEVEL COMPLETED!", 35, { 260.0f, 190.0f }, sf::Color(100, 230, 130));
	drawText("Congratulations! You completed level " + std::to_string(t_level), 20, { 205.0f, 250.0f });

	if (t_level < 5)
	{
		drawButton(MenuButton::NEXT_LEVEL, "NEXT LEVEL", 330.0f);
	}
	else
	{
		drawText("You completed all five levels!", 20, { 245.0f, 330.0f }, sf::Color(255, 190, 70));
	}

	drawButton(MenuButton::MAIN_MENU, "MAIN MENU", 420.0f);

	m_window.display();
}

void Window::renderLose(int t_level)
{
	clearButtons();

	m_window.clear();

	drawText("GAME OVER", 35, { 320.0f, 190.0f }, sf::Color(240, 100, 100)); 

	drawButton(MenuButton::TRY_AGAIN, "RESTART", 330.0f);
	drawButton(MenuButton::MAIN_MENU, "MAIN MENU", 420.0f);

	m_window.display();
}

//game running render functionss
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

    sf::Vector2f tileSize(tileWidth, tileHeight);

    for (int y = 0; y < t_level.getHeight(); y++)
    {
        for (int x = 0; x < t_level.getWidth(); x++)
        {
            int tileType = t_level.getTileType(x, y);
            sf::Vector2f position(x * tileWidth, y * tileHeight);

            drawTile(tileType, position, tileSize);
        }
    }

    sf::Vector2f playerPos = t_player.getPosition();
    drawAnimatedSprite(m_playerTexture, { playerPos.x * tileWidth, playerPos.y * tileHeight }, tileSize, m_currentFrame);

    for (const Enemy& enemy : t_level.getEnemies())
    {
        sf::Vector2f enemyPos = enemy.getPosition();
        drawAnimatedSprite(m_enemyTexture, { enemyPos.x * tileWidth, enemyPos.y * tileHeight }, tileSize, m_currentFrame);
    }
}

void Window::drawTile(int t_tileType, sf::Vector2f t_position, sf::Vector2f t_size)
{
    sf::Sprite floorSprite(m_floorTexture);
    sf::Sprite sprite(m_floorTexture);
	sf::RectangleShape error(t_size);

    sprite.setPosition(t_position);
    sprite.setScale({t_size.x / 32.0f, t_size.y / 32.0f});

    if (m_floorTexture.getSize().x > 0)
    {
        floorSprite.setPosition(t_position);
        floorSprite.setScale({t_size.x / 32.0f, t_size.y / 32.0f});

        m_window.draw(floorSprite);
    }
    else
    {
        error.setPosition(t_position);
        error.setFillColor(sf::Color::Magenta);

        m_window.draw(error);
        return;
    }

    sf::Texture* texture = nullptr;

    switch (t_tileType)
    {
    case 0:
    case 8:
        return;
    case 1:
        texture = &m_wallTexture;
        break;
    case 3:
        texture = &m_unlockedDoorTexture;
        break;
    case 4:
        texture = &m_lockedDoorTexture;
        break;
    case 5:
        texture = &m_keyTexture;
        break;
    case 6:
        texture = &m_coinTexture;
        break;
    case 9:
        texture = nullptr;
        break;
    default:
    	{
        	error.setPosition(t_position);
        	error.setFillColor(sf::Color::Magenta);

        	m_window.draw(error);
        	return;
    	}
    }

    if (t_tileType == 9)
    {
        drawAnimatedSprite(m_flagTexture, t_position, t_size, m_currentFrame);
        return;
    }

    if (texture == nullptr || texture->getSize().x == 0)
    {
        error.setPosition(t_position);
        error.setFillColor(sf::Color::Magenta);

        m_window.draw(error);
        return;
    }

    sf::Sprite object(*texture);
    object.setPosition(t_position);
    object.setScale({t_size.x / 32.0f, t_size.y / 32.0f});

    m_window.draw(object);
}

void Window::updateAnimations(sf::Time t_deltaTime)
{
    m_animationTimer += t_deltaTime.asSeconds();

    if (m_animationTimer >= m_frameDuration)
    {
        m_animationTimer = 0.0f;
        m_currentFrame++;

        if (m_currentFrame >= 4)
        {
            m_currentFrame = 0;
        }
    }
}

void Window::drawAnimatedSprite(sf::Texture& t_texture, sf::Vector2f t_position, sf::Vector2f t_size, int t_frame)
{
        sf::RectangleShape error(t_size);

    if (t_texture.getSize().x < 128 || t_texture.getSize().y < 32)
    {
        error.setPosition(t_position);
        error.setFillColor(sf::Color::Magenta);

        m_window.draw(error);
        return;
    }

    sf::Sprite sprite(t_texture);
    sprite.setTextureRect(sf::IntRect({ t_frame * 32, 0 }, { 32, 32 }));
    sprite.setPosition(t_position);
    sprite.setScale({t_size.x / 32.0f, t_size.y / 32.0f});

    m_window.draw(sprite);
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
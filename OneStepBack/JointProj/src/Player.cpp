#include "../include/Player.h"

void Player::update()
{
}

void Player::handleInput(float t_dt, const Level& t_level)
{
	float dtSeconds = t_dt / 1000.0f;

	sf::Vector2f inputVector{ 0.f, 0.f };
	m_forward = { cos(m_angle), sin(m_angle) };
	m_strafe = { cos(m_angle + 3.14159f / 2), sin(m_angle + 3.14159f / 2) };

	//forwrd and back movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) inputVector = inputVector + m_forward;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) inputVector = inputVector - m_forward;

	//side straffing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) inputVector = inputVector - m_strafe;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) inputVector = inputVector + m_strafe;

	//normalize - fix diagonal movement
	float len = std::sqrt(inputVector.x * inputVector.x + inputVector.y * inputVector.y);
	if (len > 0.f) inputVector /= len;

	//clamp speed
	m_velocity = m_velocity + (inputVector * m_acceleration * dtSeconds);
	float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
	if (speed > m_MAX_SPEED) m_velocity = (m_velocity / speed) * m_MAX_SPEED;

	//adding friction
	float appliedFriction = std::pow(m_friction, dtSeconds * 60.0f);
	m_velocity = m_velocity * appliedFriction;

	//checking new position agaisnt level boundary
	sf::Vector2f newPos = m_position + (m_velocity * dtSeconds);
	sf::Vector2f newPosX = { newPos.x, m_position.y };
	sf::Vector2f newPosY = { m_position.x, newPos.y };

	if (t_level.getTileType((int)newPosX.x, (int)newPosX.y) != 1) m_position.x = newPosX.x; 
	else m_velocity.x = 0.0f; 

	if (t_level.getTileType((int)newPosY.x, (int)newPosY.y) != 1) m_position.y = newPosY.y;
	else m_velocity.y = 0.0f; 

	//rotating
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) m_angle = m_angle - (m_ROTATION_SPEED * dtSeconds);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) m_angle = m_angle + (m_ROTATION_SPEED * dtSeconds);
}

sf::Vector2f Player::getPosition() const
{
	return m_position;
}

float Player::getAngle() const
{
	return m_angle;
}
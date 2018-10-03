// Project Includes
#include "Player.h"
#include "AssetManager.h"

// Constants
#define JUMP_SPEED -1500.0f
#define GRAVITY 2000.0f
#define RUN_SPEED_INITIAL 500.0f

// Constructor
Player::Player()
	: m_sprite ()
	, m_jumpSound ()
	, m_deathSound ()
	, m_landSound ()
	, m_animation ()
	, m_velocity (0.0f, 0.0f)
{
	// Sprite Setup
	m_sprite.setTexture(AssetManager::GetTexture("graphics/playerJump.png"));

	// Sound Setup
	m_jumpSound.setBuffer(AssetManager::GetSoundBuffer("audio/jump.wav"));
	m_deathSound.setBuffer(AssetManager::GetSoundBuffer("audio/death.wav"));
	m_landSound.setBuffer(AssetManager::GetSoundBuffer("audio/landing.wav"));

	// Set up animation
	m_animation.SetSprite(m_sprite);

	Animation& runAnimation = m_animation.CreateAnimation("run");
	runAnimation.AddFrame(AssetManager::GetTexture("graphics/playerRun1.png"));
	runAnimation.AddFrame(AssetManager::GetTexture("graphics/playerRun2.png"));
	runAnimation.SetLoop(true);
	runAnimation.SetPlayBackSpeed(10.0f);

	Animation& jumpAnimation = m_animation.CreateAnimation("jump");
	jumpAnimation.AddFrame(AssetManager::GetTexture("graphics/playerJump.png"));

	m_animation.Play("jump");
}

void Player::Input(sf::Event _gameEvent)
{
	// Check if the player has pressed the jump button.
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Check if the button was space
		if (_gameEvent.key.code == sf::Keyboard::Space)
		{
			// Player has tried to jump!

			// Play jump sound
			m_jumpSound.play();

			// Set the player's upward velocity
			m_velocity.y = JUMP_SPEED;
		}
	}
}

void Player::Update(sf::Time _frameTime)
{
	// Process the animation
	m_animation.Update(_frameTime);

	// Apply gravity to our velocity
	float velocityChange = GRAVITY * _frameTime.asSeconds();
	m_velocity.y += velocityChange;

	// Move sprite based on velocity
	sf::Vector2f currentPosition = m_sprite.getPosition();
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();
	m_sprite.setPosition(currentPosition + positionChange);
}


void Player::Draw(sf::RenderTarget& _target)
{
	_target.draw(m_sprite);
}

void Player::Spawn()
{
	m_sprite.setPosition(0, 0);
	m_animation.Play("jump");
	m_velocity.y = 0;
	m_velocity.x = RUN_SPEED_INITIAL;
}
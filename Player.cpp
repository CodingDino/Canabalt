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
	, m_touchingGround (false)
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

			// Only allow jump if they are touching the ground
			if (m_touchingGround == true)
			{
				// Play jump sound
				m_jumpSound.play();

				// Play jump animation
				m_animation.Play("jump");

				// Set the player's upward velocity
				m_velocity.y = JUMP_SPEED;
			}
		}
	}
}

void Player::Update(sf::Time _frameTime)
{
	// Process the animation
	m_animation.Update(_frameTime);

	// Apply gravity to our velocity
	if (m_touchingGround == false)
	{
		float velocityChange = GRAVITY * _frameTime.asSeconds();
		m_velocity.y += velocityChange;
	}

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

sf::Vector2f Player::GetPosition()
{
	return m_sprite.getPosition();
}

void Player::HandleCollision(std::vector<sf::FloatRect> _platforms)
{
	// Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;

	// Assume we did not collide
	m_touchingGround = false;

	// Get the collider for the player
	sf::FloatRect playerCollider = m_sprite.getGlobalBounds();

	// Loop through each platform collider
	for (auto it = _platforms.begin(); it != _platforms.end(); ++it)
	{
		// Go to iterator pointer and get rect
		sf::FloatRect platformCollider = *it;

		// Does our sprite intersect the platform?
		if (playerCollider.intersects(platformCollider))
		{
			// Yes it intersects!

			// Check if the bottom of our feet is touching the 
			// top of the platform

			// Create feet collider
			sf::FloatRect feetCollider = playerCollider;
			// Set our feet top to be 10 pixels from the bottom of the player collider
			feetCollider.top += playerCollider.height - 10;
			// Set our feet collider height to be 10 pixels
			feetCollider.height = 10;

			// Create platform top collider
			sf::FloatRect platformTop = platformCollider;
			platformTop.height = 10;

			// Are the feet touching the top of the platform?
			if (feetCollider.intersects(platformTop))
			{
				// Yes! Feet are touching

				// Record that we're touching the ground
				m_touchingGround = true;

				// Check if we are falling downward 
				// and not already touching the ground
				if (wereTouchingGround == false && m_velocity.y > 0)
				{
					// We have just now touched the ground!!!!!
					m_animation.Play("run");
					m_landSound.play();
					m_velocity.y = 0;
				}
			}
		}
	}


	// If we aren't touching the ground, 
	// and we were before
	// play the jump animation (falling)
	if (m_touchingGround == false && wereTouchingGround == true)
	{
		m_animation.Play("jump");
	}
}
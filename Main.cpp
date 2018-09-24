// --------------------------------------
// Includes
// --------------------------------------
// Libraries
#include <SFML/Graphics.hpp>

// Project Includes
#include "AssetManager.h"

int main()
{
	// --------------------------------------
	// Game Setup
	// --------------------------------------

	// Render Window creation
	sf::RenderWindow gameWindow;
	gameWindow.create(sf::VideoMode::getDesktopMode(), "Canabalt",
		sf::Style::Titlebar | sf::Style::Close);

	// Timer functionality
	sf::Clock gameClock;

	// Create AssetManager
	AssetManager assets;

	// Testing AssetManager
	sf::Sprite testSprite;
	testSprite.setTexture(AssetManager::GetTexture("graphics/playerJump.png"));

	// end game setup
	// --------------------------------------


	// --------------------------------------
	// Game Loop
	// --------------------------------------
	while (gameWindow.isOpen())
	{
		// --------------------------------------
		// Input
		// --------------------------------------
		sf::Event event;
		while (gameWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				gameWindow.close();
			} // End if (closed)
		} // end event polling loop

		// end input
		// --------------------------------------


		// --------------------------------------
		// Update
		// --------------------------------------
		sf::Time frameTime = gameClock.restart();

		// end update
		// --------------------------------------



		// --------------------------------------
		// Draw
		// --------------------------------------

		// Clear the window to a single colour
		gameWindow.clear();

		// Draw Everything
		gameWindow.draw(testSprite);

		// Display the window contents to the screen
		gameWindow.display();

		// end draw
		// --------------------------------------

	} // end of the game while loop

	// end game loop
	// --------------------------------------

	// exit point for the program
	return 0;

} // end of the main() function

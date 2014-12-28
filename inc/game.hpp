#ifndef INC_GAME_H
#define INC_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "player.hpp"
#include "tile.hpp"

constexpr int windowWidth{ 600 }, windowHeight{ 600 };
constexpr int tileSize{ 60 };
constexpr float ftStep{ 1.f };
using MilliSec = float;

class Game
{
private:
	// These members are related to the control of the game.
	sf::RenderWindow window{ { windowWidth, windowHeight }, "Shady Platformer" };
	MilliSec lastFt{ 0.f }, currentSlice{ 0.f }; 
	sf::Clock clock;
	bool running{ false };

	// Game entities.
	std::vector<sf::Texture> textures;
	std::vector<std::unique_ptr<Tile>> tiles;
	Player player{ 50, 50 };

public:

	Game();

	void run();

	void inputPhase();

	void updatePhase();

	void drawPhase();
};

#endif
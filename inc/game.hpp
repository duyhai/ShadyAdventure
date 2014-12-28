#ifndef INC_GAME_H
#define INC_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "rectangle.hpp"

constexpr int windowWidth{ 600 }, windowHeight{ 600 };
constexpr int tilemapWidth{ 20 }, tilemapHeight{ 20 };
constexpr int tileSize{ 30 };
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
	std::vector<std::vector<Tile*>> tilemap;

public:

	Game();

	void addTile( Tile* tile );

	void run();

	void inputPhase();

	void updatePhase();

	void drawPhase();
};

#endif
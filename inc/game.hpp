#ifndef INC_GAME_H
#define INC_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "player.hpp"
#include "enemy.hpp"
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
	sf::View view{ sf::FloatRect( 0, 0, windowWidth, windowHeight ) };
	MilliSec lastFt{ 0.f }, currentSlice{ 0.f }; 
	sf::Clock clock;
	bool running{ false };

	// Game entities.
	std::vector<sf::Texture> textures;
	std::vector<Enemy> enemies;
	std::vector<std::unique_ptr<Tile>> tiles;
	Player player{ 0, 0 };

public:

	Game();

	void run();

	void inputPhase();

	void updatePhase();

	void drawPhase();
};

#endif
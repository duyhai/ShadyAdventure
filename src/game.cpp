#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include "game.hpp"
#include "rectangle.hpp"

using namespace std;
using namespace sf;

Game::Game()
{		
	window.setFramerateLimit( 30 );

	// Populate
	for ( int i = 0; i < 6; i++ )
	{
		textures.emplace_back();
		auto& texture = textures[i];
		ostringstream oss;
		oss << "data//tex" << i << ".jpg";
		if ( !texture.loadFromFile( oss.str() ) )
		{
			cout << "Error" << endl;
			continue;
		}
	    texture.setSmooth( true );
	    texture.setRepeated( true );
	}

	for ( int i = 0; i < tilemapWidth; i++ )
	{
		tilemap.emplace_back( tilemapHeight, nullptr );
	}
	addTile( new Rectangle( 150, 585, 300, 30, Color::White, &textures[0] ) );
	addTile( new Rectangle( 15, 345, 30, 90, Color::White, &textures[1] ) );
	addTile( new Rectangle( 430, 415, 60, 30, Color::White, &textures[2] ) );
}

void Game::addTile( Tile* tile )
{
	int beginX = tile->left() / tileSize,
		beginY = tile->top() / tileSize,
		endX = tile->right() / tileSize,
		endY = tile->bottom() / tileSize;
	if ( beginY < 0 || endY > tilemapHeight ||
			beginX < 0 || endX > tilemapWidth ) return;
	for ( int i = beginX; i < endX; i++ )
	{
		for ( int j = beginY; j < endY; j++ )
		{
			tilemap[i][j] = tile;
		}
	}
	for ( int j = 0; j < tilemapHeight; j++ )
	{
		for ( int i = 0; i < tilemapWidth; i++ )
		{
			if ( tilemap[i][j] )
				cout << 1;
			else
				cout << 0;
		}
		cout << endl;
	}
	tiles.emplace_back( tile );
}

void Game::run()
{
	running = true;

	while ( running )
	{
		Time timePoint1{ clock.getElapsedTime() };
		
		window.clear( Color::Black );
				
		inputPhase();
		updatePhase();
		drawPhase();		

		Time timePoint2{ clock.getElapsedTime() };
		Time elapsedTime{ timePoint2 - timePoint1 };
		
		lastFt = elapsedTime.asMicroseconds() / 1000.f;
		
		auto fps( 1.f /  elapsedTime.asSeconds() );
 
		window.setTitle( "FT: " + to_string( lastFt ) + "\tFPS: " + to_string( fps ) );
	}	
}

void Game::inputPhase()
{
	Event event;
	while ( window.pollEvent( event ) ) 
	{ 
		if ( event.type == Event::Closed ) 
		{
			window.close();
			break;
		}
	}

	if ( Keyboard::isKeyPressed( Keyboard::Key::Escape ) ) 
		running = false;
}

void Game::updatePhase()
{
	currentSlice += lastFt;
	for (; currentSlice >= ftStep; currentSlice -= ftStep )
	{		
		// Update

		// Test collision
	}
}

void Game::drawPhase()
{
	// Draw everything
	for ( auto& tile : tiles )
	{
		window.draw( *tile->getShape() );
	}

	window.display();
}
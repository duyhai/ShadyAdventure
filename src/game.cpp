#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "game.hpp"

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

	tiles.emplace_back( new Tile( 50, 50, 300, 300, Color::White, &textures[0] ) );
	player.setTexture( &textures[0] );
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

	player.setVelocity( 0.f, 0.f );
	float spd = 0.2f;

	if ( Keyboard::isKeyPressed( Keyboard::Key::A ) )		
		player.setVelocity( -spd, player.getVelocity().y );
	if ( Keyboard::isKeyPressed( Keyboard::Key::D ) )
		player.setVelocity( spd, player.getVelocity().y );
	if ( Keyboard::isKeyPressed( Keyboard::Key::W ) )
		player.setVelocity( player.getVelocity().x, -spd );
	if ( Keyboard::isKeyPressed( Keyboard::Key::S ) )
		player.setVelocity( player.getVelocity().x, spd );
}

void Game::updatePhase()
{
	currentSlice += lastFt;
	for (; currentSlice >= ftStep; currentSlice -= ftStep )
	{		
		// Update
		player.update( ftStep );

		// Test collision
	}
}

void Game::drawPhase()
{
	// Draw everything
	for ( auto& tile : tiles )
	{
		window.draw( tile->getShape() );
	}
	window.draw( player.getShape() );
	window.draw( player.getHead() );

	window.display();
}
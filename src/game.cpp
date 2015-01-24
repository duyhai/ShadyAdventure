#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include "bounded.hpp"
#include "game.hpp"
#include <utility>
#include "mapgen.hpp"
#include "helper.hpp"

using namespace std;
using namespace sf;

Game::Game()
{		
	window.setFramerateLimit( 30 );
	view.setViewport( sf::FloatRect( 0.f, 0.f, 1.f, 1.f ) );
	window.setView( view );

	// Populate

	// Texture
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

	// Map	
	ifstream ifs{ "data/map.txt" };
	createMap( ifs, tiles, textures );
	auto 	x = tiles[0].size(),
			y = tiles.size();

	// Player
	int playerX{ 0 }, playerY{ 0 };
	while ( tiles[playerY][playerX].get() == nullptr || !tiles[playerY][playerX]->getPassable() )
	{
		playerX = rand() % x;
		playerY = rand() % y;
	}
	auto playerpos = tiles[playerY][playerX]->getShape().getPosition();
	player.move( playerpos.x, playerpos.y );
	player.setTexture( &textures[0] );
	player.setOutlineColor( { 72, 113, 28 } );
	player.setOutlineThickness( 2.f );

	// Enemies
	for ( int i = 0; i < 5; i++ )
	{
		int tileX{ 0 }, tileY{ 0 };
		while ( tiles[tileY][tileX].get() == nullptr || !tiles[tileY][tileX]->getPassable() || 
				( tileX == playerX && tileY == playerY ) )
		{
			tileX = rand() % x;
			tileY = rand() % y;
		}
		auto pos = tiles[tileY][tileX]->getShape().getPosition();
		enemies.emplace_back( pos.x, pos.y );
		enemies[i].setTexture( &textures[std::rand() % 5 + 1] );
		enemies[i].setOutlineColor( { 80, 5, 5 } );
		enemies[i].setOutlineThickness( 2.f );
	}
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
	if ( Keyboard::isKeyPressed( Keyboard::Key::Space ) )
		player.setTexture( player.getHeldTileTexture() );
}

Tile* Game::collideBounded( Bounded& bounded )
{
	auto bounds = convertToBounds( bounded, tileSize );
	for ( int j = bounds.second.first; j < bounds.second.second; j++ )
	{
		for ( int k = bounds.first.first; k < bounds.first.second; k++ )
		{
			auto& tile = tiles[j][k];
			if ( tile.get() == nullptr ) continue;
			if ( !tile->getPassable() && tile->isIntersecting( bounded ) )
			{
				return tile.get();
			}
		}	
	}	
	return nullptr;
}

void Game::updateActors( float ftStep )
{
	player.setHeldTileTexture( nullptr );

	for ( auto& bullet : bullets )
	{
		bullet.update( ftStep );
	}

	for ( int i = 0; i < 2; i++ )
	{
		// Update
		player.update( i, ftStep );
		for ( auto& enemy : enemies )
		{
			enemy.update( i, ftStep );
		}

		// Test collision
		if ( Tile* tile = collideBounded( player ) )
		{
			player.setHeldTileTexture( tile->getTexture() );
			player.update( i, -ftStep );
		}	
		for ( auto& enemy : enemies )
		{
			if ( collideBounded( enemy ) )
				enemy.update( i, -ftStep );
		}	
	}
}

void Game::enemyAI()
{
	for ( auto& enemy : enemies )
	{
		if ( enemy.ready2Fire() )
		{
			bool tofire = true;
			auto enemypos = enemy.getShape().getPosition();
			auto playerpos = player.getShape().getPosition();
			auto mid = (playerpos + enemypos) * 0.5f;
			Tile scanarea{ mid.x, mid.y, abs( enemypos.x - playerpos.x ), abs( enemypos.y - playerpos.y ) };
			auto bounds = convertToBounds( scanarea , tileSize );
			bool brkloop = false;
			for ( int j = bounds.second.first; j < bounds.second.second; j++ )
			{
				for ( int k = bounds.first.first; k < bounds.first.second; k++ )
				{
					auto& tile = tiles[j][k];
					if ( tile.get() == nullptr || tile->getPassable() ) continue;
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->top() }, { tile->right(), tile->top() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->right(), tile->top() }, { tile->right(), tile->bottom() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->bottom() }, { tile->right(), tile->bottom() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->top() }, { tile->left(), tile->bottom() } );
					if ( !tofire ) { brkloop = true; break; }
				}	
				if ( brkloop ) break;
			}
			if ( tofire )
			{
				auto v = playerpos-enemypos;
				float len = sqrt( v.x*v.x+v.y*v.y );
				if ( abs( len ) > 0.001 )
					v /= len;
				v *= 0.3f;
				bullets.emplace_back( 5, enemypos.x, enemypos.y, v, Color::Red, enemy.getTexture() );
				enemy.fire();
			}
		}
	}
}

void Game::updatePhase()
{
	currentSlice += lastFt;
	for (; currentSlice >= ftStep; currentSlice -= ftStep )
	{	
		updateActors( ftStep );
		enemyAI();		
	}
	view.setCenter( player.getShape().getPosition() );
	window.setView( view );
}

void Game::drawPhase()
{
	// Draw everything
	for ( auto& tilerows : tiles )
	{
		if ( tilerows.size() == 0 ) continue;
		for ( auto& tile : tilerows )
		{
			if ( tile.get() == nullptr ) continue;
			window.draw( tile->getShape() );
		}
	}
	for ( auto& enemy : enemies )
	{
		window.draw( enemy.getShape() );
		window.draw( enemy.getHead() );
	}
	for ( auto& bullet : bullets )
	{
		window.draw( bullet.getShape() );
	}
	window.draw( player.getShape() );
	window.draw( player.getHead() );

	window.display();
}
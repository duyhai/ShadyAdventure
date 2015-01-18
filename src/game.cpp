#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "game.hpp"

using namespace std;
using namespace sf;

bool linesIntersect( sf::Vector2f x1, sf::Vector2f x2, sf::Vector2f y1, sf::Vector2f y2 )
{
	auto 	v1 = x2 - x1,
			v2 = y2 - y1;

	float	x00 = x1.x, y00 = x1.y,
			x10 = y1.x, y10 = y1.y,
			x01 = v1.x, y01 = v1.y,
			x11 = v2.x, y11 = v2.y;

	float d = x11 * y01 - x01 * y11;

	if ( abs ( d ) < 0.001 ) return false;

	float 	s = ( 1/d ) * ( ( x00 - x10 ) * y01 - ( y00 - y10 ) * x01 ),
			t = -( 1/d ) * ( -( x00 - x10 ) * y11 + ( y00 - y10 ) * x11 );
	return ( s <= 1.f && t <= 1.f && s >= 0.f && t >= 0.f );
}

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
	string input;

	int row{ 0 };
	while ( getline( ifs, input ) )
	{
		int column{ -1 };
		for ( char c:input )
		{
			column++;
			if ( c == '-' ) continue;

			Tile* tile = new Tile( ( column + 0.5f ) * tileSize, ( row + 0.5f ) * tileSize,
									 tileSize, tileSize, Color::White, &textures[c-'0'] );
			tile->setOutlineColor( Color::Black );
			tile->setOutlineThickness( 2.f );
			if ( c != '3' )
			{
				tile->setPassable( true );
			}
			tiles.emplace_back( tile );
		}
		row++;
	}

	// Player
	int playernum{ 0 };
	for ( int tilenum = tiles.size(); !tiles[playernum]->getPassable(); playernum = rand() % tilenum );
	auto playerpos = tiles[playernum]->getShape().getPosition();
	player.move( playerpos.x, playerpos.y );
	player.setTexture( &textures[0] );
	player.setOutlineColor( { 72, 113, 28 } );
	player.setOutlineThickness( 2.f );

	// Enemies
	for ( int i = 0; i < 5; i++ )
	{
		int num{ 0 };
		for ( int tilenum = tiles.size(); !tiles[num]->getPassable() && num != playernum; num = rand() % tilenum );
		auto pos = tiles[num]->getShape().getPosition();
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

void Game::updatePhase()
{
	currentSlice += lastFt;
	for (; currentSlice >= ftStep; currentSlice -= ftStep )
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
			for ( auto& tile : tiles )
			{
				if ( !tile->getPassable() && tile->isIntersecting( player ) )
				{
					player.setHeldTileTexture( tile->getTexture() );
					player.update( i, -ftStep );
					break;
				}				
			}	
			for ( auto& enemy : enemies )
			{
				for ( auto& tile : tiles )
				{
					if ( !tile->getPassable() && tile->isIntersecting( enemy ) )
					{
						enemy.update( i, -ftStep-0.001f );
						break;
					}				
				}
			}		
		}

		for ( auto& enemy : enemies )
		{
			if ( enemy.ready2Fire() )
			{
				bool tofire = true;
				auto enemypos = enemy.getShape().getPosition();
				auto playerpos = player.getShape().getPosition();
				for ( auto& tile : tiles )
				{
					if ( tile->getPassable() ) continue;
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->top() }, { tile->right(), tile->top() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->right(), tile->top() }, { tile->right(), tile->bottom() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->bottom() }, { tile->right(), tile->bottom() } );
					tofire &= !linesIntersect( enemypos, playerpos, { tile->left(), tile->top() }, { tile->left(), tile->bottom() } );
					if ( !tofire ) break;
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
	view.setCenter( player.getShape().getPosition() );
	window.setView( view );
}

void Game::drawPhase()
{
	// Draw everything
	for ( auto& tile : tiles )
	{
		window.draw( tile->getShape() );
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
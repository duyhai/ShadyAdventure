#ifndef INC_MAPGEN_HPP
#define INC_MAPGEN_HPP
	
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include <iostream>

void createMap( std::istream& is, std::vector<std::vector<std::unique_ptr<Tile>>>& tiles, std::vector<sf::Texture>& textures )
{
	std::string input;
	getline( is, input );
	int x{ stoi( input ) };
	getline( is, input );
	int y{ stoi( input ) };

	int row{ 0 };
	while ( getline( is, input ) )
	{
		tiles.emplace_back();
		int column{ -1 };
		for ( char c:input )
		{
			column++;
			if ( c == '-' ) 
			{
				tiles[row].emplace_back();				
				continue;
			}

			Tile* tile = new Tile( ( column + 0.5f ) * tileSize, ( row + 0.5f ) * tileSize,
									 tileSize, tileSize, sf::Color::White, &textures[c-'0'] );
			tile->setOutlineColor( sf::Color::Black );
			tile->setOutlineThickness( 2.f );
			if ( c != '3' )
			{
				tile->setPassable( true );
			}
			tiles[row].emplace_back( tile );
		}
		for ( int i = ++column; i < x; i++ ) tiles[row].emplace_back( nullptr );
		row++;
	}
	for ( int i = row; i < y; i++ ) tiles.emplace_back( x );
}

#endif
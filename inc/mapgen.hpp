#ifndef INC_MAPGEN_HPP
#define INC_MAPGEN_HPP
	
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "tile.hpp"
#include <iostream>
#include <vector>

std::vector<char> PASSABLE{ '0' };

int wallsNearby( const std::vector<std::string>& map, int x, int y, int radius )
{
	if ( map.size() == 0 ) return 0;
	int beginX{ std::max<int>( 0, x - radius ) },
		endX{ std::min<int>( map[0].size() - 1, x + radius ) },
		beginY{ std::max<int>( 0, y - radius ) },
		endY{ std::min<int>( map.size() - 1, y + radius ) },
		diameter{ 2 * radius + 1 },
		count{ 0 };

	for ( int i = beginY; i <= endY; i++ )
	{			
		for ( int j = beginX; j <= endX; j++ )
		{
			count += map[i][j] == '1' ? 1 : 0;
		}
	}

	int populatedRegionX = endX - beginX + 1;
	int populatedRegionY = endY - beginY + 1;
	count += diameter * ( diameter - populatedRegionY );
	count += ( diameter - populatedRegionX ) * populatedRegionY;
	return count;
}

void printmap( const std::vector<std::string>& map )
{
	for ( const std::string& s : map )
		std::cout << s << std::endl;
}

// http://www.roguebasin.com/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels
std::vector<std::string> generateMap( int width, int height )
{
	std::vector<std::string> map[2];
	for ( int i = 0; i < height; i++ )
	{
		if ( i == 0 || i == height - 1 )
		{
			map[0].emplace_back( width, '1' );
			map[1].emplace_back( width, '1' );
		}
		else
		{
			map[0].push_back( "1" + std::string( width - 2, '0' ) + "1" );
			map[1].push_back( "1" + std::string( width - 2, '0' ) + "1" );
		}
	}
	for ( int i = 1; i < map[0].size() - 1; i++ )
	{
		for ( int j = 1; j < map[0][i].length() - 1; j++ )
		{
			char& c = map[0][i][j];
			c = rand() % 100 < 40 ? '1' : '0';			
		}
	}

	int phase{ 0 };
	for ( int k = 0; k < 4; k++ )
	{
		int currphase = phase % 2;
		for ( int i = 0; i < map[currphase].size(); i++ )
		{
			for ( int j = 0; j < map[currphase][i].length(); j++ )
			{
				bool makewall = wallsNearby( map[currphase], j, i, 1 ) >= 5 || wallsNearby( map[currphase], j, i, 2 ) <= 2;
				map[( phase + 1 ) % 2][i][j] = makewall ? '1' : '0';		
			}
		}
		phase++;
	}
	for ( int k = 0; k < 3; k++ )
	{
		int currphase = phase % 2;
		for ( int i = 0; i < map[currphase].size(); i++ )
		{
			for ( int j = 0; j < map[currphase][i].length(); j++ )
			{
				bool makewall = wallsNearby( map[currphase], j, i, 1 ) >= 5;
				map[( phase + 1 ) % 2][i][j] = makewall ? '1' : '0';				 
			}
		}
		phase++;
	}
	int currphase = phase % 2;
	for ( std::string& s : map[currphase] )
		for ( char& c : s )
			if ( c == '1' )
				c = rand() % 5 + 1 + '0';
	map[currphase].insert( map[currphase].begin(), std::to_string( height ) );
	map[currphase].insert( map[currphase].begin(), std::to_string( width ) );
	return map[currphase];
}

void createMap( const std::vector<std::string>& map, std::vector<std::vector<std::unique_ptr<Tile>>>& tiles, std::vector<sf::Texture>& textures, int tileSize )
{
	if ( map.size() < 3 ) return;

	int x{ stoi( map[0] ) },
	    y{ stoi( map[1] ) },
	    row{ 0 };
	for ( int i = 2; i < map.size(); i++ )
	{
		const std::string& input = map[i];
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
			tile->setOutlineThickness( -1.f );
			if ( std::find( PASSABLE.begin(), PASSABLE.end(), c ) != PASSABLE.end() )
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

void createMap( std::istream& is, std::vector<std::vector<std::unique_ptr<Tile>>>& tiles, std::vector<sf::Texture>& textures, int tileSize )
{
	std::string input;
	std::vector<std::string> map;

	while ( std::getline( is, input ) )
	{
		map.push_back( input );
	}

	createMap( map, tiles, textures, tileSize );
}

#endif
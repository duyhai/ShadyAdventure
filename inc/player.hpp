#ifndef INC_PLAYER_H
#define INC_PLAYER_H

#include <SFML/Graphics.hpp>
#include "character.hpp"

class Player: public Character
{
private:

	const sf::Texture* heldTileTex{ nullptr };

public:

	Player( float mX, float mY ): Character( mX, mY ){}

	void setHeldTileTexture( const sf::Texture* tex )
	{		
		heldTileTex = tex;
	}

	const sf::Texture* getHeldTileTexture() noexcept
	{
		return heldTileTex;
	}

};

#endif
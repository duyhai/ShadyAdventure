#ifndef INC_TILE_H
#define INC_TILE_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "character.hpp"

class Tile
{
private:
	bool mPassable = false;	
	sf::RectangleShape mShape;

public:
	Tile( float mX, float mY, float width, float height, 
		sf::Color c = sf::Color::White, sf::Texture* texture = nullptr )
	{
		mShape.setSize( { width, height } );
		mShape.setFillColor( c );
		mShape.setPosition( mX, mY );
		mShape.setOrigin( width / 2.f, height / 2.f );
		if ( texture == nullptr ) return;		
	    mShape.setTexture( texture );
	    mShape.setTextureRect( sf::IntRect( left(), top(), width, height ) );
	}

	void setTexture( const sf::Texture* tex )
	{		
		if ( tex == nullptr ) return;		
	    mShape.setTexture( tex );
	}

	const sf::Texture* getTexture() noexcept
	{
		return mShape.getTexture();
	}

	sf::Shape& getShape()		 							noexcept { return mShape; }
	void setOutlineColor( const sf::Color &color ) 			noexcept { mShape.setOutlineColor( color ); }
	sf::Color getOutlineColor() const 						noexcept { return mShape.getOutlineColor(); }
	void setOutlineThickness( float thickness ) 			noexcept { mShape.setOutlineThickness( thickness ); }
	float getOutlineThickness() const 						noexcept { return mShape.getOutlineThickness(); }

	void setPassable( bool passable ) noexcept { mPassable = passable; }
	bool getPassable() noexcept { return mPassable; }

	float x() 		const noexcept { return mShape.getPosition().x; }
	float y() 		const noexcept { return mShape.getPosition().y; }
	float left() 	const noexcept { return x() - mShape.getSize().x / 2.f; }
	float right() 	const noexcept { return x() + mShape.getSize().x / 2.f; }
	float top() 	const noexcept { return y() - mShape.getSize().y / 2.f; }
	float bottom() 	const noexcept { return y() + mShape.getSize().y / 2.f; }

	bool isIntersecting( Tile& mA ) noexcept
	{
		return mA.right() >= left() && mA.left() <= right() 
				&& mA.bottom() >= top() && mA.top() <= bottom();
	}

	bool isIntersecting( Character& mA ) noexcept
	{
		return mA.right() >= left() && mA.left() <= right() 
				&& mA.bottom() >= top() && mA.top() <= bottom();
	}
};

#endif
#ifndef INC_RECTANGLE_H
#define INC_RECTANGLE_H

#include <SFML/Graphics.hpp>
#include "tile.hpp"

class Rectangle: public Tile
{
public:
	sf::RectangleShape mShape;

	Rectangle( float mX, float mY, float width, float height, 
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

	sf::Shape* getShape()		 							noexcept { return &mShape; }
	void setOutlineColor( const sf::Color &color ) 			noexcept { mShape.setOutlineColor( color ); }
	sf::Color getOutlineColor() const 						noexcept { return mShape.getOutlineColor(); }
	void setOutlineThickness( float thickness ) 			noexcept { mShape.setOutlineThickness( thickness ); }
	float getOutlineThickness() const 						noexcept { return mShape.getOutlineThickness(); }

	float x() 		const noexcept { return mShape.getPosition().x; }
	float y() 		const noexcept { return mShape.getPosition().y; }
	float left() 	const noexcept { return x() - mShape.getSize().x / 2.f; }
	float right() 	const noexcept { return x() + mShape.getSize().x / 2.f; }
	float top() 	const noexcept { return y() - mShape.getSize().y / 2.f; }
	float bottom() 	const noexcept { return y() + mShape.getSize().y / 2.f; }
};

#endif
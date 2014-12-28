#ifndef INC_PLAYER_H
#define INC_PLAYER_H

#include <SFML/Graphics.hpp>
#include "tile.hpp"

class Player: public Tile
{
private:

	sf::Vector2f velocity{ 0.f, 0.f };
	sf::CircleShape head{ 10, 10 };
	sf::RectangleShape body{ sf::Vector2f { 30, 40 } };

public:

	Player( float mX, float mY )
	{		
		body.setPosition( mX, mY+10 );
		body.setOrigin( 15, 20 );
		head.setPosition( mX, mY-20 );
		head.setOrigin( 10, 10 );
	}

	void setTexture( const sf::Texture* tex )
	{		
		if ( tex == nullptr ) return;		
	    body.setTexture( tex );
	    body.setTextureRect( sf::IntRect( left(), top(), 30, 40 ) );
	    head.setTexture( tex );
	    head.setTextureRect( sf::IntRect( head.getPosition().x - 10, head.getPosition().y - 10, 10, 10 ) );
	}

	const sf::Texture* getTexture() noexcept
	{
		return body.getTexture();
	}

	void move( float mX, float mY )
	{
		body.move( mX, mY );
		head.move( 0, mY );
		int offset = mX < 0 ? 4 : -4;
		head.setPosition( body.getPosition().x + offset, head.getPosition().y );
	    body.setTextureRect( sf::IntRect( left(), top(), 30, 40 ) );
	    head.setTextureRect( sf::IntRect( head.getPosition().x - 10, head.getPosition().y - 10, 10, 10 ) );
	}

	void setVelocity( float x, float y )
	{
		velocity.x = x;
		velocity.y = y;
	}

	const sf::Vector2f& getVelocity( ) const noexcept
	{
		return velocity;
	}

	sf::Shape* getHead()  noexcept
	{
		return &head;
	}

	sf::Shape* getShape()  									noexcept { return &body; }

	void setOutlineColor( const sf::Color &color ) 			noexcept { body.setOutlineColor( color ); }
	sf::Color getOutlineColor() const 						noexcept { return body.getOutlineColor(); }
	void setOutlineThickness( float thickness ) 			noexcept { body.setOutlineThickness( thickness ); }
	float getOutlineThickness() const						noexcept { return body.getOutlineThickness(); }

	float x() 		const noexcept { return body.getPosition().x; }
	float y() 		const noexcept { return body.getPosition().y; }
	float left() 	const noexcept { return x() - body.getSize().x / 2.f; }
	float right() 	const noexcept { return x() + body.getSize().x / 2.f; }
	float top() 	const noexcept { return y() - body.getSize().y / 2.f; }
	float bottom() 	const noexcept { return y() + body.getSize().y / 2.f; }

	void update( float time )
	{
		velocity.y += 0.001f*time;
		move( velocity.x*time, velocity.y*time );
	}
};

#endif
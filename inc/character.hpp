#ifndef INC_CHARACTER_H
#define INC_CHARACTER_H

#include <SFML/Graphics.hpp>

constexpr int bwidth{ 20 }, bheight{ 30 }, hradius{ 8 };

class Character
{
private:

	sf::CircleShape head{ hradius, 10 };
	sf::ConvexShape body{ 4 };
	sf::Vector2f velocity{ 0.f, 0.f };

	void alignTexture()
	{
	    body.setTextureRect( sf::IntRect( left(), top() + head.getRadius() * 2.f, bwidth, bheight ) );
	    head.setTextureRect( sf::IntRect( head.getPosition().x - hradius, head.getPosition().y - hradius, hradius*2, hradius*2 ) );		
	}

public:

	Character( float mX, float mY )
	{		
		body.setPoint( 0, sf::Vector2f { bwidth * 0.5f, bheight * 0.5f } );
		body.setPoint( 1, sf::Vector2f { bwidth * 0.25f, -bheight * 0.5f } );
		body.setPoint( 2, sf::Vector2f { -bwidth * 0.25f, -bheight * 0.5f } );
		body.setPoint( 3, sf::Vector2f { -bwidth * 0.5f, bheight * 0.5f } );
		body.setPosition( mX, mY+bheight * 0.25f );
		head.setPosition( mX, mY - bheight * 0.25f - hradius );
		head.setOrigin( hradius, 10 );
	}

	void setTexture( const sf::Texture* tex )
	{		
		if ( tex == nullptr ) return;		
	    body.setTexture( tex );
	    head.setTexture( tex );
	    alignTexture();
	}

	const sf::Texture* getTexture() noexcept
	{
		return body.getTexture();
	}

	void move( float mX, float mY ) noexcept
	{
		body.move( mX, mY );
		head.move( 0, mY );
		int offset = velocity.x * 20;
		body.setPoint( 1, { bwidth * 0.25f + offset, -bheight * 0.5f } );
		body.setPoint( 2, { -bwidth * 0.25f + offset, -bheight * 0.5f } );
		head.setPosition( body.getPosition().x + offset, head.getPosition().y );

		alignTexture();
	}

	void setVelocity( float x, float y ) noexcept
	{
		velocity.x = x;
		velocity.y = y;
	}

	const sf::Vector2f& getVelocity(  ) noexcept
	{
		return velocity;
	}

	sf::Shape& getHead()  									noexcept { return head; }
	sf::Shape& getShape()  									noexcept { return body; }

	void setOutlineColor( const sf::Color &color ) 			noexcept { body.setOutlineColor( color ); head.setOutlineColor( color ); }
	sf::Color getOutlineColor() const 						noexcept { return body.getOutlineColor(); }
	void setOutlineThickness( float thickness ) 			noexcept { body.setOutlineThickness( thickness ); head.setOutlineThickness( thickness ); }
	float getOutlineThickness() const						noexcept { return body.getOutlineThickness(); }

	float x() 		const noexcept { return body.getPosition().x; }
	float y() 		const noexcept { return body.getPosition().y; }
	float left() 	const noexcept { return x() - bwidth / 2.f; }
	float right() 	const noexcept { return x() + bwidth / 2.f; }
	float top() 	const noexcept { return y() - bheight / 2.f - head.getRadius() * 2.f; }
	float bottom() 	const noexcept { return y() + bheight / 2.f; }

	virtual void update( int phase, float time )
	{
		if ( phase == 0 )
			move( velocity.x*time, 0 );
		if ( phase == 1 )
			move( 0, velocity.y*time );
	}
};

#endif
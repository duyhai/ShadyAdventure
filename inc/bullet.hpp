#ifndef INC_BULLET_HPP
#define INC_BULLET_HPP

#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::CircleShape mShape;
	sf::Vector2f mVelocity;

	void alignTexture()
	{
	    mShape.setTextureRect( sf::IntRect( left(), top() + mShape.getRadius() * 2.f, bwidth, bheight ) );
	}

public:
	Bullet( float radius, float mX, float mY, sf::Vector2f v,
			sf::Color c = sf::Color::White, const sf::Texture* texture = nullptr ): mVelocity( v )
	{
		mShape.setPosition( mX, mY );
		mShape.setFillColor( c );
		mShape.setRadius( radius );
		mShape.setOrigin( radius, radius );
		setOutlineColor( { 80, 5, 5 } );
		setOutlineThickness( 2.f );

		if ( texture == nullptr ) return;		
		mShape.setTexture( texture );
		alignTexture();
	}

	void setTexture( const sf::Texture* tex )
	{		
		if ( tex == nullptr ) return;		
	    mShape.setTexture( tex );
	    alignTexture();
	}

	const sf::Texture* getTexture() noexcept
	{
		return mShape.getTexture();
	}

	void setPosition( float mX, float mY ) noexcept
	{
		auto& pos = getShape().getPosition();
		move( mX - pos.x, mY - pos.y );
	}

	void move( float mX, float mY ) noexcept
	{
		mShape.move( mX, mY );
		alignTexture();
	}

	void setVelocity( float x, float y ) noexcept
	{
		mVelocity.x = x;
		mVelocity.y = y;
	}

	const sf::Vector2f& getVelocity(  ) noexcept
	{
		return mVelocity;
	}

	sf::Shape& getShape()  									noexcept { return mShape; }

	void setOutlineColor( const sf::Color &color ) 			noexcept { mShape.setOutlineColor( color ); }
	sf::Color getOutlineColor() const 						noexcept { return mShape.getOutlineColor(); }
	void setOutlineThickness( float thickness ) 			noexcept { mShape.setOutlineThickness( thickness ); }
	float getOutlineThickness() const						noexcept { return mShape.getOutlineThickness(); }

	float x() 		const noexcept { return mShape.getPosition().x; }
	float y() 		const noexcept { return mShape.getPosition().y; }
	float left() 	const noexcept { return x() - mShape.getRadius(); }
	float right() 	const noexcept { return x() + mShape.getRadius(); }
	float top() 	const noexcept { return y() - mShape.getRadius(); }
	float bottom() 	const noexcept { return y() + mShape.getRadius(); }

	virtual void update( float time )
	{
		move( mVelocity.x*time, mVelocity.y*time );
	}

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
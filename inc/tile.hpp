#ifndef INC_TILE_H
#define INC_TILE_H

#include <memory>
#include <SFML/Graphics.hpp>

class Tile
{
private:
	bool mPassable = false;	

public:
	virtual sf::Shape* getShape() noexcept = 0;
	virtual void setOutlineColor( const sf::Color &color ) noexcept = 0;
	virtual sf::Color getOutlineColor() const noexcept = 0;
	virtual void setOutlineThickness( float thickness ) noexcept = 0;
	virtual float getOutlineThickness() const noexcept = 0;

	void setPassable( bool passable ) noexcept { mPassable = passable; }
	bool getPassable() noexcept { return mPassable; }

	virtual float left() const noexcept = 0;
	virtual float right() const noexcept = 0;
	virtual float top() const noexcept = 0;
	virtual float bottom() const noexcept = 0;

	bool isIntersecting( Tile& mA, Tile& mB ) noexcept
	{
		return mA.right() >= mB.left() && mA.left() <= mB.right() 
				&& mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
	}
};

#endif
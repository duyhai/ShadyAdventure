#ifndef INC_HELPER_HPP
#define INC_HELPER_HPP

#include "bounded.hpp"
#include <SFML/Graphics.hpp>
#include <utility>


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

std::pair<std::pair<int, int>, std::pair<int, int>> convertToBounds( Bounded& rect, int tileSize )
{
	return { { rect.left() / tileSize, rect.right() / tileSize + 1 },
			 { rect.top() / tileSize, rect.bottom() / tileSize + 1 } };
}

#endif
#ifndef INC_ENEMY_H
#define INC_ENEMY_H

#include "character.hpp"
#include <cstdlib>

class Enemy: public Character
{
private:
	int fireRate{ 60 }, firePhase{ 0 };
	int walkRate{ 900 }, walkPhase{ 0 };

public:
	Enemy( float mX, float mY ): Character( mX, mY ){}

	void update( int phase, float time )
	{
		if ( walkPhase % walkRate == 0 && time >= 0 )
		{
			if ( phase == 0 )
				setVelocity( static_cast<float>( std::rand() % 30 - 15 ) / 100.f, getVelocity().y );
			if ( phase == 1 )
				setVelocity( getVelocity().x, static_cast<float>( std::rand() % 30 - 15 ) / 100.f );			
		}

		Character::update( phase, time );

		if ( time < 0 || phase == 0 ) return;

		firePhase++;
		walkPhase++;

		if ( firePhase % fireRate == 0 )
		{
			// shoot		
		}
	}
};

#endif
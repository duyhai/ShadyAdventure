#include "game.hpp"
#include <cstdlib>
#include <ctime>

int main() 
{
	std::srand( std::time( NULL ) );
	Game{}.run();	

	return 0;
}
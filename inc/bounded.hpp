#ifndef INC_BOUNDED_HPP
#define INC_BOUNDED_HPP

class Bounded
{
public:
	virtual float x() 		const noexcept = 0;
	virtual float y() 		const noexcept = 0;
	virtual float left() 	const noexcept = 0;
	virtual float right() 	const noexcept = 0;
	virtual float top() 	const noexcept = 0;
	virtual float bottom() 	const noexcept = 0;
};

#endif
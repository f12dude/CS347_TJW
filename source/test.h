#ifndef _TEST_
#define _TEST_

#include "grid.h"

class TestGrid : public Grid
{
public:

	using Grid::Grid;

	virtual void RunAI();

};

#endif // _TEST_

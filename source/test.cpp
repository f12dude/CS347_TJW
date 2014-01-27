#include "test.h"
#include <ctime>
#include <stack>
#include <cassert>
#include <unordered_set>
#include <iostream>

using namespace std;

void TestGrid::RunAI()
{
	unsigned int counter = 0;
	GenerateGraph(0,1,3, counter); // 3

	cout << counter << endl;
}

void TestGrid::GenerateGraph(unsigned int w, unsigned int h, unsigned int d, unsigned int& counter)
{
	// Algorithm to build the state tree:
	// Todo: store the direction index in the stack so that when we come back up the tree, we do not loop back down the same path
	// While there are wrigglers in the queue, do the following:
	// Get the wriggler in the front of the queue and pop him off, call him top.
	// Move the wriggler that was popped off of the queue
	// For all 8 different possibilies for each wriggler at its current location
	//		If moving in this direction can be done,
	//			Add the move to the stack along with the current indicies(I need to check this out a bit more)
	// Unmove the wriggler

	static std::unordered_set<Wriggler, WrigglerHash> mySet;

	// Move the wriggler
	if(MoveWriggler(w,h,(Direction)d))
	{
		if(mySet.insert(m_wrigglers[w]).second)
		{
			cout << "Load Factor: " << mySet.load_factor() << endl;
			cout << *this << endl;
			++counter;

			// Try to move all the wrigglers
			for(unsigned int w = 0; w < GetNumWrigglers(); ++w)
			{
				// Try to move the head and the tail
				for(unsigned int h = 0; h < 2; ++h)
				{
					// Try to move in all four directions
					for(unsigned int d = 0; d < 4; ++d)
					{
						GenerateGraph(w,h,d,counter);
					}
				}
			}
		}

		// Todo: need to move the wriggler back
		//MoveWriggler(w,~h,(Direction)d);
	}


}

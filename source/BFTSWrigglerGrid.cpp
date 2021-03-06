#include "BFTSWrigglerGrid.h"
#include "Timer.h"
#include <iostream>
#include <cassert>

using namespace std;

BFTSWrigglerGrid::BFTSWrigglerGrid() : m_wallTime(0)
{
}

BFTSWrigglerGrid::BFTSWrigglerGrid(const string& file) : m_wallTime(0)
{
	if(!Load(file))
	{
		throw std::string("Cannot open " + file);
	}
}

bool BFTSWrigglerGrid::Load(const string& file)
{
	bool bSuccess = WrigglerGrid::Load(file);
	if(bSuccess)
	{
		Node* pTree = new Node();

		m_tree.Clear();
		m_tree.SetRoot(pTree);
		m_tree.AddNode(pTree);

		Timer theTimer;
		theTimer.Start();

		std::unordered_set<Wriggler,WrigglerHash> closedList;
		GenerateTree(closedList, pTree);

		m_wallTime += theTimer.GetTime();
	}

	return bSuccess;
}

void BFTSWrigglerGrid::RunAI()
{
	Timer theTimer;
	theTimer.Start();

	// Find a path that will solve the puzzle
	std::list<Node*> path;
	m_tree.BreadthFirstSearch(path,[this](const Node& n) -> bool
	{
		bool bFoundFinalState = false;

		// Final state check
		if(n.pPrevious != nullptr)
		{
			if(m_wrigglers[n.move.w].id == 0)
			{
				// Check if the blue wriggler has moved to the bottom right corner of the grid
				uvec2 finalPos = { m_uiWidth - 1, m_uiHeight - 1 };
				if(n.head == finalPos || n.tail == finalPos)
				{
					bFoundFinalState = true;
				}
			}
		}

		return bFoundFinalState;

	});

	m_wallTime += theTimer.GetTime();

	// Draw the path that was found
	for(const Node* pNode : path)
	{
		uvec2 pos = pNode->move.h ? pNode->head : pNode->tail;
		cout << pNode->move.w << " " << !pNode->move.h << " " << pos.x << " " << pos.y << endl;

		MoveWriggler(pNode->move);
	}

	DisplayResults(m_wallTime, path.size());
}

void BFTSWrigglerGrid::GenerateTree(std::unordered_set<Wriggler, WrigglerHash>& closedList, Node* pTree)
{
	// Try to move the wriggler in 8 different directions, 4 for the head and the tail
	//		If unique position for the wriggler in the current path
	//			Add new node to the graph
	//			Call GenerateGraph()
	//		Unmove the wriggler

	// Try to move all the wrigglers
	for(unsigned int w = 0; w < GetNumWrigglers(); ++w)
	{
		// Try to move in all four directions
		for(Direction d : {Up, Down, Left, Right})
		{
			// Try to move the head and the tail
			for(bool h : {true, false})
			{
				Direction dir = GetGetWrigglerTailDir(w,h);
				if(MoveWriggler({w,h,d}))
				{
					bool bProcessOnce = closedList.insert(m_wrigglers[w]).second;

					if(bProcessOnce)
					{
						// If we can move and we have not processed this position on this path, 
						// create a new node that is part of this path

						auto* pNewNode = new Node();
						pNewNode->pPrevious = pTree;
						pNewNode->move = {w,h,d};
						pNewNode->head = m_wrigglers[w].positions.front();
						pNewNode->tail = m_wrigglers[w].positions.back();
						pNewNode->pPrevious->nodes.push_back(pNewNode);

						m_tree.AddNode(pNewNode);

						// Try to continue moving
						GenerateTree(closedList,pNewNode);

						// Cannot move anymore, so um-mark this position as we backtrack
						closedList.erase(m_wrigglers[w]);
					}

					// Move the wriggler back as we are backtracking,
					// if this wriggler fails to move, then something is really wrong
					bool bMovedBack = MoveWriggler({w,!h,dir});
					assert("Cannot move wriggler back" && bMovedBack);
				}
			}

		}

	}

}

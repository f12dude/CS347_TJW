#ifndef _BFTS_GRID_
#define _BFTS_GRID_

#include "Grid.h"
#include "Tree.h"
#include "vec2.h"
#include <unordered_set>

// Defines a node that is part of a search tree
struct Node
{
	Node() : pPrevious(nullptr)
	{
	}

	// A list of all children of this node
	std::vector<Node*> nodes;

	// Pointer to the previous Node in the tree
	Node* pPrevious;

	// Head of the wriggler
	uvec2 head;

	// Tail of the wriggler
	uvec2 tail;

	// Movement from the previous state to this state
	Move move;
};

// Breadth First Tree Search Wriggler AI
class BFTSGrid : public Grid
{
public:

	BFTSGrid();
	BFTSGrid(const std::string& file);

	// Loads puzzle description from the specified file
	// The old state of the grid is discarded
	bool Load(const std::string& file);

	// Solves the wriggler puzzle and prints the solution using BFTS
	virtual void RunAI();

private:

	Tree<Node> m_tree;

	void GenerateTree(std::unordered_set<Wriggler, WrigglerHash> &closedList, Node *pTree);

};

#endif // _BFTS_GRID_
